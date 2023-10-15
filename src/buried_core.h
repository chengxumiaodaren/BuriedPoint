#pragma once

#include <stdint.h>

#include <string>

#include "buried_common.h"
#include "include/buried.h"

class Buried {
 public:
  struct Config {
    std::string host;
    std::string topic;
    std::string user_id;
    std::string version;
    std::string custom_data;
  };

 public:
  Buried(std::string work_dir);

  ~Buried();

  BuriedResult Start(const Config& config);

  BuriedResult Report(const char* report_data, uint32_t priority);
};
