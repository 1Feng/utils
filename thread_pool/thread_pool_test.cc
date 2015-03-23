#include <iostream>
#include <sstream>
#include "thread_pool.h"

void test(int i) {
    std::stringstream stream;
    stream << "thread_id: " << boost::this_thread::get_id()
           << " i: " << i << std::endl;
    std::cout << stream.str();
}

int main() {
  ThreadPool pool(10);
  for (size_t i = 0; i < 50; ++i) {
    pool.Post(std::bind(test, i));
  }
  boost::thread_group threads;
  for (size_t i = 50; i < 100; ++i) {
    std::function<void()> func = boost::bind(test, i);
    threads.create_thread(boost::bind(&ThreadPool::Post, &pool, func));
  }
  return 0;
}
