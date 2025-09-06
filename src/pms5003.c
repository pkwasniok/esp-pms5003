#include "pms5003.h"

#include "driver/uart.h"

int pms5003_init(pms5003_handle_t device, pms5003_config_t* config) {
    device->uart_port = config->uart_port;

    ESP_ERROR_CHECK(uart_driver_install(device->uart_port, 1024, 1024, 10, &(device->uart_queue), 0));

    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    ESP_ERROR_CHECK(uart_param_config(device->uart_port, &uart_config));

    ESP_ERROR_CHECK(uart_set_pin(device->uart_port, config->uart_tx_ionum, config->uart_rx_ionum, -1, -1));

    return PMS5003_OK;
}

int pms5003_get(pms5003_handle_t device, pms5003_data_t* data) {
    int length = 0;

    while (length == 0)
        uart_get_buffered_data_len(device->uart_port, (size_t*)&length);

    uint8_t buffer[128];
    length = uart_read_bytes(device->uart_port, buffer, length, 100);

    if (length < 32) {
        return PMS5003_ERROR;
    }

    if (buffer[0] != 0x42 || buffer[1] != 0x4D) {
        return PMS5003_ERROR;
    }

    uint16_t received_checksum = (buffer[30] << 8) | buffer[31];

    uint16_t calculated_checksum = 0;
    for (int i = 0; i < 30; i++)
        calculated_checksum += buffer[i];

    if (received_checksum != calculated_checksum) {
        return PMS5003_ERROR;
    }

    data->pm1 = (buffer[4] << 8) | buffer[5];
    data->pm2 = (buffer[6] << 8) | buffer[7];
    data->pm10 = (buffer[8] << 8) | buffer[9];

    return PMS5003_OK;
}
