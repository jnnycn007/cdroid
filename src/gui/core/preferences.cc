#include <preferences.h>
#include <iostream>
#include <sstream>
#include <fstream>
#if defined(_WIN32)||defined(_WIN64)
  #include <fcntl.h>
  #include <io.h>
  #ifndef F_OK
    #define F_OK 0
    #define W_OK 2
    #define R_OK 4
  #endif
#elif defined(__linux__)||defined(__unix__)
  #include <unistd.h>
#endif
#include <cdtypes.h>
#include <cdlog.h>
#include <expat.h>
#include <string.h>
#include <textutils.h>
#include <core/attributeset.h>
#include <core/iostreams.h>

namespace cdroid{

Preferences::Preferences(){
   updates=0;
}

Preferences::~Preferences(){
   if(updates&&!mFileName.empty())
       save(mFileName);
}
typedef struct{
   Preferences*pref;
   std::string section;
   std::string key;
   std::string value;
}PREFPARSER;

static void startElement(void *userData, const XML_Char *name, const XML_Char **satts){
    PREFPARSER*kvp=(PREFPARSER*)userData;
    if(strcmp(name,"section")==0){//root node is not in KVPARSER::attrs
        AttributeSet atts;
        atts.set(satts);
        kvp->section=atts.getString("name");
    }else if(strcmp(name,"item")==0){
        AttributeSet atts;
        atts.set(satts);
	kvp->key=atts.getString("name");
        kvp->value=std::string();
    }
}

static void CharacterHandler(void *userData,const XML_Char *s, int len){
    PREFPARSER*kvp=(PREFPARSER*)userData;
    kvp->value+=std::string(s,len);
}

static void endElement(void *userData, const XML_Char *name){
    PREFPARSER*kvp=(PREFPARSER*)userData;
    if(strcmp(name,"item")==0){//root node is not in KVPARSER::attrs
        TextUtils::trim(kvp->value);
	kvp->pref->setValue(kvp->section,kvp->key,kvp->value);
    }
}

void Preferences::load(const std::string&fname){
    if(access(fname.c_str(),F_OK)==0){
        std::ifstream fin(fname);
        mFileName=fname;
        load(fin);
    }else if(fname.find("<sections>")!=std::string::npos){
        load(fname.c_str(),fname.length());
    }
}

void Preferences::load(const char*buf,size_t len){
    if(buf&&len){
        MemoryInputStream stream(buf,len);
        load(stream);
    }
}

void Preferences::load(std::istream&istream){
    XML_Parser parser=XML_ParserCreate(nullptr);
    int len = 0;
    PREFPARSER kvp;
    kvp.pref=this;
    XML_SetUserData(parser,&kvp);
    XML_SetElementHandler(parser, startElement, endElement);
    XML_SetCharacterDataHandler(parser,CharacterHandler);
    if((istream.good()==false)||istream.eof())return;
    do {
        std::string str;
        std::getline(istream,str);
        len=str.length();
        if (XML_Parse(parser, str.c_str(),len,len==0) == XML_STATUS_ERROR) {
            const char*es=XML_ErrorString(XML_GetErrorCode(parser));
            LOGE("%s at line %ld",es, XML_GetCurrentLineNumber(parser));
            XML_ParserFree(parser);
            return;
        }
    } while(len!=0);
    XML_ParserFree(parser);
    updates = 0; 
}

void Preferences::save(const std::string&fname){
    std::ofstream ofs(fname);
    save(ofs);
}

void Preferences::save(std::ostream&os){
    os<<"<sections>"<<std::endl;
    for(auto sec:mPrefs){
	    os<<"<section name=\""<<sec.first<<"\">"<<std::endl;
	    for(auto kv:sec.second){
	        os<<"<item name=\""<<kv.first<<"\">"<<kv.second<<"</item>"<<std::endl;
	    }
	    os<<"</section>"<<std::endl;
    }
    os<<"</sections>"<<std::endl;
    updates=0;
}

int Preferences::getSectionCount()const{
    return mPrefs.size();
}

int Preferences::getSections(std::vector<std::string>&mbs){
    mbs.clear();
    for(auto s:mPrefs)
	mbs.push_back(s.first);
    return mbs.size();
}

void Preferences::removeSection(const std::string&section){
    auto sec = mPrefs.find(section);
    if(sec!=mPrefs.end()){
        mPrefs.erase(sec);
        updates++;
    }
}

bool Preferences::hasSection(const std::string&section)const{
    return mPrefs.find(section) != mPrefs.end();
}

int Preferences::getUpdates()const{
    return updates;
}

bool Preferences::getBool(const std::string&section,const std::string&key,bool def){
    auto sec=mPrefs.find(section);
    if(sec==mPrefs.end())return def;
    auto kv=sec->second.find(key);
    if(kv==sec->second.end())return def;
    std::string s=kv->second;
    LOGV("%s:%s=%s",section.c_str(),key.c_str(),s.c_str());
    return (s[0]=='t')||(s[0]=='T');
}

int Preferences::getInt(const std::string&section,const std::string&key,int def){
    auto sec=mPrefs.find(section);
    if(sec==mPrefs.end())return def;
    auto kv=sec->second.find(key);
    if(kv==sec->second.end())return def;
    std::stringstream ss(kv->second);
    ss>>def;
    return def;
}

static std::vector<std::string> split(const std::string & path) {
    std::vector<std::string> vec;
    size_t begin;
    begin = path.find_first_not_of("|");
    while (begin != std::string::npos) {
        size_t end = path.find_first_of("|", begin);
        vec.push_back(path.substr(begin, end-begin));
        begin = path.find_first_not_of("|", end);
    }
    return vec;
}

int Preferences::getInt(const std::string&section,const std::string&key,const std::map<std::string,int>&kvs,int def){
    const std::string vstr = getString(section,key,"");
    if( vstr.size() && (vstr.find('|') != std::string::npos) ){
        std::vector<std::string> gs = split(vstr);
        int result= 0;
        int count = 0;
        for(std::string s:gs){
            auto it = kvs.find(s);
            if(it != kvs.end()){
                result |= it->second;
                count++;
            }
        }
        return count ? result : def;
    }else{
        auto it = kvs.find(vstr);
        return it == kvs.end() ? def : it->second;
    }    
}

float Preferences::getFloat(const std::string&section,const std::string&key,float def){
    auto sec=mPrefs.find(section);
    if(sec==mPrefs.end())return def;
    auto kv=sec->second.find(key);
    if(kv==sec->second.end())return def;
    std::stringstream ss(kv->second);
    ss>>def;
    return def;
}

double Preferences::getDouble(const std::string&section,const std::string&key,double def){
    auto sec=mPrefs.find(section);
    if(sec==mPrefs.end())return def;
    auto kv=sec->second.find(key);
    if(kv==sec->second.end())return def;
    std::stringstream ss(kv->second);
    ss>>def;
    return def;
}

std::string Preferences::getString(const std::string&section,const std::string&key,const std::string&def){
    auto sec=mPrefs.find(section);
    if(sec==mPrefs.end())return def;
    auto kv=sec->second.find(key);
    if(kv==sec->second.end())return def;
    LOGV("%s:%s=%s",section.c_str(),key.c_str(),kv->second.c_str());
    return kv->second;
}

void Preferences::setValue(const std::string&section,const std::string&key,bool v){
    auto sec=mPrefs.find(section);
    if(sec==mPrefs.end())
	    sec = mPrefs.insert({section,std::map<std::string,std::string>()}).first;
    auto kv = sec->second.find(key);
    if(kv == sec->second.end())
	    sec->second.insert({key,(v?"true":"false")});
    else kv->second = (v?"true":"false");
    updates++;
}

void Preferences::setValue(const std::string&section,const std::string&key,int v){
    auto sec = mPrefs.find(section);
    if(sec == mPrefs.end())
        sec = mPrefs.insert({section,std::map<std::string,std::string>()}).first;
    auto kv = sec->second.find(key);
    if(kv == sec->second.end())
	    sec->second.insert({key,std::to_string(v)});
    else kv->second = std::to_string(v);
    LOGV("%s %s %d",section.c_str(),key.c_str(),v);
    updates++;
}

void Preferences::setValue(const std::string&section,const std::string&key,float v){
    auto sec = mPrefs.find(section);
    if(sec == mPrefs.end())
        sec = mPrefs.insert({section,std::map<std::string,std::string>()}).first;
    auto kv = sec->second.find(key);
    if(kv == sec->second.end())
	    sec->second.insert({key,std::to_string(v)});
    else kv->second = std::to_string(v);
    LOGV("%s %s %f",section.c_str(),key.c_str(),v);
    updates++;
}

void Preferences::setValue(const std::string&section,const std::string&key,const std::string&v){
    auto sec = mPrefs.find(section);
    if(sec == mPrefs.end())
	    sec = mPrefs.insert({section,std::map<std::string,std::string>()}).first;
    auto kv = sec->second.find(key);
    if(kv == sec->second.end())
	    sec->second.insert({key,v});
    else kv->second = v;
    LOGV("%s %s %s",section.c_str(),key.c_str(),v.c_str());
    updates++;
}

void Preferences::setValue(const std::string&section,const std::string&key,double v){
    auto sec = mPrefs.find(section);
    if(sec==mPrefs.end())
	    sec = mPrefs.insert({section,std::map<std::string,std::string>()}).first;
    auto kv = sec->second.find(key);
    if(kv == sec->second.end())
	    sec->second.insert({key,std::to_string(v)});
    else kv->second = std::to_string(v);
    LOGV("%s %s %f",section.c_str(),key.c_str(),v);
    updates++;
}

}//namespace
