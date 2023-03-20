#pragma once
#include "sdkconfig.h"
#include "esp_wifi_types.h"

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t esp_wifi_raw_start(wifi_interface_t iface);

typedef void (*esp_wifi_raw_cb_t)(void *buffer, size_t len, void *userData);
esp_err_t esp_wifi_raw_set_cb(esp_wifi_raw_cb_t cb, void *userData);
esp_err_t esp_wifi_raw_transmit(wifi_interface_t iface, void *buffer, size_t len);



#ifdef __cplusplus
}
#endif
