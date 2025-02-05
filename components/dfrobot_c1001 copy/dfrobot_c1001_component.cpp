#include "dfrobot_c1001_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

void DFRobotC1001Component::setup() {
  ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");
  
  // Ensure the UART parent is valid and cast it to Stream*
  if (uart_parent_ == nullptr) {
    ESP_LOGE(TAG, "UART component is null!");
    return;
  }

  // Correct instantiation: Pass the UART component as a Stream*
  sensor_ = new DFRobot_HumanDetection(reinterpret_cast<Stream*>(uart_parent_));
  
  if (!sensor_->begin()) {
    ESP_LOGE(TAG, "Failed to initialize DFRobot C1001 Sensor!");
  } else {
    ESP_LOGI(TAG, "DFRobot C1001 Sensor initialized successfully.");
  }
}

void DFRobotC1001Component::update() {
  // Limit updates to at most once per second.
  uint32_t now = millis();
  if (now - last_read_time_ < 1000) return;
  last_read_time_ = now;

  ESP_LOGI(TAG, "Updating DFRobot C1001 sensor data...");

  if (sensor_ == nullptr) {
    ESP_LOGE(TAG, "Sensor is not initialized!");
    return;
  }

  // Query the sensor for data.
  int presence = sensor_->smHumanData(DFRobot_HumanDetection::eHumanPresence);
  int movement = sensor_->smHumanData(DFRobot_HumanDetection::eHumanMovement);
  int fall_state = sensor_->getFallData(DFRobot_HumanDetection::eFallState);
  int residency_state = sensor_->getStaticResidencyTime();

  ESP_LOGI(TAG, "Presence: %d, Movement: %d, Fall: %d, Residency: %d",
           presence, movement, fall_state, residency_state);

  // Publish sensor states if configured.
  if (human_presence_sensor) {
    human_presence_sensor->publish_state(presence);
  }
  if (human_movement_sensor) {
    human_movement_sensor->publish_state(movement);
  }
  if (fall_state_sensor) {
    fall_state_sensor->publish_state(fall_state);
  }
  if (residency_state_sensor) {
    residency_state_sensor->publish_state(residency_state);
  }
}

}  // namespace dfrobot_c1001
}  // namespace esphome
