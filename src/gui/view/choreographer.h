#ifndef __CHOREO_GRAPHER_H__
#define __CHOREO_GRAPHER_H__
#include <core/looper.h>
#include <core/neverdestroyed.h>
#include <drawables/drawable.h>
namespace cdroid{
class Choreographer:protected EventHandler{
public:
    enum CallbackType{
        CALLBACK_INPUT = 0,
        CALLBACK_ANIMATION=1,
        CALLBACK_TRAVERSAL=2,
        CALLBACK_COMMIT= 3,
        CALLBACK_LAST  = CALLBACK_COMMIT
    };
    typedef CallbackBase<void,int64_t>FrameCallback;
    static constexpr long DEFAULT_FRAME_DELAY=33;
private:
    class CallbackRecord{
    public:
        CallbackRecord*next;
        int64_t dueTime;
        Runnable action;
        FrameCallback frameCallback;
        void* token;
    public:
        CallbackRecord();
        void run(int64_t frameTimeNanos);
        bool compare(void*action,void*token)const;
    };
    class CallbackQueue{
    public:
        Choreographer*mChoreographer;
        CallbackRecord*mHead;
    public:
        CallbackQueue(Choreographer*choreographer);
        bool hasDueCallbacksLocked(int64_t now)const;
        CallbackRecord* extractDueCallbacksLocked(int64_t now);
        void addCallbackLocked(int64_t dueTime, void* action,void* token);
        int removeCallbacksLocked(void* action, void* token);
        int hasCallbacksLocked(void* action, void* token)const;
    };
private:
    Looper *mLooper;
    bool mFrameScheduled;
    bool mCallbacksRunning;
    nsecs_t mLastFrameTimeNanos;
    nsecs_t mFrameIntervalNanos;
    CallbackRecord* mCallbackPool;
    CallbackQueue* mCallbackQueues[CALLBACK_LAST+1];
    static long sFrameDelay;
    friend NeverDestroyed<Choreographer>;
    Choreographer();
    ~Choreographer();
    static float getRefreshRate();
    CallbackRecord* obtainCallbackLocked(int64_t dueTime,void* action,void* token);
    void recycleCallbackLocked(CallbackRecord* callback);
    int removeCallbacksInternal(int callbackType,void* action, void* token);
    void postCallbackDelayedInternal(int callbackType,void* action, void* token, int64_t delayMillis);
    void scheduleFrameLocked(int64_t);
protected:
    int checkEvents()override;
    int handleEvents()override;
    void doFrame(nsecs_t frameTimeNanos,int frame);
    void doCallbacks(int callbackType, long frameTimeMillis);
public:
    static Choreographer& getInstance();
    static long getFrameDelay();
    static void setFrameDelay(long frameDelay);
    static int64_t subtractFrameDelay(long delayMillis);
    int64_t getFrameTimeNanos()const;
    int64_t getLastFrameTimeNanos()const;
    int64_t getFrameTime()const;
    int64_t getFrameIntervalNanos()const;
    void postCallback(int callbackType,const Runnable& action, void* token);
    void postCallbackDelayed(int callbackType,const Runnable& action,void*token,long delayMillis);
    int removeCallbacks(int callbackType, const Runnable* action,void*token);
    int removeFrameCallback(const FrameCallback& callback);
    int hasCallbacks(int callbackType, const Runnable* action,void*token)const;
    void postFrameCallbackDelayed(const FrameCallback& callback, long delayMillis);
    void postFrameCallback(const FrameCallback& callback);
};
}

#endif
