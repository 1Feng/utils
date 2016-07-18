// Copyright (c) 2016
// Author: @1Feng
// Mail: codingforfan@gmail.com
#ifndef CONCURRENCY_CONDITION_H_
#define CONCURRENCY_CONDITION_H_

#include <pthread.h>
#include <time.h>
#include <errno.h>
#include "mutex.h"  // NOLINT

namespace concurrency {

class ConditionVariable {
 public:
  ConditionVariable() {
    SAFE_PTHREAD(pthread_cond_init(&cond_, NULL));
  }

  ~ConditionVariable() {
    SAFE_PTHREAD(pthread_cond_destroy(&cond_));
  }

  inline void wait(UniqueLock& ulock) {  // NOLINT
    assert(ulock.owns_lock());
    SAFE_PTHREAD(pthread_cond_wait(&cond_,
                                   ulock.get_mutex().get_posix_mutex()));
  }

  // return true if timeout, otherwise return false
  inline bool wait_for(UniqueLock& ulock, int seconds) {  // NOLINT
    assert(ulock.owns_lock());
    struct timespec timeout = {seconds, 0};
    int ret = pthread_cond_timedwait(&cond_,
                                     ulock.get_mutex().get_posix_mutex(),
                                     &timeout);
    if (ret == ETIMEDOUT) {
      return true;
    }
    return false;
  }

  inline void notify_one() {
    SAFE_PTHREAD(pthread_cond_signal(&cond_));
  }

  inline void notify_all() {
    SAFE_PTHREAD(pthread_cond_broadcast(&cond_));
  }

 private:
  // nocopyable
  ConditionVariable(const ConditionVariable&);
  void operator=(const ConditionVariable&);

 private:
  pthread_cond_t cond_;
};

}  // namespace concurrency

#endif  // CONCURRENCY_CONDITION_H_
