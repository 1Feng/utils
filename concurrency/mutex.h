// Copyright (c) 2016
// Author: @1Feng
// Mail: codingforfan@gmail.com
#ifndef CONCURRENCY_MUTEX_H_
#define CONCURRENCY_MUTEX_H_

#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

#ifdef NDEBUG
#define SAFE_PTHREAD(exp)  do {                    \
  int ret = exp;                                   \
  if (ret != 0) abort();                           \
} while (0)
#else
#define SAFE_PTHREAD(exp)  do {                    \
  int ret = exp;                                   \
  assert(ret);                                     \
} while (0)
#endif

namespace concurrency {

class Mutex {
 public:
  Mutex() {
    SAFE_PTHREAD(pthread_mutex_init(&mutex_, NULL));
  }

  ~Mutex() {
    SAFE_PTHREAD(pthread_mutex_destroy(&mutex_));
  }

  inline void lock() {
    SAFE_PTHREAD(pthread_mutex_lock(&mutex_));
  }

  inline bool try_lock() {
    return pthread_mutex_trylock(&mutex_) == 0 ? true : false;
  }

  inline void unlock() {
    SAFE_PTHREAD(pthread_mutex_unlock(&mutex_));
  }

  inline pthread_mutex_t* get_posix_mutex() {
    return &mutex_;
  }

 private:
  // nocopyable
  Mutex(const Mutex&);
  void operator=(const Mutex&);

 private:
  pthread_mutex_t mutex_;
};


class RWMutex {
 public:
  RWMutex() {
    SAFE_PTHREAD(pthread_rwlock_init(&mutex_, NULL));
  }

  ~RWMutex() {
    SAFE_PTHREAD(pthread_rwlock_destroy(&mutex_));
  }

  inline void read_lock() {
    SAFE_PTHREAD(pthread_rwlock_rdlock(&mutex_));
  }

  inline void write_lock() {
    SAFE_PTHREAD(pthread_rwlock_wrlock(&mutex_));
  }

  inline bool try_read_lock() {
    return pthread_rwlock_tryrdlock(&mutex_) == 0 ? true : false;
  }

  inline bool try_write_lock() {
    return pthread_rwlock_trywrlock(&mutex_) == 0 ? true : false;
  }

  inline void unlock() {
    SAFE_PTHREAD(pthread_rwlock_destroy(&mutex_));
  }

 private:
  // nocopyable
  RWMutex(const RWMutex&);
  void operator=(const RWMutex&);

 private:
  pthread_rwlock_t mutex_;
};

class LockGuard {
 public:
  explicit LockGuard(Mutex& mutex) : mutex_(mutex) {
    mutex_.lock();
  }

  ~LockGuard() {
    mutex_.unlock();
  }

 private:
  // nocopyable
  LockGuard(const LockGuard&);
  void operator=(const LockGuard&);

 private:
  Mutex& mutex_;
};

class UniqueLock {
 public:
  explicit UniqueLock(Mutex& mutex) : mutex_(mutex) {
    mutex_.lock();
  }

  ~UniqueLock() {
    mutex_.unlock();
  }

  inline void lock() {
    mutex_.lock();
  }

  inline bool try_lock() {
    return mutex_.try_lock();
  }

  inline void unlock() {
    mutex_.unlock();
  }

 private:
  // nocopyable
  UniqueLock(const UniqueLock&);
  void operator=(const UniqueLock&);

 private:
  Mutex& mutex_;
};

class ReadLock {
 public:
  explicit ReadLock(RWMutex& mutex) : mutex_(mutex) {
    mutex_.read_lock();
  }

  ~ReadLock() {
    mutex_.unlock();
  }

 private:
  // nocopyable
  ReadLock(const ReadLock&);
  void operator=(const ReadLock&);

 private:
  RWMutex& mutex_;
};

class WriteLock {
 public:
  explicit WriteLock(RWMutex& mutex) : mutex_(mutex) {
    mutex_.write_lock();
  }

  ~WriteLock() {
    mutex_.unlock();
  }

 private:
  // nocopyable
  WriteLock(const WriteLock&);
  void operator=(const WriteLock&);

 private:
  RWMutex& mutex_;
};

}  // namespace concurrency

#endif  // CONCURRENCY_MUTEX_H_
