#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "driver/uart.h"

#define PMS5003_OK 0
#define PMS5003_ERROR 1

typedef enum {
    PMS5003_MODE_NORMAL,
    PMS5003_MODE_SLEEP,
} pms5003_mode_t;

typedef struct {
    int uart_port;
    QueueHandle_t uart_queue;
    int mode_ionum;
} pms5003_device_t;

typedef pms5003_device_t* pms5003_handle_t;

int pms5003_init(pms5003_handle_t, int, int, int, int);
int pms5003_set_mode(pms5003_handle_t, pms5003_mode_t);

int pms5003_get_pm(pms5003_handle_t, uint16_t*, uint16_t*, uint16_t*);
