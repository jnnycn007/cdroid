#ifndef __NINEPATCH_DRAWABLE_H__
#define __NINEPATCH_DRAWABLE_H__
#include <drawables/drawable.h>
#include <drawables/ninepatch.h>

namespace cdroid{
class NinePatch;
class NinePatchDrawable:public Drawable{
private:
    class NinePatchState:public std::enable_shared_from_this<NinePatchState>,public ConstantState{
    public:
        float mBaseAlpha;//= 1.0f;
        bool mDither;//=DEFAULT_DITHER;
        bool mAutoMirrored;//= false;
        Rect mPadding;
        Insets mOpticalInsets;
        int mTintMode;
        int mChangingConfigurations;
        const ColorStateList*mTint;
        Cairo::RefPtr<NinePatch>mNinePatch;
        NinePatchState();
        NinePatchState(const NinePatchState&state);
        void setBitmap(Cairo::RefPtr<Cairo::ImageSurface>bitmap,const Rect*padding=nullptr);
        void setBitmap(Context*ctx,const std::string&resid,const Rect*padding=nullptr);
        NinePatchDrawable*newDrawable()override;
        void draw(Canvas&canvas,const Rect&rect,int alpha);
        int getChangingConfigurations()const override;
    };
    int mAlpha;
    int mBitmapWidth;
    int mBitmapHeight;
    int mTargetDensity;
    float mOutlineRadius;
    Insets mOpticalInsets;
    Rect mPadding;
    bool mMutated;
    bool mFilterBitmap;
    bool needsMirroring();
    void computeBitmapSize();
    std::shared_ptr<NinePatchState>mNinePatchState;
    PorterDuffColorFilter*mTintFilter;
    NinePatchDrawable(std::shared_ptr<NinePatchState>state);
    void updateStateFromTypedArray(const AttributeSet&a);
protected:
    bool onStateChange(const std::vector<int>& stateSet)override;
public:
    NinePatchDrawable();
    NinePatchDrawable(Context*,const std::string&resid);
    NinePatchDrawable(Cairo::RefPtr<Cairo::ImageSurface>bmp);
    ~NinePatchDrawable();
    void setTargetDensity(int density);
    Insets getOpticalInsets()override;
    void setAlpha(int alpha)override;
    bool getPadding(Rect& padding) override;
    void getOutline(Outline&)override;
    int getAlpha()const override;
    void setTintList(const ColorStateList* tint)override;
    void setTintMode(int mode)override;
    void setAutoMirrored(bool mirrored)override;
    bool isAutoMirrored()const override;
    void setFilterBitmap(bool filter)override;
    bool isFilterBitmap()const override;
    int getIntrinsicWidth() override;
    int getIntrinsicHeight() override;
    NinePatchDrawable*mutate()override;
    bool isStateful()const override;
    bool hasFocusStateSpecified()const override;
    std::shared_ptr<ConstantState>getConstantState()override;
    void draw(Canvas&canvas)override;
    void inflate(XmlPullParser&parser,const AttributeSet&atts)override;
};
}
#endif
