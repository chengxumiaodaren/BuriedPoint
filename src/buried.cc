#include "include/buried.h"

#include <iostream>

#include "buried_core.h"

int BuriedTest() {
  std::cout << "Test";
  return 1;
}

Buried* Buried_Create(const char* work_dir) {
  if (!work_dir) {
    return nullptr;
  }
  return new Buried(work_dir);
}

void Buried_Destroy(Buried* buried) {
  if (buried) {
    delete buried;
  }
}

int32_t Buried_Start(Buried* buried, BuriedConfig* config) {
  if (!buried || !config) {
    return BuriedResult::kBuriedInvalidParam;
  }
  Buried::Config buried_config;
  if (config->host) {
    buried_config.host = config->host;
  }
  if (config->topic) {
    buried_config.topic = config->topic;
  }
  if (config->user_id) {
    buried_config.user_id = config->user_id;
  }
  if (config->version) {
    buried_config.version = config->version;
  }
  if (config->custom_data) {
    buried_config.custom_data = config->custom_data;
  }
  return buried->Start(buried_config);
}

int32_t Buried_Report(Buried* buried, const char* report_data,
                      uint32_t priority) {
  if (!buried || !report_data) {
    return BuriedResult::kBuriedInvalidParam;
  }
  return buried->Report(report_data, priority);
}
