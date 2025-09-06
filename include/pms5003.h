#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define PMS5003_OK 0
#define PMS5003_ERROR 1

typedef struct {
    int uart_port;
    int uart_rx_ionum;
    int uart_tx_ionum;
} pms5003_config_t;

typedef struct {
    int uart_port;
    QueueHandle_t uart_queue;
} pms5003_device_t;

typedef pms5003_device_t* pms5003_handle_t;

typedef struct {
    uint16_t pm1;
    uint16_t pm2;
    uint16_t pm10;
} pms5003_data_t;

int pms5003_init(pms5003_handle_t, pms5003_config_t*);

int pms5003_get(pms5003_handle_t, pms5003_data_t*);
