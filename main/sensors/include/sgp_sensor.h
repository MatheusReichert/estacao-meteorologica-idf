#pragma once


#include <inttypes.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <sgp40.h>
#include <hdc1000.h>
#include <string.h>
#include <esp_err.h>
#include <esp_log.h>

typedef struct hdc1080{
    float temperature;
    float humidity;
} hdc;

extern hdc hdc_values;

void sgp_task(void *pvParamters);