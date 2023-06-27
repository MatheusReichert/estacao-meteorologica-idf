#include "esp_log.h"
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <i2cdev.h>
#include <stdio.h>
#include <string.h>

#include "bme_sensor.h"
#include "ml_sensor.h"
#include "mqtt.h"
#include "sgp_sensor.h"
#include "tsl_sensor.h"
#include "wifi_config.h"
#include "windspeed_sensor.h"

bme680_values bmp_values;
lux lux_metric;
hdc hdc_values;
float uv_metric;
int32_t voc_index;
esp_mqtt_client_handle_t client;
float windSpeed;
time_t now;

TimerHandle_t timerHandle;
static const char *TAG = "app_main";

char *create_payload() {
  // Get time now
  time(&now);
  // Convert the time to Unix timestamp
  int timestamp = (int)now;

  // Calculate the size of the payload string
  int payload_size = snprintf(
      NULL, 0,
      "{\r\n  \"timestamp\": %d,\r\n  \"gas_resistance\": %.2f,\r\n  "
      "\"humidity\": %.2f,\r\n  \"hdc_humidity\": %.2f,\r\n  "
      "\"hdc_temperature\": %.2f,\r\n  \"lux\": \"%.2f,\r\n  \"pressure\": "
      "%.2f,\r\n  \"temperature\": %.2f,\r\n  \"uv\": \"%.2f,\r\n  "
      "\"voc_index\": %d,\r\n  \"wind_speed\": %.2f\r\n}\r\n",
      timestamp, bmp_values.gas_resistance, bmp_values.humidity,
      hdc_values.humidity, hdc_values.temperature, lux_metric.value,
      bmp_values.pressure, bmp_values.temperature, uv_metric, (int)voc_index,
      windSpeed);

  // Create a buffer to store the payload string
  char *payload = (char *)malloc((payload_size + 1) * sizeof(char));
  if (payload == NULL) {
    printf("Failed to allocate memory for payload!\n");
    return NULL;
  }

  // Format the payload string
  snprintf(
      payload, payload_size + 1,
      "{\r\n  \"timestamp\": %d,\r\n  \"gas_resistance\": %.2f,\r\n  "
      "\"humidity\": %.2f,\r\n  \"hdc_humidity\": %.2f,\r\n  "
      "\"hdc_temperature\": %.2f,\r\n  \"lux\": \"%.2f,\r\n  \"pressure\": "
      "%.2f,\r\n  \"temperature\": %.2f,\r\n  \"uv\": \"%.2f,\r\n  "
      "\"voc_index\": %d,\r\n  \"wind_speed\": %.2f\r\n}\r\n",
      timestamp, bmp_values.gas_resistance, bmp_values.humidity,
      hdc_values.humidity, hdc_values.temperature, lux_metric.value,
      bmp_values.pressure, bmp_values.temperature, uv_metric, (int)voc_index,
      windSpeed);
  return payload;
}

void timerCallback(TimerHandle_t xTimer) {
  ESP_LOGI(TAG, "Publicando mensagem\n");
  char *payload = create_payload();
  if (client != NULL) {
    esp_mqtt_client_publish(client, "metrics", payload, 0, 0, 0);
  }
}

esp_err_t init_sensors() {
  esp_err_t i2c_err = i2cdev_init();
  esp_err_t bme_err = bme_init();
  esp_err_t ml_err = ml8511_init();
  esp_err_t windspeed_err = init_windspeed_sensor();

  if (i2c_err != ESP_OK) {
    return i2c_err;
  }

  if (bme_err != ESP_OK) {
    return bme_err;
  }

  if (ml_err != ESP_OK) {
    return ml_err;
  }

  if (windspeed_err != ESP_OK) {
    return windspeed_err;
  }
  return ESP_OK;
}

esp_err_t init_background_tasks() {
  BaseType_t task1 = xTaskCreatePinnedToCore(tsl2591_task, "tsl2591_task",
                                             configMINIMAL_STACK_SIZE * 8, NULL,
                                             5, NULL, APP_CPU_NUM);
  BaseType_t task2 = xTaskCreatePinnedToCore(bme_read_task, "bme_read_task",
                                             configMINIMAL_STACK_SIZE * 8, NULL,
                                             5, NULL, APP_CPU_NUM);
  BaseType_t task3 = xTaskCreatePinnedToCore(sgp_task, "SGP_read_task",
                                             configMINIMAL_STACK_SIZE * 8, NULL,
                                             5, NULL, APP_CPU_NUM);
  BaseType_t task4 = xTaskCreatePinnedToCore(ml_task, "ml_read_task",
                                             configMINIMAL_STACK_SIZE * 8, NULL,
                                             5, NULL, APP_CPU_NUM);
  if (task1 != pdPASS || task2 != pdPASS || task3 != pdPASS ||
      task4 != pdPASS) {
    return ESP_FAIL;
  }

  return ESP_OK;
}

void app_main(void) {

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

  // SENSORS
  ESP_ERROR_CHECK(init_sensors());
  ESP_ERROR_CHECK(init_background_tasks());
  //

  // PUBLISH
  timerHandle = xTimerCreate("Mqtt timer", pdMS_TO_TICKS(300000), pdTRUE, 0,
                             timerCallback);

  if (timerHandle != NULL) {
    // Inicie o timer
    if (xTimerStart(timerHandle, 0) != pdPASS) {
      printf("Falha ao iniciar o timer!\n");
    }
  } else {
    printf("Falha ao criar o timer!\n");
  }
  //
}
