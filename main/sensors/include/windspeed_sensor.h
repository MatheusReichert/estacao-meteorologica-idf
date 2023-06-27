#pragma once

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <stdint.h>
#include <stdio.h>

#define ANEMOMETER_PIN 4

extern float windSpeed;

esp_err_t init_windspeed_sensor();

void anemometerInterruptHandler(void *arg);

void anemometerTask(void *pvParameters);
