#pragma once

#include <stdint.h>

#include <filesystem>
#include <memory>
#include <string>

#include "buried_common.h"
#include "include/buried.h"

namespace spdlog {
class logger;
}

namespace buried {
class BuriedReport;
}

struct Buried {
 public:
  struct Config {
    std::string host;
    std::string port;
    std::string topic;
    std::string user_id;
    std::string app_version;
    std::string app_name;
    std::string custom_data;
  };

 public:
  Buried(const std::string& work_dir);

  ~Buried();

  BuriedResult Start(const Config& config);

  BuriedResult Report(std::string title, std::string data, uint32_t priority);

 public:
  std::shared_ptr<spdlog::logger> Logger();

 private:
  void InitWorkPath_(const std::string& work_dir);

  void InitLogger_();

 private:
  std::shared_ptr<spdlog::logger> logger_;
  std::unique_ptr<buried::BuriedReport> buried_report_;

  std::filesystem::path work_path_;
};
