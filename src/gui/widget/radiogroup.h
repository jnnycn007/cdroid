#ifndef __RADIOGROUP_H__
#define __RADIOGROUP_H__
#include <widget/linearlayout.h>
#include <widget/radiobutton.h>
namespace cdroid{


class RadioGroup: public LinearLayout{
public:
    class LayoutParams:public LinearLayout::LayoutParams{
    public:
        LayoutParams(Context*c,const AttributeSet&);
        LayoutParams(int w,int h);
        LayoutParams(int w, int h, float initWeight);
        LayoutParams(const ViewGroup::LayoutParams& p);
        LayoutParams(const MarginLayoutParams& source);
    };
private:
    int mCheckedId;
    bool mInitialCheckedId;
    bool mProtectFromCheckedChange;
    CompoundButton::OnCheckedChangeListener mChildOnCheckedChangeListener;
    CompoundButton::OnCheckedChangeListener mOnCheckedChangeListener;
    ViewGroup::OnHierarchyChangeListener mOnHierarchyChangeListener;
    void init();
    void onChildViewAdded(ViewGroup& parent, View* child);
    void onChildViewRemoved(ViewGroup& parent, View* child);
    void setCheckedId(int id);
    void setCheckedStateForView(int viewId, bool checked);
    void onRadioChecked(CompoundButton&c,bool checked);
protected:
    bool checkLayoutParams(const ViewGroup::LayoutParams* p)const override;
    ViewGroup::LayoutParams* generateDefaultLayoutParams()const override;
    void onFinishInflate()override;
public:
    RadioGroup(int w,int h);
    RadioGroup(Context* context,const AttributeSet& attrs);
    ViewGroup::LayoutParams* generateLayoutParams(const AttributeSet& attrs)const override;
    View& addView(View* child, int index,ViewGroup::LayoutParams* params)override;
    int getCheckedRadioButtonId()const;
    std::string getAccessibilityClassName()const override;
    void setOnHierarchyChangeListener(const ViewGroup::OnHierarchyChangeListener& listener)override;
    void setOnCheckedChangeListener(CompoundButton::OnCheckedChangeListener listener);
    void check(int id);
    void clearCheck();
};
}//namepace

#endif
