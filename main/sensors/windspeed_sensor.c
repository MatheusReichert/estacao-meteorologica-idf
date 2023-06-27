#include "windspeed_sensor.h"
#include "esp_log.h"

static const char *TAG = "Windspeed Sensor";
const float pi = 3.14159265; // Número de pi
const int period = 5000;     // Tempo de medida (milissegundos)
const int delaytime = 2000;  // Intervalo entre as amostras (milissegundos)
const int radius = 147;      // Raio do anemômetro (mm)s
volatile uint32_t revolutionCount = 0;

void anemometerInterruptHandler(void *arg) { revolutionCount++; }

void anemometerTask(void *pvParameters) {
  uint32_t previousCount = 0;

  while (1) {
    uint32_t currentCount = revolutionCount;
    uint32_t revolutionDiff = currentCount - previousCount;
    uint32_t rpm = (revolutionDiff * 60) / (period / 1000);
    windSpeed = ((4 * pi * radius * rpm) / 60) / 1000;

    ESP_LOGI(TAG, "Velocidade do vento: %.2f m/s", windSpeed);

    previousCount = currentCount;

    vTaskDelay(pdMS_TO_TICKS(delaytime));
  }
}

esp_err_t init_windspeed_sensor() {
  gpio_set_direction(ANEMOMETER_PIN, GPIO_MODE_INPUT);
  gpio_set_intr_type(ANEMOMETER_PIN, GPIO_INTR_POSEDGE);
  gpio_install_isr_service(0);
  gpio_isr_handler_add(ANEMOMETER_PIN, anemometerInterruptHandler, NULL);

  xTaskCreate(anemometerTask, "anemometer_task", configMINIMAL_STACK_SIZE * 8,
              NULL, 10, NULL);

  return ESP_OK;
}
