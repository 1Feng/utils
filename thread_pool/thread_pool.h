#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_
#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>

// A thread pool impl by boost asio
// https://think-async.com/Asio/Recipes
class ThreadPool : public boost::noncopyable {
public:
  explicit ThreadPool(size_t n)
      : pool_size_(n),
        threads_(),
        io_service_(),
        work_(io_service_) {
    for (size_t i = 0; i < pool_size_; ++i) {
      threads_.create_thread(
          // Here we can not use std::bind in c++11
          // because, it can not deal with overloaded function
          // U can find the reason in  c++ faq
          boost::bind(&boost::asio::io_service::run, &io_service_));
    }
  }

  ~ThreadPool() {
    io_service_.stop();
    threads_.join_all();
  }

  void Post(std::function<void()> closure) {
    io_service_.post(closure);
  }

private:
  size_t pool_size_;
  boost::thread_group threads_;
  boost::asio::io_service io_service_;
  boost::asio::io_service::work work_;
};

#endif  // _THREAD_POOL_H
