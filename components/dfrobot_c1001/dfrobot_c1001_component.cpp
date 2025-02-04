#include "dfrobot_c1001_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

void DFRobotC1001Component::setup() {
  ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");
  // Initialize the sensor using 'this' (our UARTDevice) as the stream.
  sensor_ = DFRobot_HumanDetection(this);
  if (!sensor_.begin()) {
    ESP_LOGE(TAG, "Failed to initialize DFRobot C1001 Sensor!");
  } else {
    ESP_LOGI(TAG, "DFRobot C1001 Sensor initialized successfully.");
  }
}

void DFRobotC1001Component::loop() {
  // Continuously process incoming UART data.
  while (available()) {
    uint8_t c;
    read_byte(&c);
    sensor_.getData(c);
  }
}

void DFRobotC1001Component::update() {
  // Update at most once per second (adjust as needed).
  uint32_t now = millis();
  if (now - last_read_time_ < 1000) return;
  last_read_time_ = now;

  ESP_LOGI(TAG, "Updating DFRobot C1001 sensor data...");

  // Read sensor values.
  int presence = sensor_.smHumanData(DFRobot_HumanDetection::eHumanPresence);
  int movement = sensor_.smHumanData(DFRobot_HumanDetection::eHumanMovement);
  int fall_state = sensor_.getFallData(DFRobot_HumanDetection::eFallState);
  int residency_state = sensor_.getStaticResidencyTime();

  ESP_LOGI(TAG, "Presence: %d, Movement: %d, Fall: %d, Residency: %d",
           presence, movement, fall_state, residency_state);

  // Publish each sensor's state if configured.
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
