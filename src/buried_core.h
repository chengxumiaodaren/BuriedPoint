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

struct Buried {
 public:
  struct Config {
    std::string host;
    std::string topic;
    std::string user_id;
    std::string version;
    std::string custom_data;
  };

 public:
  Buried(const std::string& work_dir);

  ~Buried();

  BuriedResult Start(const Config& config);

  BuriedResult Report(const char* report_data, uint32_t priority);

 public:
  std::shared_ptr<spdlog::logger> Logger();

 private:
  void InitWorkPath_(const std::string& work_dir);

  void InitLogger_();

 private:
  std::shared_ptr<spdlog::logger> logger_;

  std::filesystem::path work_path_;
};
