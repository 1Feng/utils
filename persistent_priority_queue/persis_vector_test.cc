#include "persis_vector.h"
#include "gtest/gtest.h"
#include <iostream>
#include <algorithm>

TEST(Vector, Init)
{
  SeriaVector<int> vec;
  ASSERT_TRUE(vec.Init("vector_test"));
}

TEST(Vector, ReloadAndTestAll)
{
  SeriaVector<int> vec;
  ASSERT_TRUE(vec.Init("vector_test"));
  ASSERT_EQ(static_cast<size_t>(0), vec.Size());
  size_t capacity = vec.Capacity() ? vec.Capacity() : 1;
  for(int i = 1; i < 10; ++i)
  {
    vec.Push_back(i);
    ASSERT_EQ(static_cast<size_t>(i), vec.Size());
    while(capacity <= static_cast<size_t>(i))
    {
      capacity *= 2;
    }
    ASSERT_EQ(capacity, vec.Capacity());
  }

  int value = 1;
  for(SeriaVector<int>::iterator it = vec.Begin(); it != vec.End(); ++it)
  {
    EXPECT_EQ(value++, *it);
  }

  for(size_t i = 0; i < vec.Size(); ++i)
  {
    EXPECT_EQ(static_cast<int>(i+1), vec[i]);
  }
  size_t size = vec.Size();
  while(!vec.Empty())
  {
    vec.Pop_back();
    EXPECT_EQ(--size, vec.Size());
  }
  ASSERT_EQ(capacity, vec.Capacity());
}

