/*********************************************************************************
 * Copyright (C) [2019] [houzh@msn.com]
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *********************************************************************************/
#ifndef __LAYOUT_INFLATE_H__
#define __LAYOUT_INFLATE_H__
#include <core/attributeset.h>
#include <core/context.h>
#include <core/xmlpullparser.h>

namespace cdroid{
class View;
class ViewGroup;
class MenuInflater;
class LayoutInflater{
public:
    typedef std::function<View*(Context*ctx, const AttributeSet&attrs)>ViewInflater;
    typedef std::function<bool(const std::string&)>Filter;
    typedef std::function<View*(const std::string&,Context*,AttributeSet&)>Factory;
    typedef std::function<View*(View*,const std::string&,Context*,AttributeSet&)>Factory2;
private:
    static constexpr const char* TAG_MERGE = "merge";
    static constexpr const char* TAG_INCLUDE = "include";
    static constexpr const char* TAG_1995 = "blink";
    static constexpr const char* TAG_REQUEST_FOCUS = "requestFocus";
    static constexpr const char* TAG_TAG = "tag";
    static constexpr const char* ATTR_LAYOUT = "layout";

    Context*mContext;
    Factory mFactory;
    Factory2 mFactory2;
    Factory2 mPrivateFactory;
    Filter mFilter;
    LayoutInflater(Context*ctx);
    static LayoutInflater*mInst;
    typedef std::unordered_map<std::string,ViewInflater>INFLATERMAPPER;
    typedef std::unordered_map<std::string,std::string>STYLEMAPPER;
private:
    static INFLATERMAPPER& getInflaterMap();
    static STYLEMAPPER& getStyleMap();

    static void consumeChildElements(XmlPullParser& parser);
    void parseViewTag(XmlPullParser&parser, View*parent,const AttributeSet& attrs);
    void parseInclude(XmlPullParser&parser, Context*,View*prent,const AttributeSet& attrs);
protected:
    friend MenuInflater;
    View* createView(const std::string& name, const std::string& prefix,const AttributeSet& attrs);
    View* createViewFromTag(View* parent,const std::string& name, Context* context,AttributeSet& attrs,bool ignoreThemeAttr);
    void rInflateChildren(XmlPullParser& parser, View* parent,AttributeSet& attrs,bool finishInflate);
    void rInflate(XmlPullParser& parser, View* parent, Context* context,AttributeSet& attrs, bool finishInflate);
public:
    static LayoutInflater*from(Context*context);
    static ViewInflater getInflater(const std::string&);
    static bool registerInflater(const std::string&name,const std::string&,ViewInflater fun);
    const std::string getDefaultStyle(const std::string&name)const;
    Context*getContext()const;
    Factory getFactory()const;
    Factory2 getFactory2()const;
    void setFactory(Factory factory);
    void setFactory2(Factory2 factory);
    void setPrivateFactory(Factory2 factory);
    Filter getFilter()const;
    void setFilter(Filter f);
    [[deprecated("This function is deprecated")]]
    View* inflate(const std::string&package,std::istream&stream,ViewGroup*root,bool attachToRoot,AttributeSet*);
    View* inflate(XmlPullParser& parser,ViewGroup* root);
    /**
      * Inflate a new view hierarchy from the specified xml resource. Throws
      * {@link InflateException} if there is an error.
      *
      * @param resource ID for an XML layout resource to load (e.g.,
      *        <code>R.layout.main_page</code>)
      * @param root Optional view to be the parent of the generated hierarchy (if
      *        <em>attachToRoot</em> is true), or else simply an object that
      *        provides a set of LayoutParams values for root of the returned
      *        hierarchy (if <em>attachToRoot</em> is false.)
      * @param attachToRoot Whether the inflated hierarchy should be attached to
      *        the root parameter? If false, root is only used to create the
      *        correct subclass of LayoutParams for the root view in the XML.
      * @return In Android The root View of the inflated hierarchy. If root was supplied and
      *         attachToRoot is true, this is root; otherwise it is the root of the inflated XML file.
      *         In cdroid ,we allways return  the root of the inflated XML file.
      */
    View* inflate(XmlPullParser& parser,ViewGroup* root, bool attachToRoot);
    View* inflate(const std::string&resource,ViewGroup* root);
    View* inflate(const std::string&resource,ViewGroup* root, bool attachToRoot);
};

template<typename T>
class InflaterRegister{
public:
    InflaterRegister(const std::string&name,const std::string&defstyle){
        LayoutInflater::registerInflater(name,defstyle,[](Context*ctx,const AttributeSet&attr)->View*{return new T(ctx,attr);});
    }
};

#define DECLARE_WIDGET(T) static InflaterRegister<T> widget_inflater_##T(#T,"");
#define DECLARE_WIDGET2(T,style) static InflaterRegister<T> widget_inflater_##T(#T,style);
#define DECLARE_WIDGET3(T,name,style) static InflaterRegister<T> widget_inflater_##name(#name,style);
}//endof namespace
#endif
