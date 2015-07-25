#include "priority_queue.h"
#include <gtest/gtest.h>
#include <iostream>

struct Value
{
  bool operator >(const Value& lval) const
  {
    return b > lval.b;
  }

  bool operator <(const Value& lval) const
  {
    return b < lval.b;
  }

  bool operator <=(const Value& lval) const
  {
    return !(*this > lval);
  }

  bool operator ==(const Value& lval) const
  {
    return b == lval.b;
  }

  void operator =(const Value& lval)
  {
    a = lval.a;
    b = lval.b;
  }
  int a;
  int b;
};

TEST(PriorityQueue, InitAndPush)
{
  PriorityQueue<Value> pq;
  ASSERT_TRUE(pq.Init("pq_test"));
  for(int i = 1; i < 10; ++i)
  {
    Value v;
    v.a = rand()%10;
    v.b = rand()%10;
    pq.Push(v);
  }
}

TEST(PriorityQueue, LoadAndPop)
{
  PriorityQueue<Value> pq;
  ASSERT_TRUE(pq.Init("pq_test"));
  for(int i = 1; i < 10; ++i)
  {
    Value v;
    v.a = rand()%10;
    v.b = rand()%10;
    pq.Push(v);
  }
  Value preVal;
  preVal.a = -1;
  preVal.b = -1;
  while(!pq.Empty())
  {
    EXPECT_TRUE(preVal <= pq.Top());
    preVal = pq.Top();
    pq.Pop();
  }
}
