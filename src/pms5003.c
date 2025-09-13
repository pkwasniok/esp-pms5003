#include "pms5003.h"

uint16_t _pms5003_checksum(uint8_t* buffer, int length) {
    uint16_t checksum = 0;

    for (int i = 0; i < length; i++)
        checksum += buffer[i];

    return checksum;
}

int _pms5003_write(pms5003_handle_t device, uint8_t command, uint16_t data) {
    uint8_t buffer[7];

    buffer[0] = 0x42;
    buffer[1] = 0x4D;
    buffer[2] = command;
    buffer[3] = (data >> 8);
    buffer[4] = data;

    uint16_t checksum = _pms5003_checksum(buffer, 5);

    buffer[5] = checksum >> 8;
    buffer[6] = checksum;

    if (uart_write_bytes(device->uart_port, buffer, 7) != 7)
        return PMS5003_ERROR;

    if (uart_wait_tx_done(device->uart_port, 1000 / portTICK_PERIOD_MS) != ESP_OK)
        return PMS5003_ERROR;

    return PMS5003_OK;
}

int pms5003_init(pms5003_handle_t device, int uart_port, int uart_tx_ionum, int uart_rx_ionum) {
    device->uart_port = uart_port;

    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .stop_bits = UART_STOP_BITS_1,
        .parity = UART_PARITY_DISABLE,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    if (uart_param_config(device->uart_port, &uart_config) != ESP_OK)
        return PMS5003_ERROR;

    if (uart_set_pin(device->uart_port, uart_tx_ionum, uart_rx_ionum, -1, -1) != ESP_OK)
        return PMS5003_ERROR;

    if (uart_driver_install(device->uart_port, 511, 512, 10, &device->uart_queue, 0) != ESP_OK)
        return PMS5003_ERROR;

    if (_pms5003_write(device, 0xE1, 0x0000) != PMS5003_OK)
        return PMS5003_ERROR;

    return PMS5003_OK;
}

int pms5003_read(pms5003_handle_t device, uint16_t* pm1, uint16_t* pm2, uint16_t* pm10) {
    uint8_t buffer[32];

    if (uart_flush(device->uart_port) != ESP_OK)
        return PMS5003_ERROR;

    if (_pms5003_write(device, 0xE2, 0x0000) != PMS5003_OK)
        return PMS5003_ERROR;

    if (uart_read_bytes(device->uart_port, buffer, 32, 1000 / portTICK_PERIOD_MS) != 32)
        return PMS5003_ERROR;

    if (buffer[0] != 0x42 || buffer[1] != 0x4D)
        return PMS5003_ERROR;

    uint16_t checksum_received = (buffer[30] << 8) | buffer[31];
    if (checksum_received != _pms5003_checksum(buffer, 30))
        return PMS5003_ERROR;

    (*pm1) = (buffer[4] << 8) | buffer[5];
    (*pm2) = (buffer[6] << 8) | buffer[7];
    (*pm10) = (buffer[8] << 8) | buffer[9];

    return PMS5003_OK;
}
