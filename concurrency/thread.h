// Copyright (c) 2016
// Author: @1Feng
// Mail: codingforfan@gmail.com
#ifndef CONCURRENCY_THREAD_H_
#define CONCURRENCY_THREAD_H_
#include <pthread.h>
#include <syscall.h>
#include <assert.h>
#include <boost/function.hpp>

#ifdef NDEBUG
#define SAFE_PTHREAD(exp)  do {                    \
  int ret = exp;                                   \
  if (ret != 0) abort();                           \
} while (0)
#else
#define SAFE_PTHREAD(exp)  do {                    \
  int ret = exp;                                   \
  assert(!ret);                                    \
} while (0)
#endif
namespace concurrency {

class ProxyObj {
 public:
  ProxyObj(void* func, pid_t* tid) : tid_(tid), func_(func) {
  }

  inline void* get_func() {
    return func_;
  }

  inline void set_tid(pid_t tid) {
    *tid_ = tid;
  }

 private:
  pid_t* tid_;
  void*  func_;
};

template <class Fp>
void* thread_routine(void* vp) {
  ProxyObj* obj(static_cast<ProxyObj*>(vp));
  Fp* p(static_cast<Fp*>(obj->get_func()));
  pid_t tid = syscall(SYS_gettid);
  obj->set_tid(tid);
  (*p)();
  delete obj;
}


// issue: Thread memeber function could run before the real thread_routine
class Thread {
 public:
/*
#ifdef __GXX_EXPERIMENTAL_CXX0X__
  typedef std::function<void ()>  ThreadFuncObj;
#else
*/
  typedef boost::function<void()> ThreadFuncObj;
/*
#endif
*/

 public:
  explicit Thread(const ThreadFuncObj& func) :
                              tid_(0),
                              joined_(false),
                              thread_id_(0),
                              func_(func) {
    ProxyObj* obj = new ProxyObj(static_cast<void*>(&func_), &tid_);
    SAFE_PTHREAD(pthread_create(&thread_id_, NULL,
                                &thread_routine<ThreadFuncObj>,
                                static_cast<void*>(obj)));
  }

/*
#ifdef __GXX_EXPERIMENTAL_CXX0X__
  template <class F, class ...Args> explicit thread(F&& f, Args&&... args) {
    // TODO(@1Feng): impl
  }
#endif
*/

  ~Thread() {
    if (!joined_) {
      pthread_detach(thread_id_);
    }
  }

  inline void join() {
    assert(!joined_);
    joined_ = true;
    SAFE_PTHREAD(pthread_join(thread_id_, NULL));
  }

  inline void detach() {
    assert(!joined_);
    pthread_detach(thread_id_);
  }

  inline bool joinalbe() {
    if (!joined_) {
      return true;
    }
    return false;
  }

  inline pid_t get_id() {
    while (tid_ == 0) {}
    return tid_;
  }

  static pid_t current_threadid() {
    pid_t tid = syscall(SYS_gettid);
    return tid;
  }

 private:
  // nocopyable
  Thread(const Thread&);
  void operator=(const Thread&);

 private:
  pid_t      tid_;
  bool       joined_;
  pthread_t  thread_id_;
  ThreadFuncObj func_;
};
}  // namespace concurrency

#endif  // CONCURRENCY_THREAD_H_
