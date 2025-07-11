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
#ifndef __CALENDAR_VIEW_H__
#define __CALENDAR_VIEW_H__
#include <widget/framelayout.h>

namespace cdroid{

class CalendarView:public FrameLayout{
public:
    class CalendarViewDelegate;
    class AbstractCalendarViewDelegate;
    DECLARE_UIEVENT(void,OnDateChangeListener,CalendarView& view, int year, int month, int dayOfMonth);
private:
public:
    CalendarView(int w,int h);
    CalendarView(Context*,const AttributeSet&atts);
    void setWeekDayTextAppearance(const std::string&resid);
    const std::string getWeekDayTextAppearance()const;
    void setDateTextAppearance(const std::string&resid);
    const std::string getDateTextAppearance()const;
    void setMinDate(int64_t minDate);/*date from 1970.1.1*/
    int64_t getMinDate()const;
    void setMaxDate(int64_t maxDate);
    int64_t getMaxDate()const;
    void setFirstDayOfWeek(int firstDayOfWeek);
    int getFirstDayOfWeek()const;
    void setOnDateChangeListener(OnDateChangeListener listener);
    void setDate(long date);
    /*get selected Date*/
    int64_t getDate();
    bool getBoundsForDate(int64_t date,Rect& outBounds);
};

class CalendarView::CalendarViewDelegate{
public:
    virtual void setShownWeekCount(int count)=0;
    virtual int getShownWeekCount()=0;

    virtual void setSelectedWeekBackgroundColor(int color)=0;
    virtual int getSelectedWeekBackgroundColor()=0;

    virtual void setFocusedMonthDateColor(int color)=0;
    virtual int getFocusedMonthDateColor()=0;

    virtual void setUnfocusedMonthDateColor(int color)=0;
    virtual int getUnfocusedMonthDateColor()=0;

    virtual void setWeekNumberColor(int color)=0;
    virtual int getWeekNumberColor()=0;

    virtual void setWeekSeparatorLineColor(int color)=0;
    virtual int getWeekSeparatorLineColor()=0;

    virtual void setSelectedDateVerticalBar(const std::string& resourceId)=0;
    virtual void setSelectedDateVerticalBar(Drawable* drawable)=0;
    virtual Drawable* getSelectedDateVerticalBar()=0;

    virtual void setWeekDayTextAppearance(const std::string& resourceId)=0;
    virtual const std::string getWeekDayTextAppearance()=0;

    virtual void setDateTextAppearance(const std::string&resourceId)=0;
    virtual const std::string getDateTextAppearance()=0;

    virtual void setMinDate(int64_t minDate)=0;
    virtual int64_t getMinDate()=0;

    virtual void setMaxDate(int64_t maxDate)=0;
    virtual int64_t getMaxDate()=0;

    virtual void setShowWeekNumber(bool showWeekNumber)=0;
    virtual bool getShowWeekNumber()=0;

    virtual void setFirstDayOfWeek(int firstDayOfWeek)=0;
    virtual int getFirstDayOfWeek()=0;

    virtual void setDate(int64_t date)=0;
    virtual void setDate(int64_t date, bool animate, bool center)=0;
    virtual long getDate()=0;

    virtual bool getBoundsForDate(int64_t date, Rect& outBounds)=0;

    virtual void setOnDateChangeListener(const OnDateChangeListener& listener);

    virtual void onConfigurationChanged(int newConfig)=0;
};

class CalendarView::AbstractCalendarViewDelegate:public CalendarViewDelegate {
protected:
    /** The default minimal date. */
    //static constexpr std::string DEFAULT_MIN_DATE = "01/01/1900";

    /** The default maximal date. */
    //static constexpr std::string DEFAULT_MAX_DATE = "01/01/2100";

    CalendarView* mDelegator;
    Context* mContext;
    //Locale mCurrentLocale;
public:
    AbstractCalendarViewDelegate(CalendarView* delegator, Context* context) {
        mDelegator = delegator;
        mContext = context;

        // Initialization based on locale
        //setCurrentLocale(Locale.getDefault());
    }

    /*void setCurrentLocale(Locale locale) {
        if (locale.equals(mCurrentLocale)) {
            return;
        }
        mCurrentLocale = locale;
    }*/

    void setShownWeekCount(int count) override{
        // Deprecated.
    }

   int getShownWeekCount() override{
        // Deprecated.
        return 0;
    }

    void setSelectedWeekBackgroundColor(int color) override{
        // Deprecated.
    }

   int getSelectedWeekBackgroundColor() override{
        return 0;
    }

    void setFocusedMonthDateColor(int color) override{
        // Deprecated.
    }

    int getFocusedMonthDateColor() override{
        return 0;
    }

    void setUnfocusedMonthDateColor(int color) override{
        // Deprecated.
    }

    int getUnfocusedMonthDateColor() override{
        return 0;
    }

    void setWeekNumberColor(int color) override{
        // Deprecated.
    }

    int getWeekNumberColor() override{
        // Deprecated.
        return 0;
    }

    void setWeekSeparatorLineColor( int color) {
        // Deprecated.
    }

    int getWeekSeparatorLineColor() override{
        // Deprecated.
        return 0;
    }

    void setSelectedDateVerticalBar(const std::string& resId) override{
        // Deprecated.
    }

    void setSelectedDateVerticalBar(Drawable* drawable) override{
        // Deprecated.
    }

    Drawable* getSelectedDateVerticalBar() {
        // Deprecated.
        return nullptr;
    }

    void setShowWeekNumber(bool showWeekNumber) override{
        // Deprecated.
    }

    bool getShowWeekNumber() override{
        // Deprecated.
        return false;
    }

    void onConfigurationChanged(int newConfig) override{
        // Nothing to do here, configuration changes are already propagated
        // by ViewGroup.
    }
};

}//namespace
#endif
