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
#include <widgetEx/recyclerview/recyclerview.h>
#include <widgetEx/recyclerview/childhelper.h>

namespace cdroid{
ChildHelper::ChildHelper(ChildHelper::Callback& callback){
    mCallback = callback;
    mBucket = new Bucket();
}

ChildHelper::~ChildHelper(){
    delete mBucket;
}

void ChildHelper::hideViewInternal(View* child){
    mHiddenViews.push_back(child);
    mCallback.onEnteredHiddenState(child);
}

bool ChildHelper::unhideViewInternal(View* child){
   auto it=std::find(mHiddenViews.begin(),mHiddenViews.end(),child);
   if (it!=mHiddenViews.end()){//HiddenViews.remove(child)) {
       mHiddenViews.erase(it);
       mCallback.onLeftHiddenState(child);
       return true;
   }
   return false;
}

void ChildHelper::addView(View* child, bool hidden){
    addView(child,-1,hidden);
}

void ChildHelper::addView(View* child, int index, bool hidden){
    int offset;
    if (index < 0) {
        offset = mCallback.getChildCount();
    } else {
        offset = getOffset(index);
    }
    mBucket->insert(offset, hidden);
    if (hidden) {
        hideViewInternal(child);
    }
    mCallback.addView(child, offset);
    LOGD_IF(_Debug,"addViewAt %d,h:%d",index,hidden);
}

int ChildHelper::getOffset(int index){
    if (index < 0) {
        return -1; //anything below 0 won't work as diff will be undefined.
    }
    const int limit = mCallback.getChildCount();
    int offset = index;
    while (offset < limit) {
        const int removedBefore = mBucket->countOnesBefore(offset);
        const int diff = index - (offset - removedBefore);
        if (diff == 0) {
            while (mBucket->get(offset)) { // ensure this offset is not hidden
                offset++;
            }
            return offset;
        } else {
            offset += diff;
        }
    }
    return -1;
}

void ChildHelper::removeView(View* view){
    if (mRemoveStatus == REMOVE_STATUS_IN_REMOVE) {
        throw std::logic_error("Cannot call removeView(At) within removeView(At)");
    } else if (mRemoveStatus == REMOVE_STATUS_IN_REMOVE_IF_HIDDEN) {
        throw std::logic_error("Cannot call removeView(At) within removeViewIfHidden");
    }
    mRemoveStatus = REMOVE_STATUS_IN_REMOVE;
    mViewInRemoveView = view;
    const int index = mCallback.indexOfChild(view);
    if (index < 0) {
        return;
    }
    if (mBucket->remove(index)) {
        unhideViewInternal(view);
    }
    mCallback.removeViewAt(index);
    LOGD_IF(_Debug,"remove View off:%d",index);

    mRemoveStatus = REMOVE_STATUS_NONE;
    mViewInRemoveView = nullptr;
}

void ChildHelper::removeViewAt(int index){
    if (mRemoveStatus == REMOVE_STATUS_IN_REMOVE) {
        throw std::logic_error("Cannot call removeView(At) within removeView(At)");
    } else if (mRemoveStatus == REMOVE_STATUS_IN_REMOVE_IF_HIDDEN) {
        throw std::logic_error("Cannot call removeView(At) within removeViewIfHidden");
    }
    const int offset = getOffset(index);
    View* view = mCallback.getChildAt(offset);
    if (view == nullptr) {
        return;
    }
    mRemoveStatus = REMOVE_STATUS_IN_REMOVE;
    mViewInRemoveView = view;
    if (mBucket->remove(offset)) {
        unhideViewInternal(view);
    }
    mCallback.removeViewAt(offset);
    LOGD_IF(_Debug,"removeViewAt %d off:%d",index,offset);

    mRemoveStatus = REMOVE_STATUS_NONE;
    mViewInRemoveView = nullptr;
}

View* ChildHelper::getChildAt(int index){
    const int offset = getOffset(index);
    return mCallback.getChildAt(offset);
}

void ChildHelper::removeAllViewsUnfiltered(){
    mBucket->reset();
    for (int i = int(mHiddenViews.size() - 1); i >= 0; i--) {
        mCallback.onLeftHiddenState(mHiddenViews.at(i));
        //mHiddenViews.remove(i);
    }
    mHiddenViews.clear();
    mCallback.removeAllViews();
    LOGD_IF(_Debug,"removeAllViewsUnfiltered");
}

View* ChildHelper::findHiddenNonRemovedView(int position){
    const int count = (int)mHiddenViews.size();
    for (int i = 0; i < count; i++) {
        View* view = mHiddenViews.at(i);
        RecyclerView::ViewHolder* holder = mCallback.getChildViewHolder(view);
        if (holder->getLayoutPosition() == position
                && !holder->isInvalid()
                && !holder->isRemoved()) {
            return view;
        }
    }
    return nullptr;
}

void ChildHelper::attachViewToParent(View* child, int index, ViewGroup::LayoutParams* layoutParams,
            bool hidden){
    int offset;
    if (index < 0) {
        offset = mCallback.getChildCount();
    } else {
        offset = getOffset(index);
    }
    mBucket->insert(offset, hidden);
    if (hidden) {
        hideViewInternal(child);
    }
    mCallback.attachViewToParent(child, offset, layoutParams);
    LOGD_IF(_Debug,"attach view to parent index:%d off:%d h:%d ",index,offset,hidden);
}

int ChildHelper::getChildCount()const{
    return mCallback.getChildCount() - mHiddenViews.size();
}

int ChildHelper::getUnfilteredChildCount(){
    return mCallback.getChildCount();
}

View* ChildHelper::getUnfilteredChildAt(int index){
    return mCallback.getChildAt(index);
}

void ChildHelper::detachViewFromParent(int index){
    const int offset = getOffset(index);
    mBucket->remove(offset);
    mCallback.detachViewFromParent(offset);
    LOGD_IF(_Debug,"detach view from parent %d of:%d",index,offset);
}

int ChildHelper::indexOfChild(View* child){
    const int index = mCallback.indexOfChild(child);
    if (index == -1) {
        return -1;
    }
    if (mBucket->get(index)) {
        if (_Debug) {
            FATAL("cannot get index of a hidden child");
        } else {
            return -1;
        }
    }
    // reverse the index
    return index - mBucket->countOnesBefore(index);
}

bool ChildHelper::isHidden(View* view){
    //return mHiddenViews.contains(view);
    return mHiddenViews.end()!=std::find(mHiddenViews.begin(),mHiddenViews.end(),view);
}

void ChildHelper::hide(View* view){
    const int offset = mCallback.indexOfChild(view);
    if (offset < 0) {
        FATAL("view is not a child, cannot hide %p",view);
    }
    if (_Debug && mBucket->get(offset)) {
        FATAL("trying to hide same view twice, how come ?%p ",view);
    }
    mBucket->set(offset);
    hideViewInternal(view);
    LOGD_IF(_Debug,"hiding child %p at offset",view,offset);
}

void ChildHelper::unhide(View* view){
    const int offset = mCallback.indexOfChild(view);
    if (offset < 0) {
        FATAL("view is not a child, cannot hide %p",view);
    }
    if (!mBucket->get(offset)) {
        FATAL("trying to unhide a view that was not hidden",view);
    }
    mBucket->clear(offset);
    unhideViewInternal(view);
}

bool ChildHelper::removeViewIfHidden(View* view){
    if (mRemoveStatus == REMOVE_STATUS_IN_REMOVE) {
        if (mViewInRemoveView != view) {
            throw std::logic_error("Cannot call removeViewIfHidden within removeView (At) for a different view");
        }
        // removeView ends the ItemAnimation and triggers removeViewIfHidden
        return false;
    } else if (mRemoveStatus == REMOVE_STATUS_IN_REMOVE_IF_HIDDEN) {
        throw std::logic_error("Cannot call removeViewIfHidden within removeViewIfHidden");
    }
    mRemoveStatus = REMOVE_STATUS_IN_REMOVE_IF_HIDDEN;
    const int index = mCallback.indexOfChild(view);
    if (index == -1) {
        LOGE_IF(unhideViewInternal(view),"view is in hidden list but not in view group");
        mRemoveStatus = REMOVE_STATUS_NONE;
        return true;
    }
    if (mBucket->get(index)) {
        mBucket->remove(index);
        LOGE_IF(!unhideViewInternal(view),"removed a hidden view but it is not in hidden views list");
        mCallback.removeViewAt(index);
        mRemoveStatus = REMOVE_STATUS_NONE;
        return true;
    }
    mRemoveStatus = REMOVE_STATUS_NONE;
    return false;
}

static unsigned long RotateRight(unsigned long num, int distance) {
    if (distance < 0) {
        distance = -distance;
        return (num << distance) | (num >> (64 - distance));
    }
    return (num >> distance) | (num << (64 - distance));
}

static int BitCount(long num) {
    int count = 0;
    while (num != 0) {
        if (num & 1) {
            count++;
        }
        num >>= 1;
    }
    return count;
}

ChildHelper::Bucket::Bucket(){
    mData =0;
    mNext = nullptr;
}

void ChildHelper::Bucket::ensureNext(){
    if (mNext == nullptr) {
        mNext = new Bucket();
    }
}

void ChildHelper::Bucket::set(int index){
    if (index >= BITS_PER_WORD) {
        ensureNext();
        mNext->set(index - BITS_PER_WORD);
    } else {
        mData |= 1L << index;
    }
}

void ChildHelper::Bucket::clear(int index){
    if (index >= BITS_PER_WORD) {
        if (mNext != nullptr)  {
            mNext->clear(index - BITS_PER_WORD);
        }
    } else {
        mData &= ~(1L << index);
    }
}

bool ChildHelper::Bucket::get(int index){
    if (index >= BITS_PER_WORD) {
        ensureNext();
        return mNext->get(index - BITS_PER_WORD);
    } else {
        return (mData & (1L << index)) != 0;
    }
}

void ChildHelper::Bucket::reset(){
    mData = 0;
    if (mNext != nullptr) {
        mNext->reset();
    }
}

void ChildHelper::Bucket::insert(int index,bool value){
    if (index >= BITS_PER_WORD) {
        ensureNext();
        mNext->insert(index - BITS_PER_WORD, value);
    } else {
        const bool lastBit = (mData & LAST_BIT) != 0;
        long mask = (1L << index) - 1;
        const long before = mData & mask;
        const long after = (mData & ~mask) << 1;
        mData = before | after;
        if (value) {
            set(index);
        } else {
            clear(index);
        }
        if (lastBit || mNext != nullptr) {
            ensureNext();
            mNext->insert(0, lastBit);
        }
    }
}

bool ChildHelper::Bucket::remove(int index){
    if (index >= BITS_PER_WORD) {
        ensureNext();
        return mNext->remove(index - BITS_PER_WORD);
    } else {
        long mask = (1L << index);
        const bool value = (mData & mask) != 0;
        mData &= ~mask;
        mask = mask - 1;
        const long before = mData & mask;
        // cannot use >> because it adds one.
        const long after = RotateRight(mData & ~mask, 1);
        mData = before | after;
        if (mNext != nullptr) {
            if (mNext->get(0)) {
                set(BITS_PER_WORD - 1);
            }
            mNext->remove(0);
        }
        return value;
    }
}

int ChildHelper::Bucket::countOnesBefore(int index){
    if (mNext == nullptr) {
        if (index >= (int)BITS_PER_WORD) {
            return BitCount(mData);
        }
        return BitCount(mData & ((1L << index) - 1));
    }
    if (index < BITS_PER_WORD) {
        return BitCount(mData & ((1L << index) - 1));
    } else {
        return mNext->countOnesBefore(index - BITS_PER_WORD) + BitCount(mData);
    }    
}

}/*endof namespace*/
