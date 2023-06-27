#include "ml_sensor.h"
#include "driver/adc.h"
#include "driver/adc_types_legacy.h"
#include "esp_adc_cal.h"
#include <stdint.h>

static const adc_channel_t ML8511_ADC_CHANNEL =
    ADC1_CHANNEL_6; // Change to your desired ADC channel
static const adc_channel_t ML8511_REF_CHANNEL = ADC1_CHANNEL_4;
static const char *TAG = "ml_sensor";

esp_err_t ml8511_init() {
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ML8511_ADC_CHANNEL, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ML8511_REF_CHANNEL, ADC_ATTEN_DB_11);

  return ESP_OK;
}

int averageAnalogRead(gpio_num_t adc_channel) {
  short numberOfReadings = 8;
  unsigned int runningValue = 0;

  for (int x = 0; x < numberOfReadings; x++)
    runningValue += adc1_get_raw(adc_channel);
  runningValue /= numberOfReadings;

  return (runningValue);
}

float mapfloat(float x, float in_min, float in_max, float out_min,
               float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float ml8511_read_uv() {
  uint32_t adc_value = averageAnalogRead(ML8511_ADC_CHANNEL);
  uint32_t ref_value = averageAnalogRead(ML8511_REF_CHANNEL);
  float voltage = 3.3 / ref_value * adc_value;
  float uv_intensity = mapfloat(voltage, 0.86, 2.8, 0.0, 15.0);
  return uv_intensity;
}

void ml_task(void *pvParamters) {
  while (1) {
    uv_metric = ml8511_read_uv();
    ESP_LOGI(TAG, "UV Intensity: %.2f", uv_metric);

    vTaskDelay(pdMS_TO_TICKS(CONFIG_TASK_DELAY)); // Delay for 1 second
  }
}
