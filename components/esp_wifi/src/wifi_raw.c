#include "sdkconfig.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_private/wifi.h"
#include <string.h>
#ifdef CONFIG_ESP_WIFI_NAN_ENABLE
#include "apps_private/wifi_apps_private.h"
#endif

#include "esp_wifi_raw.h"

static const char* TAG = "wifi_raw";

static esp_wifi_raw_cb_t rxcb = NULL;
static void* rxcbUserData = NULL;


esp_err_t handle_raw(void *buffer, uint16_t len, void *eb) {
    if (rxcb) {
        rxcb(buffer, len, rxcbUserData);
    }

    if (eb) {
        // I guess we free this one because it points to `buffer`?
        esp_wifi_internal_free_rx_buffer(eb);
    }

    return ESP_OK;
}

esp_err_t esp_wifi_raw_start(wifi_interface_t iface) {
    esp_err_t err = esp_wifi_internal_reg_rxcb(iface, handle_raw);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Can't set rx callback.");
        return err;
    }

    return ESP_OK;
}

esp_err_t esp_wifi_raw_set_cb(esp_wifi_raw_cb_t cb, void *userData) {
    rxcbUserData = userData; // kind of a race. :(
    rxcb = cb;

    return ESP_OK;
}

esp_err_t esp_wifi_raw_transmit(wifi_interface_t iface, void *buffer, size_t len) {
    return esp_wifi_internal_tx(iface, buffer, len);
}

