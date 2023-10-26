#include <filesystem>
#include <iostream>
#include <unordered_set>

#include "gtest/gtest.h"
#include "src/common/common_service.h"

TEST(CommonServiceTest, RandomStringTest) {
  std::unordered_set<std::string> random_set;
  for (int i = 0; i < 1000; ++i) {
    std::string rn = buried::CommonService::GetRandomId();
    std::cout << rn << std::endl;
    EXPECT_EQ(rn.size(), 32);
    EXPECT_EQ(random_set.count(rn), 0);
    random_set.insert(rn);
  }
}

TEST(CommonServiceTest, GetNowDateTest) {
  std::string now_date = buried::CommonService::GetNowDate();
  std::cout << now_date << std::endl;
  EXPECT_GT(now_date.size(), 0);
}

TEST(CommonServiceTest, GetProcessTimeTest) {
  std::string process_time = buried::CommonService::GetProcessTime();
  std::cout << process_time << std::endl;
  EXPECT_GT(process_time.size(), 0);
}

TEST(CommonServiceTest, BasicTest) {
  buried::CommonService common_service;
  EXPECT_GT(common_service.system_version.size(), 0);
  EXPECT_GT(common_service.device_name.size(), 0);
  EXPECT_GT(common_service.device_id.size(), 0);
  EXPECT_GT(common_service.buried_version.size(), 0);
  EXPECT_GT(common_service.lifecycle_id.size(), 0);
}
