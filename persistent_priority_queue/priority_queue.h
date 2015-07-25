//priority_queue for persistent vector
//if u need a normal priority_queue, please check the std::priority_queue
#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_
#include "persis_vector.h"
#include <algorithm>


template <class T, class Compare = std::greater<T> >
class PriorityQueue
{
public:
  typedef T value_type;
  typedef value_type* iterator;
  typedef const value_type* const_iterator;
  typedef value_type& reference;
  typedef const value_type& const_reference;

  bool Init(const std::string& file_name)
  {
    return vec_.Init(file_name);
  }

  void Push(value_type value)
  {
    vec_.Push_back(value);
    std::push_heap(vec_.Begin(), vec_.End(), Compare());
  }

  void Pop()
  {
    std::pop_heap(vec_.Begin(), vec_.End(), Compare());
    vec_.Pop_back();
  }

  const_reference Top() const
  {
    return vec_.Front();
  }

  size_t Size() const
  {
    return vec_.Size();
  }

  bool Empty() const
  {
    return vec_.Empty();
  }

  iterator Begin()
  {
    return vec_.Begin();
  }

  const_iterator Begin() const
  {
    return vec_.Begin();
  }

  iterator End()
  {
    return vec_.End();
  }

  const_iterator End() const
  {
    return vec_.End();
  }

private:
  SeriaVector<value_type> vec_;
};

#endif
