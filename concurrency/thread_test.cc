// Copyright (c) 2016
// Author: @1Feng
// Mail: codingforfan@gmail.com
#include <cstdio>
#include <iostream>
#include <vector>
#include <boost/bind.hpp>
#include "thread.h"     // NOLINT
#include "mutex.h"      // NOLINT
#include "condition.h"  // NOLINT
#include "thread_pool.h"     // NOLINT

void func(int a, concurrency::Mutex& m) {  // NOLINT
  for (int i = 0; i < 10; i ++) {
     //printf("%p\n", static_cast<void*>(&m));
     concurrency::LockGuard LockGuard(m);
     std::cout << concurrency::Thread::current_threadid()
               << '\t' << a + i << std::endl;
  }
}


/*
int main() {
  concurrency::Mutex mutex;
  printf("%p\n", static_cast<void*>(&mutex));
  boost::function<void()> func0 = boost::bind(func, 1000, boost::ref(mutex));
  boost::function<void()> func1 = boost::bind(func, 2000, boost::ref(mutex));
  std::vector<boost::function<void()> > v;
  std::vector<concurrency::Thread*> threads;
  v.push_back(func0);
  v.push_back(func1);
  for (size_t i = 0; i < v.size(); ++i) {
    threads.push_back(new concurrency::Thread(v[i]));
  }
  printf("id0: %d\n", threads[0]->get_id());
  printf("id1: %d\n", threads[1]->get_id());
  for (size_t i = 0; i < threads.size(); ++i) {
    threads[i]->join();
    delete threads[i];
  }
  return 0;
}
*/

int main() {
  concurrency::ThreadPool pool(3);
  concurrency::Mutex mutex;
  for (int i = 0; i < 10; ++i) {
    pool.add_task(boost::bind(func, i * 10, boost::ref(mutex)));
  }
  printf("queue size : %d\n", static_cast<int>(pool.queue_size()));
  // sleep(20);
  return 0;
}
