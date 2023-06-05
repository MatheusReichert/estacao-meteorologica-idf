#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <string.h>
#include "esp_log.h"
#include <i2cdev.h>

#include "sgp_sensor.h"
#include "bme_sensor.h"
#include "tsl_sensor.h"


bme680_values bmp_values;
lux lux_metric;
hdc hdc_values;

static const char *TAG = "app_main";

void app_main(void)
{
    ESP_ERROR_CHECK(i2cdev_init());
    bme_init();
    xTaskCreatePinnedToCore(tsl2591_task, "tsl2591_test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
    xTaskCreatePinnedToCore(bme_read_task, "bme_read_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
    xTaskCreatePinnedToCore(sgp_task, "SGP_read_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);


}
