#include "ml_sensor.h"

static const adc_channel_t ML8511_ADC_CHANNEL =
    ADC_CHANNEL_6; // Change to your desired ADC channel
static const char *TAG = "ml_sensor";

esp_err_t ml8511_init() {
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ML8511_ADC_CHANNEL, ADC_ATTEN_DB_11);
  return ESP_OK;
}

float ml8511_read_uv() {
  uint32_t adc_value = adc1_get_raw(ML8511_ADC_CHANNEL);
  float voltage = (float)adc_value * (3.3 / (1 << 12));
  float uv_intensity = voltage * ML8511_SCALE_FACTOR;
  return uv_intensity;
}

void ml_task(void *pvParamters) {
  while (1) {
    float uv_metric = ml8511_read_uv();
    ESP_LOGI(TAG, "UV Intensity: %.2f", uv_metric);

    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
  }
}
