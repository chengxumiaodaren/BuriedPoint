#include <chrono>
#include <thread>

#include "include/buried.h"

int main() {
  Buried* buried = Buried_Create("D:/buried");
  if (!buried) {
    return -1;
  }
  BuriedConfig config;
  config.host = "localhost";
  config.port = "5678";
  config.topic = "test_topic";
  config.user_id = "test_user";
  config.app_version = "1.0.0";
  config.app_name = "test_app";
  config.custom_data = "{\"test\":\"test\"}";
  Buried_Start(buried, &config);

  std::thread t1([&]() {
    for (int i = 0; i < 100; ++i) {
      Buried_Report(buried, "test_title", "test_data", i);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  });

  std::thread t2([&]() {
    for (int i = 0; i < 100; ++i) {
      Buried_Report(buried, "test_2title", "test_2data", i);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  });

  t1.join();
  t2.join();
  std::this_thread::sleep_for(std::chrono::hours(1));
  Buried_Destroy(buried);
  return 0;
}
