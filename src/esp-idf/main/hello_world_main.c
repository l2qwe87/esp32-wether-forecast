/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define BLINK_GPIO 8

static const char *TAG = "BLINK_EXAMPLE";

void app_main(void)
{
    ESP_LOGI(TAG, "LED blink example started for ESP32-C3");
    
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    while(1) {
        ESP_LOGI(TAG, "Turning LED ON");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        
        ESP_LOGI(TAG, "Turning LED OFF");
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
