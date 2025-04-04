#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <assets.h>
#include <getopt.h>
#include <fcntl.h>
#include <string.h>
#include <thread>
#include <mutex>

#include <porting/cdlog.h>
#include <porting/cdgraph.h>
#define ARGS_NOEXCEPT 1
#include <core/app.h>
#include <core/cla.h>
#include <core/args.h>
#include <core/build.h>
#include <core/atexit.h>
#include <core/inputeventsource.h>
#include <core/windowmanager.h>
#include <core/inputmethodmanager.h>

#if defined(__linux__)||defined(__unix__)
extern "C" char *__progname;
#elif defined(_WIN32)||defined(_WIN64)
extern "C" unsigned long  GetModuleFileNameA(void* hModule, char* lpFilename, unsigned long nSize);
#endif

namespace cdroid{

App*App::mInst=nullptr;

App::App(int argc,const char*argv[],const std::vector<CLA::Argument>&extoptions){
    int rotation;
    LogParseModules(argc,argv);
    mQuitFlag = false;
    mExitCode = 0;
    mInst = this;
    
    args::ArgumentParser parser(argc?argv[0]:"");
    args::Flag debug(parser, "debug", "enable debug mode", {'d', "debug"});
    args::Flag fps(parser,"fps","show fps info",{"fps"});
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::ValueFlag <int> alpha(parser,"alpha",  "UI layer global alpha[0,255]",{'a',"alpha"});
    args::ValueFlag <int> framedelay(parser,"framedelay","animation frame delay",{'f',"framedelay"});
    args::ValueFlag <int> density(parser,"density","UI Density",{'D',"density"});
    args::ValueFlag <int> rotate(parser,"rotate", "display rotate",{'R',"rotate"});
    args::ValueFlag <std::string> logo(parser,"logo" , "show logo",{'l',"logo"});
    args::ValueFlag <std::string> monkey(parser,"monkey","events playback path",{'m',"monkey"});
    args::ValueFlag <std::string> record(parser,"record","events record path",{'r',"record"});
    args::ValueFlag <std::string> datadir(parser,"data","data directory",{'D',"data"});
    
    //cla.addArguments(ARGS,sizeof(ARGS)/sizeof(CLA::Argument));
    cla.addArguments(extoptions.data(),extoptions.size());
    cla.setSwitchChars("-");
    const int rc = cla.parse(argc,argv);
    try{
        if(argc&&argv)parser.ParseCLI(argc,argv);
#ifndef ARGS_NOEXCEPT
    }catch(args::Help){
        std::cout << parser;
    }catch(args::ParseError&e){
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
#endif
    }catch(...){}

    if(help){
        std::cout<<parser<<std::endl;
        exit(EXIT_SUCCESS);
        LogSetModuleLevel(nullptr,LOG_FATAL);
        mQuitFlag = true;
    }
    Typeface::setContext(this);
    onInit();
#if defined(__linux__)||defined(__unix__)
    setName(std::string(argc?argv[0]:__progname));
#elif (defined(_WIN32)||defined(_WIN64))
    char progName[260];
    GetModuleFileNameA(nullptr,progName,sizeof(progName));
    setName(std::string(argc?argv[0]:progName));
#endif
    LOGI("\033[1;35m          ┏━┓┏┓╋╋╋┏┓┏┓");
    LOGI("\033[1;35m          ┃┏╋┛┣┳┳━╋╋┛┃");
    LOGI("\033[1;35m          ┃┗┫╋┃┏┫╋┃┃╋┃");
    LOGI("\033[1;35m          ┗━┻━┻┛┗━┻┻━┛");

    LOGI("cdroid %s on %s [%s] Build:%d Commit:%s",Build::VERSION::Release.c_str(),Build::VERSION::BASE_OS.c_str(),
            Build::VERSION::CODENAME.c_str(),Build::VERSION::BuildNumber,Build::VERSION::CommitID.c_str());
    LOGI("https://www.gitee.com/houstudio/cdroid\n");
    LOGI("App [%s] started c++=%d",mName.c_str(),__cplusplus);

    Looper::prepareMainLooper();
    GraphDevice& graph =GraphDevice::getInstance();
    if(rotate){
        const int rotation = (rotate.Get()/90)%4;
        WindowManager::getInstance().setDisplayRotation(0,rotation);
        graph.setRotation(rotation);
    }
    if(logo) graph.setLogo(logo.Get());
    graph.showFPS(fps).init();
    View::VIEW_DEBUG = debug;
    DisplayMetrics::DENSITY_DEVICE = DisplayMetrics::getDeviceDensity();
    if(alpha) setOpacity(alpha.Get());
    if(density) DisplayMetrics::DENSITY_DEVICE = density.Get();//getArgAsInt("density",DisplayMetrics::getDeviceDensity());
    if(framedelay)Choreographer::setFrameDelay(framedelay.Get());
    Typeface::loadPreinstalledSystemFontMap();
    Typeface::loadFaceFromResource(this);

    AtExit::registerCallback([this](){
        LOGD("Exit...");
        mQuitFlag = true;
    });

    InputEventSource*inputsource=&InputEventSource::getInstance();//(getArg("record",""));
    addEventHandler(inputsource);
    if(monkey){
        inputsource->playback(monkey.Get());
    }
}

App::~App(){
    WindowManager::getInstance().shutDown();
    InputMethodManager::getInstance().shutDown();
    delete Looper::getMainLooper();
    delete &GraphDevice::getInstance();
}

void App::onInit(){
    LOGD("onInit");
    GFXInit();
    mDisplayMetrics.setToDefaults();
    addResource(getDataPath()+std::string("cdroid.pak"),"cdroid");
}

const std::string App::getDataPath()const{
    std::string path=getArg("data","./");
    if(path.back()!='/')path+='/';
    return path;
}

App& App::getInstance(){
    if(mInst==nullptr)
        mInst = new App;
    return *mInst;
}

const std::string App::getArg(const std::string&key,const std::string&def)const{
    std::string value = def;
    cla.find(key,value);
    return value;
}

bool App::hasArg(const std::string&key)const{
    return cla.find(key);
}

bool App::hasSwitch(const std::string&key)const{
    return cla.findSwitch(key);
}

void App::setArg(const std::string&key,const std::string&value){
    cla.setArgument(key,value);
}

int App::getArgAsInt(const std::string&key,int def)const{
    int value = def;
    cla.find(key,value);
    return value;
}

float App::getArgAsFloat(const std::string&key,float def)const{
    float value = def;
    cla.find(key,value);
    return value;
}

double App::getArgAsDouble(const std::string&key,double def)const{
    double value = def;
    cla.find(key,value);
    return value;
}

size_t App::getParamCount()const{
    return cla.getParamCount();
}

std::string App::getParam(int idx,const std::string&def)const{
    std::string value = def;
    cla.getParam(idx,value);
    return value;
}

void App::setOpacity(unsigned char alpha){
    auto primarySurface = GraphDevice::getInstance().getPrimarySurface();
    if(primarySurface){
        GFXSurfaceSetOpacity(primarySurface,alpha);
        LOGD("alpha=%d",alpha);
    }
}

void App::addEventHandler(const EventHandler*handler){
    Looper::getMainLooper()->addEventHandler(handler);
}

void App::removeEventHandler(const EventHandler*handler){
    Looper::getMainLooper()->removeEventHandler(handler);
}

int App::exec(){
    Looper*looper = Looper::getMainLooper();
    while(!mQuitFlag)looper->pollAll(1);
    return mExitCode;
}

void App::exit(int code){
    mQuitFlag = true;
    mExitCode = code;
}

void App::setName(const std::string&appname){
    mName = appname;
    size_t pos = mName.find_last_of("/");
    if(pos!=std::string::npos)
        mName = mName.substr(pos+1);
    addResource(getDataPath()+mName+std::string(".pak"));
}

const std::string& App::getName(){
    return mName;
}

}

