/*
 * XY_MD02.c
 *
 *  Created on: Jan 18, 2025
 *      Author: LattePanda
 */




/*------------------------------------------------------------------------------

	XY-MD02 temperature & humidity sensor driver for ESP32

	This example code is in the Public Domain (or CC0 licensed, at your option.)

	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.

---------------------------------------------------------------------------------*/

#include "xy_md02.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "tasks_common.h"

static const char* TAG = "XY_MD02";

static float temp = 0.0;
static float humi = 0.0;

// == Getters for Temperature and Humidity =======================

float XY_MD02_getHumidity() {
    return humi;
}

float XY_MD02_getTemperature() {
    return temp;
}

// == Error Handler ==============================================

void XY_MD02_errorHandler(int response)
{
    switch (response) {
        case XY_MD02_OK:
            ESP_LOGI(TAG, "OK: Read successful");
            break;
        case XY_MD02_TIMEOUT_ERROR:
            ESP_LOGE(TAG, "Timeout: Sensor did not respond");
            break;
        default:
            ESP_LOGE(TAG, "Error: Unknown response code");
    }
}

// == Read temperature and humidity =============================

int readXY_MD02()
{
    uint8_t request[] = {
        0x01, // Device Address
        0x04, // Function code
        0x00, // Start Address HIGH
        0x01, // Start Address LOW
        0x00, // Quantity HIGH
        0x02, // Quantity LOW
        0x20, // CRC LOW
        0x0B  // CRC HIGH
    };

    uint8_t response[9];

    uart_write_bytes(XY_MD02_UART_NUM, (const char *)request, sizeof(request));
    uart_wait_tx_done(XY_MD02_UART_NUM, 200 / portTICK_PERIOD_MS);

    int len = uart_read_bytes(XY_MD02_UART_NUM, response, sizeof(response), 200 / portTICK_PERIOD_MS);

    if (len < 7 || response[1] != 0x04) {
        return XY_MD02_TIMEOUT_ERROR;
    }

    temp = ((uint16_t)(response[3] << 8) | response[4]) / 100.0f;
    humi = ((uint16_t)(response[5] << 8) | response[6]) / 100.0f;
    return XY_MD02_OK;
}

/**
 * XY_MD02 Sensor task
 */
static void XY_MD02_task(void *pvParameter)
{
    while (1)
    {
        int ret = readXY_MD02();

        XY_MD02_errorHandler(ret);

        if (ret == XY_MD02_OK) {
//            ESP_LOGI(TAG, "Temperature: %.2f°C", XY_MD02_getTemperature());
//            ESP_LOGI(TAG, "Humidity: %.2f%%", XY_MD02_getHumidity());
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void XY_MD02_task_start(void)
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(XY_MD02_UART_NUM, &uart_config);
    uart_set_pin(XY_MD02_UART_NUM, XY_MD02_TX_PIN, XY_MD02_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(XY_MD02_UART_NUM, 256, 0, 0, NULL, 0);

    xTaskCreatePinnedToCore(
        &XY_MD02_task,                // Function implementing the task
        "XY_MD02_task",              // Name of the task
        XY_MD02_TASK_STACK_SIZE,     // Stack size in words
        NULL,                        // Task input parameter
        XY_MD02_TASK_PRIORITY,       // Priority of the task
        NULL,                        // Task handle
        XY_MD02_TASK_CORE_ID         // Core where the task should run
    );
}
