#include "dfrobot_c1001_component.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

void DFRobotC1001Component::setup() {
    ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");
    this->write_str("\xA5\x55\x00\x00\xAA"); // Reset sensor
    delay(500);
    ESP_LOGI(TAG, "Sensor initialization complete.");
}

void DFRobotC1001Component::update() {
    ESP_LOGI(TAG, "Reading DFRobot C1001 sensor data...");

    std::vector<uint8_t> buffer;
    while (this->available()) {
        buffer.push_back(this->read());
    }

    if (buffer.empty()) {
        ESP_LOGW(TAG, "No data received from sensor");
        return;
    }

    ESP_LOGI(TAG, "Raw data received: %d bytes", buffer.size());
    for (size_t i = 0; i < buffer.size(); i++) {
        ESP_LOGI(TAG, "Byte %d: 0x%02X", i, buffer[i]);
    }

    // Ensure at least 8 bytes are available
    if (buffer.size() < 8) {
        ESP_LOGW(TAG, "Not enough data received from sensor");
        return;
    }

    uint8_t presence = buffer[0];
    uint8_t movement = buffer[1];
    uint8_t fall = buffer[2];
    uint8_t residency = buffer[3];

    ESP_LOGI(TAG, "Presence: %d, Movement: %d, Fall: %d, Residency: %d",
             presence, movement, fall, residency);

    if (this->human_presence_ != nullptr)
        this->human_presence_->publish_state(presence);
    if (this->human_movement_ != nullptr)
        this->human_movement_->publish_state(movement);
    if (this->fall_state_ != nullptr)
        this->fall_state_->publish_state(fall);
    if (this->residency_state_ != nullptr)
        this->residency_state_->publish_state(residency);
}

}  // namespace dfrobot_c1001
}  // namespace esphome
