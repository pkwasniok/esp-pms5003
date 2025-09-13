#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_err.h"

#include "pms5003.h"

#define GPIO_TX    39
#define GPIO_RX    37
#define GPIO_SLEEP 35

void app_main(void) {
    pms5003_device_t pms5003;

    ESP_ERROR_CHECK(pms5003_init(&pms5003, UART_NUM_0, GPIO_TX, GPIO_RX, GPIO_SLEEP));

    while (1) {
        uint16_t pm1, pm2, pm10;

        if (pms5003_get_pm(&pms5003, &pm1, &pm2, &pm10) == PMS5003_OK) {
            printf("pm1.0: %d ug/m^3\n", pm1);
            printf("pm2.5: %d ug/m^3\n", pm2);
            printf("pm10.0: %d ug/m^3\n", pm10);
        } else {
            printf("Unable to read PMS5003!\n");
        }

        printf("\n");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
