#ifndef __LINEAR_SMOOTH_SCROLLER_H__
#define __LINEAR_SMOOTH_SCROLLER_H__
#include <widgetEx/recyclerview/recyclerview.h>
namespace cdroid{
class LinearSmoothScroller:public RecyclerView::SmoothScroller {
private:
    static constexpr float MILLISECONDS_PER_INCH = 25.f;
    static constexpr int TARGET_SEEK_SCROLL_DISTANCE_PX = 10000;
    static constexpr float TARGET_SEEK_EXTRA_SCROLL_RATIO = 1.2f;
    float MILLISECONDS_PER_PX;
public:
    static constexpr int SNAP_TO_START = -1;
    static constexpr int SNAP_TO_END = 1;
    static constexpr int SNAP_TO_ANY = 0;
protected:
    LinearInterpolator* mLinearInterpolator;// = new LinearInterpolator();
    DecelerateInterpolator* mDecelerateInterpolator;// = new DecelerateInterpolator();
    PointF mTargetVector;
    int mInterimTargetDx = 0, mInterimTargetDy = 0;
    bool mTargetVectorUsable;
public:
    LinearSmoothScroller(Context* context);
    void onStart();
    void onTargetFound(View* targetView, RecyclerView::State& state, Action& action);
    void onSeekTargetStep(int dx, int dy, RecyclerView::State& state, Action& action);
    void onStop();
    float calculateSpeedPerPixel(DisplayMetrics& displayMetrics);
    int calculateTimeForDeceleration(int dx);
    int calculateTimeForScrolling(int dx);
    int getHorizontalSnapPreference();
    int getVerticalSnapPreference();
    void updateActionForInterimTarget(Action action);
    int clampApplyScroll(int tmpDt, int dt);
    int calculateDtToFit(int viewStart, int viewEnd, int boxStart, int boxEnd, int
        snapPreference);

    int calculateDyToMakeVisible(View* view, int snapPreference);
    int calculateDxToMakeVisible(View* view, int snapPreference);
};
}/*endof namespace*/
#endif/*__LINEAR_SMOOTH_SCROLLER_H__*/