#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "driver/uart.h"

#define PMS5003_OK 0
#define PMS5003_ERROR 1

typedef struct {
    int uart_port;
    QueueHandle_t uart_queue;
} pms5003_device_t;

typedef pms5003_device_t* pms5003_handle_t;

int pms5003_init(pms5003_handle_t, int, int, int);

int pms5003_read(pms5003_handle_t, uint16_t*, uint16_t*, uint16_t*);
