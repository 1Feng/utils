####demo:
```CPP
#include <cstdio>
#include <iostream>
#include <vector>
#include <boost/bind.hpp>
#include "thread_pool.h"     // NOLINT

void func(int a, concurrency::Mutex& m) {  // NOLINT
  for (int i = 0; i < 10; i ++) {
     concurrency::LockGuard LockGuard(m);
     std::cout << concurrency::Thread::current_threadid()
               << '\t' << a + i << std::endl;
  }
}

int main() {
  // make sure the shared variable should destruct after the ThreadPool
  concurrency::Mutex mutex;
  concurrency::ThreadPool pool(10);
  for (int i = 0; i < 10; ++i) {
    pool.add_task(boost::bind(func, i * 10, boost::ref(mutex)));
  }
  printf("queue size : %d\n", static_cast<int>(pool.queue_size()));
  return 0;
}
```
