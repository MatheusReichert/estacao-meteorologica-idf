#include "sgp_sensor.h"

static const char *TAG = "sgp-sensor.c";

static const char *voc_index_name(int32_t voc_index) {
  if (voc_index <= 0)
    return "INVALID VOC INDEX";
  else if (voc_index <= 10)
    return "unbelievable clean";
  else if (voc_index <= 30)
    return "extremely clean";
  else if (voc_index <= 50)
    return "higly clean";
  else if (voc_index <= 70)
    return "very clean";
  else if (voc_index <= 90)
    return "clean";
  else if (voc_index <= 120)
    return "normal";
  else if (voc_index <= 150)
    return "moderately polluted";
  else if (voc_index <= 200)
    return "higly polluted";
  else if (voc_index <= 300)
    return "extremely polluted";

  return "RUN!";
}

void sgp_task(void *pvParamters) {
  hdc1000_t hdc;
  sgp40_t sgp;

  // setup hdc1000
  memset(&hdc, 0, sizeof(hdc));
  ESP_ERROR_CHECK(hdc1000_init_desc(&hdc, 0x40, 0,
                                    CONFIG_EXAMPLE_I2C_MASTER_SDA,
                                    CONFIG_EXAMPLE_I2C_MASTER_SCL));
  ESP_ERROR_CHECK(hdc1000_init(&hdc));

  uint32_t serial[2];
  uint16_t dev_id, manuf_id;

  ESP_ERROR_CHECK(hdc1000_get_serial(&hdc, (uint64_t *)serial));
  ESP_ERROR_CHECK(hdc1000_get_manufacturer_id(&hdc, &manuf_id));
  ESP_ERROR_CHECK(hdc1000_get_device_id(&hdc, &dev_id));

  ESP_LOGI(TAG,
           "HDC1000, Manufacturer ID: 0x%04x, device ID: 0x%04x, serial "
           "number: 0x%08" PRIx32 "%08" PRIx32,
           manuf_id, dev_id, serial[0], serial[1]);

  // setup SGP40
  memset(&sgp, 0, sizeof(sgp));
  ESP_ERROR_CHECK(sgp40_init_desc(&sgp, 0, CONFIG_EXAMPLE_I2C_MASTER_SDA,
                                  CONFIG_EXAMPLE_I2C_MASTER_SCL));
  ESP_ERROR_CHECK(sgp40_init(&sgp));
  ESP_LOGI(TAG, "SGP40 initilalized. Serial: 0x%04x%04x%04x", sgp.serial[0],
           sgp.serial[1], sgp.serial[2]);

  // Wait until all set up
  vTaskDelay(pdMS_TO_TICKS(250));

  TickType_t last_wakeup = xTaskGetTickCount();

  while (1) {

    ESP_ERROR_CHECK(
        hdc1000_measure(&hdc, &hdc_values.temperature, &hdc_values.humidity));

    ESP_ERROR_CHECK(sgp40_measure_voc(&sgp, hdc_values.humidity,
                                      hdc_values.temperature, &voc_index));

    ESP_LOGI(TAG, "%.2f °C, %.2f %%, VOC index: %" PRIi32 ", Air is [%s]",
             hdc_values.temperature, hdc_values.humidity, voc_index,
             voc_index_name(voc_index));

    // Wait until 1 seconds (VOC cycle time) are over.
    vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(1000));
  }
}
