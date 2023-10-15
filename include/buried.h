#pragma once

#include <stdint.h>

int BuriedTest();

typedef struct Buried Buried;

struct BuriedConfig {
  const char* host;
  const char* topic;
  const char* user_id;
  const char* version;
  const char* custom_data;
};

Buried* Buried_Create(const char* work_dir);

void Buried_Destroy(Buried* buried);

int32_t Buried_Start(Buried* buried, BuriedConfig* config);

int32_t Buried_Report(Buried* buried, const char* report_data,
                      uint32_t priority);
