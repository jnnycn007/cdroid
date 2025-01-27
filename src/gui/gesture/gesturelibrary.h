#ifndef __GESTURE_LIBRARY_H__
#define __GESTURE_LIBRARY_H__
#include <string>
#include <vector>
#include <gesture/learner.h>
#include <gesture/gesture.h>
#include <gesture/prediction.h>
#include <gesture/gesturestore.h>
namespace cdroid{
class GestureLibrary {
protected:
    GestureStore* mStore;
    GestureLibrary() {
        mStore = new GestureStore();
    }
public:
    virtual bool save()=0;
    virtual bool load()=0;
    virtual ~GestureLibrary(){
        delete mStore;
    }
    virtual bool isReadOnly()const {
        return false;
    }

    /** @hide */
    Learner& getLearner() {
        return mStore->getLearner();
    }

    void setOrientationStyle(int style) {
        mStore->setOrientationStyle(style);
    }

    int getOrientationStyle() {
        return mStore->getOrientationStyle();
    }

    void setSequenceType(int type) {
        mStore->setSequenceType(type);
    }

    int getSequenceType() {
        return mStore->getSequenceType();
    }

    std::vector<std::string> getGestureEntries() {
        return mStore->getGestureEntries();
    }

    std::vector<Prediction> recognize(const Gesture& gesture) {
        return mStore->recognize(gesture);
    }

    void addGesture(const std::string& entryName, Gesture* gesture) {
        mStore->addGesture(entryName, gesture);
    }

    void removeGesture(const std::string& entryName, Gesture* gesture) {
        mStore->removeGesture(entryName, gesture);
    }

    void removeEntry(const std::string& entryName) {
        mStore->removeEntry(entryName);
    }

    std::vector<Gesture*> getGestures(const std::string& entryName) {
        return mStore->getGestures(entryName);
    }
};
}/*endof namespace*/
#endif/*__GESTURE_LIBRARY_H__*/
