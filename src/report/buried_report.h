#pragma once

#include <stdint.h>

#include <memory>
#include <string>

#include "common/common_service.h"

namespace spdlog {
class logger;
}

namespace buried {

struct BuriedData {
  std::string title;
  std::string data;
  uint32_t priority;
};

class BuriedReportImpl;
class BuriedReport {
 public:
  BuriedReport(std::shared_ptr<spdlog::logger> logger,
               CommonService common_service, std::string work_path);

  ~BuriedReport();

  void Start();

  void InsertData(const BuriedData& data);

 private:
  std::unique_ptr<BuriedReportImpl> impl_;
};

}  // namespace buried
