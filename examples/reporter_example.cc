#include <chrono>
#include <iostream>
#include <thread>

#include "src/context/context.h"
#include "src/report/buried_report.h"

int main() {
  buried::Context::GetGlobalContext().Start();

  buried::CommonService common_service;
  common_service.app_name = "test_app";
  common_service.app_version = "1.0.0";
  common_service.host = "localhost";
  common_service.topic = "test_topic";
  common_service.port = "5678";
  buried::BuriedReport buried_report(nullptr, common_service, "D:/");
  buried_report.Start();

  std::thread t1([&]() {
    for (int i = 0; i < 100; ++i) {
      buried::BuriedData data;
      data.title = "test_title";
      data.data = "test_data";
      data.priority = i;
      buried_report.InsertData(data);
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
  });

  std::thread t2([&]() {
    for (int i = 0; i < 100; ++i) {
      buried::BuriedData data;
      data.title = "test_2title";
      data.data = "test_2data";
      data.priority = i;
      buried_report.InsertData(data);
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
  });
  t1.join();
  t2.join();
  std::this_thread::sleep_for(std::chrono::hours(1));
  return 0;
}
