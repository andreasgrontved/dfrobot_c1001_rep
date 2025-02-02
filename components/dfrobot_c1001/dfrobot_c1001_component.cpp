#include "dfrobot_c1001_component.h"

namespace esphome {
namespace dfrobot_c1001 {

void DFRobotC1001Component::setup() {
  ESP_LOGI("dfrobot_c1001", "Initializing DFROBOT C1001 sensor...");

  // Initialize the sensor (begin() returns 0 on success).
  if (sensor_.begin() != 0) {
    ESP_LOGE("dfrobot_c1001", "Sensor initialization failed!");
  } else {
    ESP_LOGI("dfrobot_c1001", "Sensor initialized successfully");
  }

  // Configure working mode: e.g., set falling mode.
  if (sensor_.configWorkMode(DFRobot_HumanDetection::eFallingMode) != 0) {
    ESP_LOGE("dfrobot_c1001", "Failed to set work mode!");
  } else {
    ESP_LOGI("dfrobot_c1001", "Work mode set to falling mode");
  }

  // Disable LED outputs (1 means off in this sensor’s configuration).
  sensor_.configLEDLight(DFRobot_HumanDetection::eFALLLed, 1);
  sensor_.configLEDLight(DFRobot_HumanDetection::eHPLed, 1);

  // Additional sensor configuration:
  sensor_.dmInstallHeight(270);                      // Installation height (cm)
  sensor_.dmFallTime(5);                             // Fall time (seconds)
  sensor_.dmUnmannedTime(1);                         // Unmanned time (seconds)
  sensor_.dmFallConfig(DFRobot_HumanDetection::eResidenceTime, 200);  // Dwell time (seconds)
  sensor_.dmFallConfig(DFRobot_HumanDetection::eFallSensitivityC, 3); // Fall sensitivity (0–3)

  // Reset the sensor (required after configuration).
  sensor_.sensorRet();
}

void DFRobotC1001Component::update() {
  // Poll data from the sensor.
  uint16_t presence   = sensor_.smHumanData(DFRobot_HumanDetection::eHumanPresence);
  uint16_t movement   = sensor_.smHumanData(DFRobot_HumanDetection::eHumanMovement);
  uint16_t body_move  = sensor_.smHumanData(DFRobot_HumanDetection::eHumanMovingRange);
  uint16_t fall_state = sensor_.getFallData(DFRobot_HumanDetection::eFallState);
  uint16_t residency  = sensor_.getFallData(DFRobot_HumanDetection::estaticResidencyState);

  ESP_LOGI("dfrobot_c1001", "Presence: %d, Movement: %d, Body move: %d, Fall: %d, Residency: %d",
           presence, movement, body_move, fall_state, residency);

  // Publish the sensor readings.
  human_presence_sensor->publish_state(presence);
  human_movement_sensor->publish_state(movement);
  fall_state_sensor->publish_state(fall_state);
  residency_state_sensor->publish_state(residency);
}

}  // namespace dfrobot_c1001
}  // namespace esphome
