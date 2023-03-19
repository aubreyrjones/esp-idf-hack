#pragma once
#include "sdkconfig.h"
#include "esp_wifi_types.h"

#include "esp_err.h"

typedef void (*esp_wifi_raw_cb_t)(void *buffer, size_t len);

// esp_err_t esp_wifi_raw_set_handlers();
// esp_err_t esp_wifi_raw_clear_handlers();


esp_err_t esp_wifi_raw_set_cb(wifi_interface_t iface, esp_wifi_raw_cb_t cb);

// esp_err_t esp_wifi_raw_free_rx_buffer(void *buffer);

esp_err_t esp_wifi_raw_transmit(wifi_interface_t iface, void *buffer, size_t len);