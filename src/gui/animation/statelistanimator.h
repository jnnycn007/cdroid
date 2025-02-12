#ifndef __STATELIST_ANIMATOR_H__
#define __STATELIST_ANIMATOR_H__
#include <animation/valueanimator.h>
namespace cdroid{

class StateListAnimator{
protected:
    class Tuple;
private:
    friend class AnimatorInflater;
    class StateListAnimatorConstantState: public std::enable_shared_from_this<StateListAnimatorConstantState>,public ConstantState<StateListAnimator*>{
    protected:
        int mChangingConf;
        StateListAnimator*mAnimator;
    public:
        StateListAnimatorConstantState(StateListAnimator*animator);
        int getChangingConfigurations()override;
        StateListAnimator*newInstance()override;
    };
private:
    std::vector<Tuple*>mTuples;
    Tuple* mLastMatch;
    Animator*mRunningAnimator;
    class View*mView;
    std::shared_ptr<StateListAnimatorConstantState> mConstantState;
    ValueAnimator::AnimatorListener mAnimatorListener;
    int mChangingConfigurations;

    void initAnimatorListener();
    void clearTarget();
    void start(Tuple* match);
    void cancel();

public:
    StateListAnimator();
    StateListAnimator(const StateListAnimator&);
    ~StateListAnimator();
    void addState(const std::vector<int>&specs, Animator* animator);
    void setState(const std::vector<int>&state);
    Animator* getRunningAnimator();
    View* getTarget();
    void setTarget(View* view);
    void jumpToCurrentState();
    int getChangingConfigurations()const;
    void setChangingConfigurations(int configs);
    void appendChangingConfigurations(int configs);
    std::shared_ptr<ConstantState<StateListAnimator*>>createConstantState();
};

class StateListAnimator::Tuple{
public:
    std::vector<int>mSpecs;
    Animator* mAnimator;
    Tuple(const std::vector<int>&specs, Animator* animator);
    ~Tuple();
};

}//endof namespace
#endif//__STATELIST_ANIMATOR_H__
