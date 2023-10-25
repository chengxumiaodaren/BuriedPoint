#pragma once

#include <stdint.h>

#include <string>

#include "nlohmann/json.hpp"

namespace buried {

struct CommonService {
 public:
  std::string host;
  std::string port;
  std::string topic;
  std::string user_id;
  std::string app_version;
  std::string app_name;

  nlohmann::json custom_data;

  std::string system_version;
  std::string device_name;
  std::string device_model;
  std::string device_id;
  std::string buried_version;
  std::string lifecycle_id;

 public:
  CommonService();

  static std::string GetProcessTime();

  static std::string GetNowDate();

  static std::string GetRandomId();

 private:
  void Init();
};

}  // namespace buried
