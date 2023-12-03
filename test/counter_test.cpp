#include <gtest/gtest.h>
#include "counter.h"

class CounterTest : public ::testing::Test {

  protected:

    CounterTest() {
    }

    virtual ~CounterTest() {
    }

    virtual void SetUp() 
    {
      
    }

    virtual void TearDown() 
    {
        Counter::getInstance().clear();
    }

  };


TEST_F(CounterTest, GetNextTest)
{
    Counter& counter = Counter::getInstance();
    EXPECT_EQ(counter.getNext(), 0);
    EXPECT_EQ(counter.getNext(), 1);
    EXPECT_EQ(counter.getNext(), 2);
    counter.free(2);
    EXPECT_EQ(counter.getNext(), 2);
    EXPECT_EQ(counter.getNext(), 3);
    EXPECT_EQ(counter.getNext(), 4);
    EXPECT_EQ(counter.getNext(), 5);
    EXPECT_EQ(counter.getNext(), 6);
    EXPECT_EQ(counter.getNext(), 7);
    counter.free(1);
    counter.free(5);
    EXPECT_EQ(counter.getNext(), 1);
    EXPECT_EQ(counter.getNext(), 5);
    EXPECT_EQ(counter.getNext(), 8);
    EXPECT_TRUE(counter.setId(9));
    
}


TEST_F(CounterTest, Clear)
{
    Counter& counter = Counter::getInstance();
    for (unsigned int i=0; i< 15; ++i)
    {
        EXPECT_TRUE(counter.setId(i));
    }

}

TEST_F(CounterTest, SetId)
{
    Counter& counter = Counter::getInstance();
    EXPECT_TRUE(counter.setId(0));
    EXPECT_TRUE(counter.setId(25));
    EXPECT_TRUE(counter.setId(1));
    EXPECT_TRUE(counter.setId(2));
    EXPECT_TRUE(counter.setId(3));
    EXPECT_TRUE(counter.setId(4));
    EXPECT_EQ(counter.getNext(), 5);
    EXPECT_FALSE(counter.setId(1));
}

TEST_F(CounterTest, UpdateId)
{
    Counter& counter = Counter::getInstance();
    for (unsigned int i=0; i<7; ++i)
    {
        EXPECT_TRUE(counter.setId(i));
    }

    EXPECT_FALSE(counter.update(1,5));
    EXPECT_TRUE(counter.update(1,8));
    EXPECT_EQ(counter.getNext(), 1);
}