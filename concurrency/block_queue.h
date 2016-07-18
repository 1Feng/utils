// Copyright (c) 2016
// Author: @1Feng
// Mail: codingforfan@gmail.com
#ifndef CONCURRENCY_BLOCK_QUEUE_H_
#define CONCURRENCY_BLOCK_QUEUE_H_
#include <queue>
#include "condition.h"  // NOLINT

namespace concurrency {

template <class T>
class BlockQueue {
 public:
  typedef T value_type;

 public:
  BlockQueue() {
  }
  ~BlockQueue() {
  }

  void push(const value_type& t) {
    LockGuard lock_guard(&mutex_);
    queue_.push(t);
    cond_.notify_one();
  }

  value_type pop() {
    UniqueLock unique_lock(mutex_);
    while (queue_.empty()) {
      cond_.wait(&unique_lock);
    }
    value_type t = queue_.front();
    queue_.pop();
    return t;
  }

  inline size_t size() {
    LockGuard lock_guard(&mutex_);
    return queue_.size();
  }

  inline bool empty() {
    LockGuard lock_guard(&mutex_);
    return queue_.empty();
  }

 private:
  BlockQueue(const BlockQueue&);
  void operator=(const BlockQueue&);

 private:
  Mutex      mutex_;
  ConditionVariable cond_;
  std::queue<value_type> queue_;
};

}  // namespace concurrency

#endif  // CONCURRENCY_BLOCK_QUEUE_H_
