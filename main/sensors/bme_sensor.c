#include "bme_sensor.h"

static bme680_t sensor; // Declare the sensor variable as extern

static const char *TAG = "bme-sensor.c";

esp_err_t bme_init(void) {
  memset(&sensor, 0, sizeof(bme680_t));

  // Initialize the sensor descriptor
  ESP_ERROR_CHECK(bme680_init_desc(&sensor, ADDR, PORT,
                                   (gpio_num_t)CONFIG_EXAMPLE_I2C_MASTER_SDA,
                                   (gpio_num_t)CONFIG_EXAMPLE_I2C_MASTER_SCL));

  // Initialize the sensor
  ESP_ERROR_CHECK(bme680_init_sensor(&sensor));

  // Set the oversampling rates for temperature, humidity, and pressure
  bme680_set_oversampling_rates(&sensor, BME680_OSR_4X, BME680_OSR_2X,
                                BME680_OSR_2X);

  // Set the IIR filter size for temperature and pressure
  bme680_set_filter_size(&sensor, BME680_IIR_SIZE_7);

  // Set the heater profile for the sensor
  bme680_set_heater_profile(&sensor, 0, 200, 100);
  bme680_use_heater_profile(&sensor, 0);

  // Set the ambient temperature
  bme680_set_ambient_temperature(&sensor, 10);

  return ESP_OK;
}

void bme_read_task(void *pvParameters) {
  // Get the measurement duration of the sensor
  uint32_t duration;
  bme680_get_measurement_duration(&sensor, &duration);

  while (1) {
    // Trigger the sensor to start one TPHG measurement cycle
    if (bme680_force_measurement(&sensor) == ESP_OK) {
      // Passive wait until measurement results are available
      vTaskDelay(duration);

      // Get the results and print them
      if (bme680_get_results_float(&sensor, &bmp_values) == ESP_OK) {
        ESP_LOGI(TAG, "BME680 Sensor: %.2f °C, %.2f %%, %.2f hPa, %.2f Ohm",
                 bmp_values.temperature, bmp_values.humidity,
                 bmp_values.pressure, bmp_values.gas_resistance);
      }
    }

    // Passive wait for 1 second
    vTaskDelay(pdMS_TO_TICKS(CONFIG_TASK_DELAY));
  }
}
