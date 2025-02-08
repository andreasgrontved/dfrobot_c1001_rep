#include "dfrobot_c1001_component.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

void DFRobotC1001Component::setup() {
  ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");

  // Reset the sensor (according to the library)
  if (sensor_.sensorRet() != 0) {
    ESP_LOGW(TAG, "Sensor reset failed");
  } else {
    ESP_LOGI(TAG, "Sensor reset successful");
  }
  delay(1000);  // You might wish to adjust this delay

  // Initialize the sensor.
  if (sensor_.begin() != 0) {
    ESP_LOGE(TAG, "Sensor initialization failed");
  } else {
    ESP_LOGI(TAG, "Sensor initialization successful");
  }

  // Set the sensor to falling detection mode.
  if (sensor_.configWorkMode(DFRobot_HumanDetection::eFallingMode) != 0) {
    ESP_LOGE(TAG, "Failed to set sensor work mode");
  } else {
    ESP_LOGI(TAG, "Sensor work mode set to falling mode");
  }
}

void DFRobotC1001Component::update() {
  ESP_LOGI(TAG, "Reading DFRobot C1001 sensor data...");

  // Retrieve sensor values using the library's methods.
  uint16_t presence = sensor_.smHumanData(DFRobot_HumanDetection::eHumanPresence);
  uint16_t movement = sensor_.smHumanData(DFRobot_HumanDetection::eHumanMovement);
  uint16_t fall     = sensor_.getFallData(DFRobot_HumanDetection::eFallState);
  uint16_t residency = sensor_.getFallData(DFRobot_HumanDetection::estaticResidencyState);

  ESP_LOGI(TAG, "Presence: %d, Movement: %d, Fall: %d, Residency: %d",
           presence, movement, fall, residency);

  if (human_presence_ != nullptr)
    human_presence_->publish_state(presence);
  if (human_movement_ != nullptr)
    human_movement_->publish_state(movement);
  if (fall_state_ != nullptr)
    fall_state_->publish_state(fall);
  if (residency_state_ != nullptr)
    residency_state_->publish_state(residency);
}

}  // namespace dfrobot_c1001
}  // namespace esphome
