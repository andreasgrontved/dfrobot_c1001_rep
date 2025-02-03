#include "dfrobot_c1001_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

void DFRobotC1001Component::setup() {
    ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");

    sensor_ = DFRobot_HumanDetection(this); // Use 'this' to provide UARTDevice capabilities

    if (!sensor_.begin()) {  
        ESP_LOGE(TAG, "Failed to initialize DFRobot C1001 Sensor!");
        return;
    }
    ESP_LOGI(TAG, "DFRobot C1001 Sensor initialized successfully.");
}

void DFRobotC1001Component::loop() {
    while (available()) {  // Check for available UART data
        uint8_t c;
        read_byte(&c);  // Read byte from UART
        sensor_.getData(c);  // Process the data in sensor object
    }
}

void DFRobotC1001Component::update() {
    uint32_t now = millis();
    if (now - last_read_time_ < 1000) return; // Ensure at least 1 second interval
    last_read_time_ = now;

    ESP_LOGI(TAG, "Updating Sensor Data...");

    int presence = sensor_.smHumanData(DFRobot_HumanDetection::eHumanPresence);
    int movement = sensor_.smHumanData(DFRobot_HumanDetection::eHumanMovement);
    int fall_state = sensor_.getFallData(DFRobot_HumanDetection::eFallState);
    int residency_state = sensor_.getStaticResidencyTime();

    ESP_LOGI(TAG, "Presence: %d, Movement: %d, Fall: %d, Residency: %d", 
             presence, movement, fall_state, residency_state);

    // Ensure values are valid before publishing
    if (presence >= 0 && presence <= 1 && human_presence_sensor) {
        human_presence_sensor->publish_state(presence);
    }
    if (movement >= 0 && movement <= 100 && human_movement_sensor) {
        human_movement_sensor->publish_state(movement);
    }
    if (fall_state >= 0 && fall_state <= 1 && fall_state_sensor) {
        fall_state_sensor->publish_state(fall_state);
    }
    if (residency_state >= 0 && residency_state <= 60000 && residency_state_sensor) {
        residency_state_sensor->publish_state(residency_state);
    }
}

}  // namespace dfrobot_c1001
}  // namespace esphome
