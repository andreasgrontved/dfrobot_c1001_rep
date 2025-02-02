#include "dfrobot_c1001_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

void DFRobotC1001Component::setup() {
    ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");
    sensor_.begin();  // Initialize the sensor
}

void DFRobotC1001Component::update() {
    ESP_LOGI(TAG, "Updating Sensor Data...");

    // Read sensor data
    int presence = sensor_.smHumanData(eHumanPresence);
    int movement = sensor_.smHumanData(eHumanMovement);
    int fall_state = sensor_.getFallData(eFallState);
    int residency_state = sensor_.getStaticResidencyTime();

    // Publish sensor data
    if (human_presence_sensor != nullptr) {
        human_presence_sensor->publish_state(presence);
    }
    if (human_movement_sensor != nullptr) {
        human_movement_sensor->publish_state(movement);
    }
    if (fall_state_sensor != nullptr) {
        fall_state_sensor->publish_state(fall_state);
    }
    if (residency_state_sensor != nullptr) {
        residency_state_sensor->publish_state(residency_state);
    }
}

}  // namespace dfrobot_c1001
}  // namespace esphome