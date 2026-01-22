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
#include "esp_log.h"

#define INTERVAL 400
#define WAIT vTaskDelay(INTERVAL)

static const char *TAG = "ST7789";

// You have to set these CONFIG value using menuconfig.

#define CFG_WIDTH  170
#define CFG_HEIGHT 320
#define CFG_MOSI_GPIO 6
#define CFG_SCLK_GPIO 4
#define CFG_CS_GPIO -1
#define CFG_DC_GPIO 1
#define CFG_RESET_GPIO 2
#define CFG_BL_GPIO 3

#define CFG_OFFSETX 35 
#define CFG_OFFSETY 0

#define BLINK_GPIO 8

static const char *TAG = "BLINK_EXAMPLE";

TickType_t FillRectTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	lcdFillScreen(dev, CYAN);

	uint16_t red;
	uint16_t green;
	uint16_t blue;
	srand( (unsigned int)time( NULL ) );
	for(int i=1;i<100;i++) {
		red=rand()%255;
		green=rand()%255;
		blue=rand()%255;
		color=rgb565(red, green, blue);
		uint16_t xpos=rand()%width;
		uint16_t ypos=rand()%height;
		uint16_t size=rand()%(width/5);
		lcdDrawFillRect(dev, xpos, ypos, xpos+size, ypos+size, color);
	}
	lcdDrawFinish(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

void ST7789(void *pvParameters)
{
    	
	TFT_t dev;

	// Change SPI Clock Frequency
	//spi_clock_speed(40000000); // 40MHz
	//spi_clock_speed(60000000); // 60MHz

	spi_master_init(&dev, CFG_MOSI_GPIO, CFG_SCLK_GPIO, CFG_CS_GPIO, CFG_DC_GPIO, CFG_RESET_GPIO, CFG_BL_GPIO);
	lcdInit(&dev, CFG_WIDTH, CFG_HEIGHT, CFG_OFFSETX, CFG_OFFSETY);

    // lcdDrawFinish(&dev);
    FillRectTest(&dev, CFG_WIDTH, CFG_HEIGHT);
		WAIT;

    while (1) {
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}

}
void app_main(void)
{
	ESP_LOGI(TAG, "Initializing SPIFFS");

	xTaskCreate(ST7789, "ST7789", 1024*6, NULL, 2, NULL);
}
