#include "dfrobot_c1001_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

void DFRobotC1001Component::setup() {
  ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");
  // Reset sensor by sending a specific command. Adjust the command if needed.
  this->write_str("\xA5\x55\x00\x00\xAA");
  delay(500);  // Wait for sensor to reset
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

  if (buffer.size() < 8) {
    ESP_LOGW(TAG, "Not enough data received from sensor");
    return;
  }

  // Parse the first 4 bytes as sensor values. Adjust indices if your protocol differs.
  uint8_t presence = buffer[0];
  uint8_t movement = buffer[1];
  uint8_t fall     = buffer[2];
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

void DFRobotC1001Component::dump_config() {
  ESP_LOGCONFIG(TAG, "DFRobot C1001 Sensor:");
  LOG_PIN("  TX Pin: ", this->get_tx_pin());
  LOG_PIN("  RX Pin: ", this->get_rx_pin());
  if (human_presence_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Human Presence Sensor:");
    human_presence_->dump_config();
  }
  if (human_movement_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Human Movement Sensor:");
    human_movement_->dump_config();
  }
  if (fall_state_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Fall State Sensor:");
    fall_state_->dump_config();
  }
  if (residency_state_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Residency State Sensor:");
    residency_state_->dump_config();
  }
}

// Setter implementations:
void DFRobotC1001Component::set_human_presence(sensor::Sensor *s) {
  human_presence_ = s;
}

void DFRobotC1001Component::set_human_movement(sensor::Sensor *s) {
  human_movement_ = s;
}

void DFRobotC1001Component::set_fall_state(sensor::Sensor *s) {
  fall_state_ = s;
}

void DFRobotC1001Component::set_residency_state(sensor::Sensor *s) {
  residency_state_ = s;
}

}  // namespace dfrobot_c1001
}  // namespace esphome

