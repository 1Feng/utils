// Copyright (c) 2016
// Author: @1Feng
// Mail: codingforfan@gmail.com
#ifndef CONCURRENCY_THREAD_POOL_H_
#define CONCURRENCY_THREAD_POOL_H_
#include <queue>
#include <boost/bind.hpp>
#include "thread.h"
#include "condition.h"  // NOLINT

namespace concurrency {
class ThreadPool {
 public:
  typedef boost::function<void()> task_type;

 public:
  explicit ThreadPool(size_t n) :
                     stop_(false),
                     size_(0),
                     mutex_(),
                     cond_(),
                     pool_(),
                     queue_(){
    for (size_t i = 0; i < n; ++i) {
      pool_.push_back(new Thread(boost::bind(&ThreadPool::loop, this)));
    }
  }

  ~ThreadPool() {
    {
      LockGuard lock(mutex_);
      stop_ = true;
      cond_.notify_all();
    }
    for (size_t i = 0; i < size_; ++i) {
      pool_[i]->join();
      delete pool_[i];
    }
  }

  inline void add_task(const task_type& task) {
    LockGuard lock(mutex_);
    queue_.push(task);
    cond_.notify_one();
  }

  inline bool queue_empty() {
    LockGuard lock(mutex_);
    return queue_.empty();
  }

  inline size_t queue_size() {
    LockGuard lock(mutex_);
    return queue_.size();
  }

 private:
  // TODO(@yfjiang): there's some bug
  void loop() {
    UniqueLock ulock(mutex_);
    while (true) {
      while (!stop_ && queue_.empty()) {
        cond_.wait(ulock);
      }
      if (stop_) {
        break;
      }
      task_type t = queue_.front();
      queue_.pop();
      ulock.unlock();
      t();
      ulock.lock();
    }
  }

 private:
  ThreadPool(const ThreadPool&);
  void operator=(const ThreadPool&);

 private:
  bool       stop_;
  size_t     size_;
  Mutex      mutex_;
  ConditionVariable     cond_;
  std::vector<Thread*>  pool_;
  std::queue<task_type> queue_;
};

}  // namespace concurrency

#endif  // CONCURRENCY_THREAD_POOL_H_
