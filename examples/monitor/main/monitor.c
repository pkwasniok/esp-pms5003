#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#include "pms5003.h"

#define GPIO_TX 39
#define GPIO_RX 37

void app_main(void) {
    pms5003_device_t pms5003;

    pms5003_config_t pms5003_config = {
        .uart_port = UART_NUM_0,
        .uart_tx_ionum = GPIO_TX,
        .uart_rx_ionum = GPIO_RX,
    };

    if (pms5003_init(&pms5003, &pms5003_config) != PMS5003_OK) {
        printf("Error!\n");
    }

    while (1) {
        pms5003_data_t particles;

        if (pms5003_get(&pms5003, &particles) != PMS5003_OK) {
            printf("Error\n");
        }

        printf("pm 1.0: %d ug/m^3\n", particles.pm1);
        printf("pm 2.5: %d ug/m^3\n", particles.pm2);
        printf("pm 10.0: %d ug/m^3\n", particles.pm10);
        printf("\n");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
