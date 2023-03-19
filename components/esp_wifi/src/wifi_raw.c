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

#if 0
    if (eb) {
        ESP_LOGI(TAG, "eb: %lu : %*.*s", eb, len, len, eb);
        //spam_out(eb, len);
    }
#endif

    if (eb) {
        // I guess we free this one because it points to `buffer`?
        esp_wifi_internal_free_rx_buffer(eb);
    }

    return ESP_OK;
}


esp_err_t esp_wifi_raw_set_cb(wifi_interface_t iface, esp_wifi_raw_cb_t cb, void *userData) {
    bool firstTime = !rxcb;

    rxcbUserData = userData; // kind of a race. :(
    rxcb = cb;

    if (firstTime) {
        esp_err_t err = esp_wifi_internal_reg_rxcb(iface, handle_raw);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Can't set rx callback.");
            return err;
        }
    }

    return ESP_OK;
}

esp_err_t esp_wifi_raw_transmit(wifi_interface_t iface, void *buffer, size_t len) {
    return esp_wifi_internal_tx(iface, buffer, len);
}

// static bool wifi_default_handlers_set = false;

// static esp_err_t esp_wifi_raw_clear_handlers(void)
// {
//     esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_STA_START, wifi_default_action_sta_start);
//     esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_STA_STOP, wifi_default_action_sta_stop);
//     esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, wifi_default_action_sta_connected);
//     esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, wifi_default_action_sta_disconnected);
// #ifdef CONFIG_ESP_WIFI_SOFTAP_SUPPORT
//     esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_AP_START, wifi_default_action_ap_start);
//     esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_AP_STOP, wifi_default_action_ap_stop);
// #endif
//     //esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_default_action_sta_got_ip);
// #ifdef CONFIG_ESP_WIFI_NAN_ENABLE
//     esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_NAN_STARTED, wifi_default_action_nan_started);
//     esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_NAN_STOPPED, wifi_default_action_nan_stopped);
// #endif
//     esp_unregister_shutdown_handler((shutdown_handler_t)esp_wifi_stop);
//     wifi_default_handlers_set = false;
//     return ESP_OK;
// }

// /**
//  * @brief Set default handlers
//  */
// static esp_err_t esp_wifi_raw_set_handlers(void)
// {
//     if (wifi_default_handlers_set) {
//         return ESP_OK;
//     }
//     esp_err_t err;

//     err = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_START, wifi_default_action_sta_start, NULL);
//     if (err != ESP_OK) {
//         goto fail;
//     }

//     err = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_STOP, wifi_default_action_sta_stop, NULL);
//     if (err != ESP_OK) {
//         goto fail;
//     }

//     err = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, wifi_default_action_sta_connected, NULL);
//     if (err != ESP_OK) {
//         goto fail;
//     }

//     err = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, wifi_default_action_sta_disconnected, NULL);
//     if (err != ESP_OK) {
//         goto fail;
//     }

// #ifdef CONFIG_ESP_WIFI_SOFTAP_SUPPORT
//     err = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_AP_START, wifi_default_action_ap_start, NULL);
//     if (err != ESP_OK) {
//         goto fail;
//     }

//     err = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_AP_STOP, wifi_default_action_ap_stop, NULL);
//     if (err != ESP_OK) {
//         goto fail;
//     }
// #endif

// #ifdef CONFIG_ESP_WIFI_NAN_ENABLE
//     err = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_NAN_STARTED, wifi_default_action_nan_started, NULL);
//     if (err != ESP_OK) {
//         goto fail;
//     }

//     err = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_NAN_STOPPED, wifi_default_action_nan_stopped, NULL);
//     if (err != ESP_OK) {
//         goto fail;
//     }
// #endif

//     err = esp_register_shutdown_handler((shutdown_handler_t)esp_wifi_stop);
//     if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
//         goto fail;
//     }
//     wifi_default_handlers_set = true;
//     return ESP_OK;

// fail:
//     clear_default_wifi_handlers();
//     return err;
// }