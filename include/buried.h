#pragma once

#include <stdint.h>

#define BURIED_EXPORT __declspec(dllexport)

extern "C" {

BURIED_EXPORT int BuriedTest();

typedef struct Buried Buried;

struct BuriedConfig {
  const char* host;
  const char* topic;
  const char* user_id;
  const char* version;
  const char* custom_data;
};

BURIED_EXPORT Buried* Buried_Create(const char* work_dir);

BURIED_EXPORT void Buried_Destroy(Buried* buried);

BURIED_EXPORT int32_t Buried_Start(Buried* buried, BuriedConfig* config);

BURIED_EXPORT int32_t Buried_Report(Buried* buried, const char* report_data,
                                    uint32_t priority);
}
