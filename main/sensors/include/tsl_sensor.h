#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <tsl2591.h>

typedef struct lux_metric{
    float value;
} lux;

extern lux lux_metric;

void tsl2591_task(void *pvParameters);