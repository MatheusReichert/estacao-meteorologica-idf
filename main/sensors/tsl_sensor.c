#include "tsl_sensor.h"
#include "tsl2591.h"
#include <esp_log.h>

static const char *TAG = "bme-sensor.c";

void tsl2591_task(void *pvParameters) {
  tsl2591_t dev = {0};

  ESP_ERROR_CHECK(tsl2591_init_desc(&dev, 0, CONFIG_EXAMPLE_I2C_MASTER_SDA,
                                    CONFIG_EXAMPLE_I2C_MASTER_SCL));
  ESP_ERROR_CHECK(tsl2591_init(&dev));

  tsl2591_set_power_status(&dev, TSL2591_POWER_ON);
  tsl2591_set_als_status(&dev, TSL2591_ALS_ON);
  tsl2591_set_gain(&dev, TSL2591_GAIN_HIGH);
  tsl2591_set_integration_time(&dev, TSL2591_INTEGRATION_100MS);

  esp_err_t res;
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(CONFIG_TASK_DELAY));

    if ((res = tsl2591_get_lux(&dev, &lux_metric.value)) != ESP_OK)
      ESP_LOGI(TAG, "Could not read lux value: %d", res);
    else
      ESP_LOGI(TAG, "Lux: %f", lux_metric.value);
  }
}