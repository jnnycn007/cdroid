#ifndef __ASSETS_H__
#define __ASSETS_H__
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <drawables/drawable.h>

namespace cdroid{

class Assets:public Context{
private:
    int mNextAutofillViewId;
    std::string mLanguage;
    std::string mThemeName;
    AttributeSet mTheme;
    std::unordered_map<std::string,std::string>mStrings;
    std::unordered_map<std::string,int>mIDS;
    std::unordered_map<std::string,std::vector<std::string>>mArraies;
    std::unordered_map<std::string,std::weak_ptr<Drawable::ConstantState>>mDrawables;
    std::unordered_map<std::string,class ZIPArchive*>mResources;
    std::unordered_map<std::string,AttributeSet>mStyles;
    std::unordered_map<std::string,uint32_t>mColors;
    std::unordered_map<std::string,int>mDimensions;
    std::unordered_map<std::string,ColorStateList*>mStateColors;
    const std::string parseResource(const std::string&fullresid,std::string*res,std::string*ns)const;
    void parseItem(const std::string&package,const std::string&resid,const std::vector<std::string>&tag,std::vector<AttributeSet>atts,const std::string&value,void*);
    ZIPArchive*getResource(const std::string & fullresid, std::string* relativeResid,std::string*package)const;
protected:
    std::string mName;
    DisplayMetrics mDisplayMetrics;
    void loadStrings(const std::string&lan);
    int addResource(const std::string&path,const std::string&name=std::string());
    int loadKeyValues(const std::string&package,const std::string&resid,void*p);
public:
    Assets();
    Assets(const std::string&path);
    ~Assets();
    int loadStyles(const std::string&resid);
    void clearStyles();
    const std::string getPackageName()const override;
    const std::string getTheme()const override;
    void setTheme(const std::string&theme)override;
    const DisplayMetrics&getDisplayMetrics()const override;
    int getId(const std::string&)const override;
    int getNextAutofillId()override;
    const std::string getString(const std::string&id,const std::string&lan="")override;
    Cairo::RefPtr<Cairo::ImageSurface> loadImage(std::istream&,int width,int height)override;
    Cairo::RefPtr<Cairo::ImageSurface> loadImage(const std::string&resname,int width,int height)override;
    std::vector<std::string> getStringArray(const std::string&resname,const std::string&arrayname)const;
    std::unique_ptr<std::istream> getInputStream(const std::string&resname,std::string*outpkg=nullptr)override;
    Drawable * getDrawable(const std::string&resid)override;
    bool getBoolean(const std::string&resid)override;
    int getColor(const std::string&resid)override;
    int getDimension(const std::string&resid)override;
    float getFloat(const std::string&resid)override;
    size_t getArray(const std::string&resid,std::vector<int>&)override;
    size_t getArray(const std::string&resid,std::vector<std::string>&)override;
    ColorStateList* getColorStateList(const std::string&resid)override;
    AttributeSet obtainStyledAttributes(const std::string&)override;
};

}//namespace
#endif
