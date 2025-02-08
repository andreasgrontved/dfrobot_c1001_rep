#include "dfrobot_c1001_component.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

void DFRobotC1001Component::setup() {
  ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");

  // Reset the sensor (as recommended by the library)
  if (sensor_.sensorRet() != 0) {
    ESP_LOGW(TAG, "Sensor reset failed");
  } else {
    ESP_LOGI(TAG, "Sensor reset successful");
  }
  
  // (You may wish to reduce this delay from 10 s to something shorter if possible)
  delay(1000);

  // Initialize the sensor – the library’s begin() waits for a long startup time.
  if (sensor_.begin() != 0) {
    ESP_LOGE(TAG, "Sensor initialization failed");
  } else {
    ESP_LOGI(TAG, "Sensor initialization successful");
  }

  // Set sensor to falling detection mode.
  if (sensor_.configWorkMode(DFRobot_HumanDetection::eFallingMode) != 0) {
    ESP_LOGE(TAG, "Failed to set sensor work mode");
  } else {
    ESP_LOGI(TAG, "Sensor work mode set to falling mode");
  }
}

void DFRobotC1001Component::update() {
  ESP_LOGI(TAG, "Reading DFRobot C1001 sensor data...");

  // Get sensor values using the library functions.
  // (In the Arduino sample, these functions return 0/1 or data values.)
  uint16_t presence = sensor_.smHumanData(DFRobot_HumanDetection::eHumanPresence);
  uint16_t movement = sensor_.smHumanData(DFRobot_HumanDetection::eHumanMovement);
  uint16_t fall     = sensor_.getFallData(DFRobot_HumanDetection::eFallState);
  uint16_t residency = sensor_.getFallData(DFRobot_HumanDetection::estaticResidencyState);

  ESP_LOGI(TAG, "Presence: %d, Movement: %d, Fall: %d, Residency: %d", presence, movement, fall, residency);

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
