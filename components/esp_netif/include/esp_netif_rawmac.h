#pragma once
#include "sdkconfig.h"

#include "esp_netif_types.h"

typedef void (*esp_netif_raw_cb_t)(void *buffer, size_t len);

esp_netif_t* esp_netif_raw_create(esp_netif_iodriver_handle driver);

esp_err_t esp_netif_raw_set_rx_cb(esp_netif_raw_cb_t cb);
