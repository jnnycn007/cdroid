#include <wiget/framelayout.h>

namespace cdroid{
class GestureOverlayView:public FrameLayout {
public:
    static constexpr int GESTURE_STROKE_TYPE_SINGLE = 0;
    static constexpr int GESTURE_STROKE_TYPE_MULTIPLE = 1;

    static constexpr int ORIENTATION_HORIZONTAL = 0;
    static constexpr int ORIENTATION_VERTICAL = 1;
public:
    static interface OnGesturingListener {
        void onGesturingStarted(GestureOverlayView overlay);

        void onGesturingEnded(GestureOverlayView overlay);
    };

    interface OnGestureListener {
        void onGestureStarted(GestureOverlayView overlay, MotionEvent event);

        void onGesture(GestureOverlayView overlay, MotionEvent event);

        void onGestureEnded(GestureOverlayView overlay, MotionEvent event);

        void onGestureCancelled(GestureOverlayView overlay, MotionEvent event);
    };

    interface OnGesturePerformedListener {
        void onGesturePerformed(GestureOverlayView overlay, Gesture gesture);
    };
private:
    static constexpr int FADE_ANIMATION_RATE = 16;
    static constexpr bool GESTURE_RENDERING_ANTIALIAS = true;
    static constexpr bool DITHER_FLAG = true;

    long mFadeDuration = 150;
    long mFadeOffset = 420;
    long mFadingStart;
    bool mFadingHasStarted;
    bool mFadeEnabled = true;

    int mCurrentColor;
    int mCertainGestureColor = 0xFFFFFF00;
    int mUncertainGestureColor = 0x48FFFF00;
    float mGestureStrokeWidth = 12.0f;
    int mInvalidateExtraBorder = 10;

    int mGestureStrokeType = GESTURE_STROKE_TYPE_SINGLE;
    float mGestureStrokeLengthThreshold = 50.0f;
    float mGestureStrokeSquarenessTreshold = 0.275f;
    float mGestureStrokeAngleThreshold = 40.0f;

    int mOrientation = ORIENTATION_VERTICAL;

    final Rect mInvalidRect = new Rect();
    final Path mPath = new Path();
    bool mGestureVisible = true;

    float mX;
    float mY;

    float mCurveEndX;
    float mCurveEndY;

    float mTotalLength;
    bool mIsGesturing = false;
    bool mPreviousWasGesturing = false;
    bool mInterceptEvents = true;
    bool mIsListeningForGestures;
    bool mResetGesture;

    // current gesture
    Gesture* mCurrentGesture;
    std::vector<GesturePoint> mStrokeBuffer;

    // TODO: Make this a list of WeakReferences
    std::vector<OnGestureListener> mOnGestureListeners;
    // TODO: Make this a list of WeakReferences
    std::vector<OnGesturePerformedListener> mOnGesturePerformedListeners;
    // TODO: Make this a list of WeakReferences
    std::vector<OnGesturingListener> mOnGesturingListeners;

    bool mHandleGestureActions;

    // fading out effect
    bool mIsFadingOut = false;
    float mFadingAlpha = 1.0f;
    AccelerateDecelerateInterpolator*mInterpolator;// =new AccelerateDecelerateInterpolator();

    FadeOutRunnable mFadingOut;// = new FadeOutRunnable();
private:
    void init();
    void setCurrentColor(int color);
    void setPaintAlpha(int alpha);
    void clear(bool animated, bool fireActionPerformed, bool immediate);
    bool processEvent(MotionEvent& event);
    void touchDown(MotionEvent& event);
    Rect touchMove(MotionEvent& event);
    void touchUp(MotionEvent& event, bool cancel);
    void cancelGesture(MotionEvent& event);
    void fireOnGesturePerformed();
protected:
    void onDetachedFromWindow()override;
publc:
    GestureOverlayView(Context* context,const AttributeSet& attrs);
    const std::vector<GesturePoint>& getCurrentStroke() const;

    int getOrientation() const;
    void setOrientation(int orientation);
    void setGestureColor(int color);
    void setUncertainGestureColor(int color);
    int getUncertainGestureColor()const;
    int getGestureColor()const;
    float getGestureStrokeWidth()const;
    void setGestureStrokeWidth(float gestureStrokeWidth);
    int getGestureStrokeType()const;
    void setGestureStrokeType(int gestureStrokeType);
    float getGestureStrokeLengthThreshold()const;
    void setGestureStrokeLengthThreshold(float gestureStrokeLengthThreshold);
    float getGestureStrokeSquarenessTreshold()const;
    void setGestureStrokeSquarenessTreshold(float gestureStrokeSquarenessTreshold);
    float getGestureStrokeAngleThreshold()const;
    void setGestureStrokeAngleThreshold(float gestureStrokeAngleThreshold);
    bool isEventsInterceptionEnabled()const;
    void setEventsInterceptionEnabled(bool enabled);
    bool isFadeEnabled()const;
    void setFadeEnabled(bool fadeEnabled);
    Gesture getGesture()const;
    void setGesture(Gesture gesture);
    Path getGesturePath();
    Path getGesturePath(Path path);
    bool isGestureVisible()const;
    void setGestureVisible(bool visible);
    long getFadeOffset()const;
    void setFadeOffset(long fadeOffset);
    void addOnGestureListener(OnGestureListener listener);
    void removeOnGestureListener(OnGestureListener listener);
    void removeAllOnGestureListeners();
    void addOnGesturePerformedListener(OnGesturePerformedListener listener);
    void removeOnGesturePerformedListener(OnGesturePerformedListener listener);
    void removeAllOnGesturePerformedListeners();
    void addOnGesturingListener(OnGesturingListener listener);
    void removeOnGesturingListener(OnGesturingListener listener);
    void removeAllOnGesturingListeners();
    bool isGesturing()const;
    void draw(Canvas canvas);
    void clear(bool animated);
    void cancelClearAnimation();
    void cancelGesture();
    bool dispatchTouchEvent(MotionEvent& event)override;
};
}/*endof namespace*/
