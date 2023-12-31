/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include <device.h>
#include <esp_matter.h>
#include <led_driver.h>

#include <app_priv.h>

using namespace chip::app::Clusters;
using namespace esp_matter;

static const char *TAG = "app_driver";
extern uint16_t light_endpoint_id;

static void app_driver_button_toggle_cb(void *arg, void *data)
{
    ESP_LOGI(TAG, "Toggle button pressed");
    uint16_t endpoint_id = light_endpoint_id;
    uint32_t cluster_id = OnOff::Id;
    uint32_t attribute_id = OnOff::Attributes::OnOff::Id;

    node_t *node = node::get();
    endpoint_t *endpoint = endpoint::get(node, endpoint_id);
    cluster_t *cluster = cluster::get(endpoint, cluster_id);
    attribute_t *attribute = attribute::get(cluster, attribute_id);

    esp_matter_attr_val_t val = esp_matter_invalid(NULL);
    attribute::get_val(attribute, &val);
    val.val.b = !val.val.b;
    attribute::update(endpoint_id, cluster_id, attribute_id, &val);
}

esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val)
{
    // Seems to receive updates from the clients in order to update the hardware accordingly.
    // For the temperature measurement, there should be no need for attribute update.

    esp_err_t err = ESP_OK;
    if (endpoint_id == light_endpoint_id) {
        // led_driver_handle_t handle = (led_driver_handle_t)driver_handle;
        // if (cluster_id == OnOff::Id) {
        //     if (attribute_id == OnOff::Attributes::OnOff::Id) {
        //         err = app_driver_light_set_power(handle, val);
        //     }
        // } else if (cluster_id == LevelControl::Id) {
        //     if (attribute_id == LevelControl::Attributes::CurrentLevel::Id) {
        //         err = app_driver_light_set_brightness(handle, val);
        //     }
        // } else if (cluster_id == ColorControl::Id) {
        //     if (attribute_id == ColorControl::Attributes::CurrentHue::Id) {
        //         err = app_driver_light_set_hue(handle, val);
        //     } else if (attribute_id == ColorControl::Attributes::CurrentSaturation::Id) {
        //         err = app_driver_light_set_saturation(handle, val);
        //     } else if (attribute_id == ColorControl::Attributes::ColorTemperatureMireds::Id) {
        //         err = app_driver_light_set_temperature(handle, val);
        //     }
        // }
    }
    return err;
}

app_driver_handle_t app_driver_button_init()
{
    /* Initialize button */
    button_config_t config = button_driver_get_config();
    button_handle_t handle = iot_button_create(&config);
    iot_button_register_cb(handle, BUTTON_PRESS_DOWN, app_driver_button_toggle_cb, NULL);
    return (app_driver_handle_t)handle;
}
