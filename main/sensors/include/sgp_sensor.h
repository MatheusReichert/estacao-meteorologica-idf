#pragma once

#include <esp_err.h>
#include <esp_log.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <hdc1000.h>
#include <inttypes.h>
#include <sgp40.h>
#include <stdio.h>
#include <string.h>

typedef struct hdc1080 {
  float temperature;
  float humidity;
} hdc;

extern hdc hdc_values;
// Feed it to SGP40
extern int32_t voc_index;

void sgp_task(void *pvParamters);