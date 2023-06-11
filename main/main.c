#include "esp_log.h"
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <i2cdev.h>
#include <string.h>

#include "bme_sensor.h"
#include "ml_sensor.h"
#include "mqtt.h"
#include "sgp_sensor.h"
#include "tsl_sensor.h"
#include "wifi_config.h"

bme680_values bmp_values;
lux lux_metric;
hdc hdc_values;
float uv_metric;
int32_t voc_index;

esp_err_t init_sensors() {

  ESP_ERROR_CHECK(i2cdev_init());
  ESP_ERROR_CHECK(bme_init());
  ESP_ERROR_CHECK(ml8511_init());

  return ESP_OK;
}
esp_err_t init_background_tasks() {
  xTaskCreatePinnedToCore(tsl2591_task, "tsl2591_task",
                          configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL,
                          APP_CPU_NUM);
  xTaskCreatePinnedToCore(bme_read_task, "bme_read_task",
                          configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL,
                          APP_CPU_NUM);
  xTaskCreatePinnedToCore(sgp_task, "SGP_read_task",
                          configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL,
                          APP_CPU_NUM);
  xTaskCreatePinnedToCore(ml_task, "ml_read_task", configMINIMAL_STACK_SIZE * 8,
                          NULL, 5, NULL, APP_CPU_NUM);
  return ESP_OK;
}
void app_main(void) {

  static const char *TAG = "app_main";

  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", (int)esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

  // WIFI
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
  wifi_init_sta();

  // END WIFI
  // MQTT
  mqtt_app_start();
  //

  ESP_ERROR_CHECK(init_sensors());
  ESP_ERROR_CHECK(init_background_tasks());
}
