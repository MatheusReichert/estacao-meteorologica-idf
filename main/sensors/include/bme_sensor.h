#pragma once

#include "esp_err.h"
#include <bme680.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>

#define PORT 0
#if defined(CONFIG_EXAMPLE_I2C_ADDRESS_0)
#define ADDR BME680_I2C_ADDR_0
#else
#define ADDR BME680_I2C_ADDR_1 // Adicione um valor padrão para o endereço
#endif

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM // Adicione um valor padrão para o número da CPU
#endif
typedef struct bmp_values {
  float temperature; //!< temperature in degree C        (Invalid value -327.68)
  float pressure;    //!< barometric pressure in hPascal (Invalid value 0.0)
  float humidity;    //!< relative humidity in %         (Invalid value 0.0)
  float gas_resistance; //!< gas resistance in Ohm          (Invalid value 0.0)
} bme680_values;

extern bme680_values bmp_values;

esp_err_t bme_init(void);
void bme_read_task(void *pvParameters);
