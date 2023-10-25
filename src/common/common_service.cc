#include "common/common_service.h"

#include <chrono>
#include <ctime>

namespace buried {

CommonService::CommonService() { Init(); }

void CommonService::Init() {}

std::string CommonService::GetProcessTime() { return ""; }

std::string CommonService::GetNowDate() {
  auto t =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  return std::ctime(&t);
}

std::string CommonService::GetRandomId() { return ""; }

}  // namespace buried
