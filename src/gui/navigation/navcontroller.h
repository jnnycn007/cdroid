#ifndef __NAV_CONTROLLER_H__
#define __NAV_CONTROLLER_H__
#include <vector>
#include <deque>
#include <navigation/navinflater.h>
#include <navigation/navdestination.h>
#include <navigation/simplenavigatorprovider.h>

namespace cdroid{

class NavController{
public:
    typedef CallbackBase<void,NavController&controller,NavDestination&>OnNavigatedListener;
private:
    Context* mContext;
    Activity* mActivity;
    NavInflater* mInflater;
    NavGraph* mGraph;
    int mGraphId;
    Bundle mNavigatorStateToRestore;
    std::vector<int> mBackStackToRestore;
    std::deque<NavDestination*> mBackStack;
    SimpleNavigatorProvider* mNavigatorProvider;
    Navigator::OnNavigatorNavigatedListener mOnNavigatedListener;
    std::vector<OnNavigatedListener> mOnNavigatedListeners;
private:
    void onNavigatorNavigated(Navigator& navigator,/*IdRes*/int destId,
                  /*Navigator.BackStackEffect*/int backStackEffect);
    void dispatchOnNavigated(NavDestination* destination);
    void onGraphCreated();
    NavDestination* findDestination(/*@IdRes*/int destinationId);

public:
    NavController(Context* context);
    Context* getContext() const;
    NavigatorProvider* getNavigatorProvider() const;
    void addOnNavigatedListener(OnNavigatedListener listener);
    void removeOnNavigatedListener(OnNavigatedListener listener);
    bool popBackStack();
    bool popBackStack(/*@IdRes*/int destinationId, bool inclusive);
    bool navigateUp();
    void setMetadataGraph();
    NavInflater& getNavInflater();
    void setGraph(const std::string& graphResId);
    void setGraph(NavGraph* graph);
    bool onHandleDeepLink(/*Nullable*/ Intent intent);
    NavGraph* getGraph() const;
    NavDestination* getCurrentDestination();
    void navigate(/*@IdRes*/ int resId);
    void navigate(/*@IdRes*/ int resId, Bundle* args);
    void navigate(/*@IdRes*/ int resId, /*Nullable*/Bundle* args, /*Nullable*/ NavOptions* navOptions);
    void navigate(/*@NonNull*/ NavDirections& directions);
    void navigate(/*@NonNull*/ NavDirections& directions, /*Nullable*/NavOptions* navOptions);
    NavDeepLinkBuilder createDeepLink();
    Bundle* saveState();
    void restoreState(/*Nullable*/ Bundle* navState);
};

}
#endif/*__NAV_CONTROLLER_H__*/