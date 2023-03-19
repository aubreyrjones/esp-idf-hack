#include <string.h>

static const char *TAG = "esp_netif_rawmac";

#include "esp_netif.h"
#include "esp_netif_private.h"
#include "esp_netif_types.h"
#include "esp_netif_rawmac.h"

#if CONFIG_ESP_NETIF_RAW
#include "esp_log.h"

struct esp_netif_obj {
    uint8_t mac[6];

    esp_netif_raw_cb_t rxcb;

    // io driver related
    void* driver_handle;
    esp_err_t (*driver_transmit)(void *h, void *buffer, size_t len);
    esp_err_t (*driver_transmit_wrap)(void *h, void *buffer, size_t len, void *pbuf);
    void (*driver_free_rx_buffer)(void *h, void* buffer);
};

esp_netif_t raw_if = {
    {0, 0, 0, 0, 0, 0},
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};


esp_err_t esp_netif_set_driver_config(esp_netif_t *esp_netif,
                                      const esp_netif_driver_ifconfig_t *driver_config)
{
    if (esp_netif == NULL || driver_config == NULL) {
        return ESP_ERR_ESP_NETIF_INVALID_PARAMS;
    }
    esp_netif->driver_handle = driver_config->handle;
    esp_netif->driver_transmit = driver_config->transmit;
    esp_netif->driver_transmit_wrap = driver_config->transmit_wrap;
    esp_netif->driver_free_rx_buffer = driver_config->driver_free_rx_buffer;
    return ESP_OK;
}


esp_err_t esp_netif_attach(esp_netif_t *esp_netif, esp_netif_iodriver_handle driver_handle)
{
    esp_netif_driver_base_t *base_driver = driver_handle;

    esp_netif->driver_handle = driver_handle;
    if (base_driver->post_attach) {
        esp_err_t ret = base_driver->post_attach(esp_netif, driver_handle);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Post-attach callback of driver(%p) failed with %d", driver_handle, ret);
            return ESP_ERR_ESP_NETIF_DRIVER_ATTACH_FAILED;
        }
    }
    return ESP_OK;
}

// esp_err_t esp_netif_raw_receive(esp_netif_t *esp_netif, void *buffer, size_t len, void *eb) {
//     if (raw_if.rxcb) {
//         raw_if.rxcb(buffer, len);
//     }
//     esp_netif->driver_free_rx_buffer(esp_netif, buffer);
// }

esp_netif_t* esp_netif_raw_create(esp_netif_iodriver_handle driver) {
    esp_netif_attach(&raw_if, driver);

    return &raw_if;
}

esp_err_t esp_netif_raw_set_rx_cb(esp_netif_raw_cb_t cb) {
    raw_if.rxcb = cb;

    return ESP_OK;
}


#endif