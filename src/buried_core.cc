#include "buried_core.h"

Buried::Buried(std::string work_dir) {}

Buried::~Buried() {}

BuriedResult Buried::Start(const Config& config) {
  return BuriedResult::kBuriedOk;
}

BuriedResult Buried::Report(const char* report_data, uint32_t priority) {
  return BuriedResult::kBuriedOk;
}
