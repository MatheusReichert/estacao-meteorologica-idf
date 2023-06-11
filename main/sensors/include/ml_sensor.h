#ifndef ML8511_H
#define ML8511_H

#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "hal/adc_types.h"
#include <esp_err.h>
#include <esp_log.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define ML8511_SCALE_FACTOR                                                    \
  1.0 // Change this value based on your sensor calibration

extern float uv_metric;
esp_err_t ml8511_init();
float ml8511_read_uv();
void ml_task(void *pvParameters);

#endif /* ML8511_H */
