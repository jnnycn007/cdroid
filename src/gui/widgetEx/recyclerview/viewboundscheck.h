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
#ifndef __VIEW_BOUNDS_CHECK_H__
#define __VIEW_BOUNDS_CHECK_H__
#include <view/view.h>
namespace cdroid{

class ViewBoundsCheck {
public:
    static constexpr int GT = 1 << 0;
    static constexpr int EQ = 1 << 1;
    static constexpr int LT = 1 << 2;
    static constexpr int CVS_PVS_POS = 0;
    static constexpr int FLAG_CVS_GT_PVS = GT << CVS_PVS_POS;
    static constexpr int FLAG_CVS_EQ_PVS = EQ << CVS_PVS_POS;
    static constexpr int FLAG_CVS_LT_PVS = LT << CVS_PVS_POS;
    static constexpr int CVS_PVE_POS = 4;
    static constexpr int FLAG_CVS_GT_PVE = GT << CVS_PVE_POS;
    static constexpr int FLAG_CVS_EQ_PVE = EQ << CVS_PVE_POS;
    static constexpr int FLAG_CVS_LT_PVE = LT << CVS_PVE_POS;
    static constexpr int CVE_PVS_POS = 8;
    static constexpr int FLAG_CVE_GT_PVS = GT << CVE_PVS_POS;
    static constexpr int FLAG_CVE_EQ_PVS = EQ << CVE_PVS_POS;
    static constexpr int FLAG_CVE_LT_PVS = LT << CVE_PVS_POS;
    static constexpr int CVE_PVE_POS = 12;
    static constexpr int FLAG_CVE_GT_PVE = GT << CVE_PVE_POS;
    static constexpr int FLAG_CVE_EQ_PVE = EQ << CVE_PVE_POS;
    static constexpr int FLAG_CVE_LT_PVE = LT << CVE_PVE_POS;
    static constexpr int MASK = GT | EQ | LT;

    struct Callback {
        //CallbackBase<int>getChildCount;
        //CallbackBase<View*>getParent;
        CallbackBase<View*,int>getChildAt;
        CallbackBase<int>getParentStart;
        CallbackBase<int>getParentEnd;
        CallbackBase<int,View*>getChildStart;
        CallbackBase<int,View*>getChildEnd;
    };
    class BoundFlags {
        int mBoundFlags;
        int mRvStart, mRvEnd, mChildStart, mChildEnd;
    public:
        BoundFlags();
        void setBounds(int rvStart, int rvEnd, int childStart, int childEnd);
        void addFlags(int flags);
        void resetFlags();
        int compare(int x, int y);
        bool boundsMatch();
    };

    Callback mCallback;
    BoundFlags mBoundFlags;

    ViewBoundsCheck(Callback callback);
    View* findOneViewWithinBoundFlags(int fromIndex, int toIndex,int preferredBoundFlags,int acceptableBoundFlags);
    bool isViewWithinBoundFlags(View* child,int boundsFlags);
};/*ViewBoundsCheck*/
}/*endof namespace*/
#endif /*__VIEW_BOUNDS_CHECK_H__*/

