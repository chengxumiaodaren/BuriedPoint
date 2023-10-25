#include <chrono>
#include <iostream>
#include <thread>

#include "boost/asio.hpp"
#include "gtest/gtest.h"

TEST(BuriedExecutorTest, DISABLED_BasicTest) {
  boost::asio::io_context io_context1;
  boost::asio::io_context io_context2;
  boost::asio::io_context io_context3;

  boost::asio::io_context::strand strand1(io_context1);
  boost::asio::io_context::strand strand2(io_context2);
  boost::asio::io_context::strand strand3(io_context3);

  boost::asio::post(strand1, []() {
    std::cout << "Operation 1 executed in strand1 on thread id "
              << std::this_thread::get_id() << std::endl;
  });

  boost::asio::post(strand2, [&]() {
    std::cout << "Operation 2 executed in strand2 on thread id "
              << std::this_thread::get_id() << std::endl;

    boost::asio::post(strand2, []() {
      std::cout << "Operation 3 executed in strand2 on thread id "
                << std::this_thread::get_id() << std::endl;
    });

    boost::asio::post(strand1, []() {
      std::cout << "Operation 4 executed in strand1 on thread id "
                << std::this_thread::get_id() << std::endl;
    });

    boost::asio::post(strand3, []() {
      std::cout << "Operation 5 executed in strand3 on thread id "
                << std::this_thread::get_id() << std::endl;
    });
  });

  boost::asio::post(strand3, []() {
    std::cout << "Operation 6 executed in strand3 on thread id "
              << std::this_thread::get_id() << std::endl;
  });

  // 运行 io_context 以触发异步操作
  std::thread io_thread([&]() {
    io_context1.run();
    io_context2.run();

    boost::asio::post(strand3, []() {
      std::cout << "Operation 7 executed in strand3 on thread id "
                << std::this_thread::get_id() << std::endl;
    });

    for (int i = 0; i < 20; ++i) {
      io_context1.run();
      io_context2.run();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  });

  boost::asio::post(strand3, []() {
    std::cout << "Operation 8 executed in strand3 on thread id "
              << std::this_thread::get_id() << std::endl;
  });

  io_context3.run();

  std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(1));

  boost::asio::post(strand3, []() {
    std::cout << "Operation 9 executed in strand3 on thread id "
              << std::this_thread::get_id() << std::endl;
  });

  for (int i = 0; i < 20; ++i) {
    io_context3.run();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  io_thread.join();
}

#include <iostream>

#include "boost/asio.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

class printer {
 public:
  printer(boost::asio::io_service& io)
      : strand_(io),
        timer1_(io, boost::posix_time::seconds(1)),
        timer2_(io, boost::posix_time::seconds(1)),
        count_(0) {
    timer1_.async_wait(strand_.wrap(std::bind(&printer::print1, this)));
    timer2_.async_wait(strand_.wrap(std::bind(&printer::print2, this)));
  }

  ~printer() { std::cout << "Final count is " << count_ << std::endl; }
  void print1() {
    if (count_ < 10) {
      std::cout << "Timer 1: " << count_ << std::endl;
      ++count_;

      timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
      timer1_.async_wait(strand_.wrap(std::bind(&printer::print1, this)));
    }
  }

  void print2() {
    if (count_ < 10) {
      std::cout << "Timer 2: " << count_ << std::endl;
      ++count_;

      timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
      timer2_.async_wait(strand_.wrap(std::bind(&printer::print2, this)));
    }
  }

 private:
  boost::asio::io_service::strand strand_;
  boost::asio::deadline_timer timer1_;
  boost::asio::deadline_timer timer2_;
  int count_;
};

TEST(BuriedExecutorTest, TimerTest) {
  boost::asio::io_service io;
  printer p(io);
  std::thread t([&]() { io.run(); });
  // io.run();
  t.join();
}
