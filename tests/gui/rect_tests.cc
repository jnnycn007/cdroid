#include <gtest/gtest.h>
#include <cdroid.h>

using namespace cdroid;

class RECTTEST:public testing::Test{

   public :
   virtual void SetUp(){
   }
   virtual void TearDown(){
   }
};


TEST_F(RECTTEST,intersect){
    cdroid::Rect rc1={0,0,100,100};
    cdroid::Rect rc2={-40,-50,100,100};
    rc2.intersect(rc1);
    printf("%d,%d %d,%d\r\n",rc2.left,rc2.top,rc2.width,rc2.height);
    ASSERT_EQ(rc2.left,0);
    ASSERT_EQ(rc2.top,0);
    ASSERT_EQ(rc2.width,60);
    ASSERT_EQ(rc2.height,50);
     
}
TEST_F(RECTTEST,intersect2){
    cdroid::Rect rc1={0,0,100,100};
    cdroid::Rect rc2={-40,-40,100,100};
    rc1.intersect(rc2);
    printf("%d,%d %d,%d\r\n",rc1.left,rc1.top,rc1.width,rc1.height);
}

TEST_F(RECTTEST,Union){
    cdroid::Rect rc1={-83,-83,566,566};
    cdroid::Rect rc2={0,0,800,600};
    rc1.Union(rc2);
    printf("%d,%d %d,%d\r\n",rc1.left,rc1.top,rc1.width,rc1.height);
}
