#include "dfrobot_c1001_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

void DFRobotC1001Component::setup() {
  ESP_LOGE(TAG, "Start initialization");
  
  // Initialize the sensor via the library
  while (sensor_.begin() != 0) {
    ESP_LOGE(TAG, "init error!!!");
    delay(1000);
  }
  ESP_LOGE(TAG, "Initialization successful");

  ESP_LOGE(TAG, "Start switching work mode");
  while (sensor_.configWorkMode(DFRobot_HumanDetection::eFallingMode) != 0) {
    ESP_LOGE(TAG, "error!!!");
    delay(1000);
  }
  ESP_LOGE(TAG, "Work mode switch successful");

  // Configure LED lights and other parameters
  sensor_.configLEDLight(DFRobot_HumanDetection::eFALLLed, 1);   // FALL LED on
  sensor_.configLEDLight(DFRobot_HumanDetection::eHPLed, 1);     // HP LED on
  sensor_.dmInstallHeight(270);      // Set installation height to 270 cm
  sensor_.dmFallTime(5);             // Set fall time to 5 seconds
  sensor_.dmUnmannedTime(1);         // Set unmanned time to 1 second
  sensor_.dmFallConfig(DFRobot_HumanDetection::eResidenceTime, 200);  // Set dwell time to 200 s
  sensor_.dmFallConfig(DFRobot_HumanDetection::eFallSensitivityC, 3); // Set fall sensitivity to 3
  sensor_.sensorRet();  // Reset the sensor after configuration

  // Log current configuration from the sensor
  uint8_t work_mode = sensor_.getWorkMode();
  ESP_LOGE(TAG, "Current work mode: %s", 
             (work_mode == 1 ? "Fall detection mode" : 
              (work_mode == 2 ? "Sleep detection mode" : "Read error")));
  
  uint8_t hp_led_state = sensor_.getLEDLightState(DFRobot_HumanDetection::eHPLed);
  ESP_LOGE(TAG, "HP LED status: %s", (hp_led_state == 0 ? "Off" : (hp_led_state == 1 ? "On" : "Read error")));
  
  uint8_t fall_led_state = sensor_.getLEDLightState(DFRobot_HumanDetection::eFALLLed);
  ESP_LOGE(TAG, "FALL status: %s", (fall_led_state == 0 ? "Off" : (fall_led_state == 1 ? "On" : "Read error")));
  
  ESP_LOGE(TAG, "Radar installation height: %d cm", sensor_.dmGetInstallHeight());
  ESP_LOGE(TAG, "Fall duration: %d seconds", sensor_.getFallTime());
  ESP_LOGE(TAG, "Unattended duration: %d seconds", sensor_.getUnmannedTime());
  ESP_LOGE(TAG, "Dwell duration: %d seconds", sensor_.getStaticResidencyTime());
  ESP_LOGE(TAG, "Fall sensitivity: %d", sensor_.getFallData(DFRobot_HumanDetection::eFallSensitivity));
}

void DFRobotC1001Component::loop() {
  // Read sensor data using the library functions, as in the Arduino sample.
  uint16_t presence = sensor_.smHumanData(DFRobot_HumanDetection::eHumanPresence);
  uint16_t movement = sensor_.smHumanData(DFRobot_HumanDetection::eHumanMovement);
  uint16_t fall     = sensor_.getFallData(DFRobot_HumanDetection::eFallState);
  uint16_t residency = sensor_.getFallData(DFRobot_HumanDetection::estaticResidencyState);

  ESP_LOGE(TAG, "Existing information: %s", 
             (presence == 0 ? "No one is present" : 
              (presence == 1 ? "Someone is present" : "Read error")));
  
  ESP_LOGE(TAG, "Motion information: %s",
             (movement == 0 ? "None" : 
              (movement == 1 ? "Still" : 
               (movement == 2 ? "Active" : "Read error"))));

  ESP_LOGE(TAG, "Body movement parameters: %d", sensor_.smHumanData(DFRobot_HumanDetection::eHumanMovingRange));
  
  ESP_LOGE(TAG, "Fall status: %s",
             (fall == 0 ? "Not fallen" : 
              (fall == 1 ? "Fallen" : "Read error")));
  
  ESP_LOGE(TAG, "Stationary dwell status: %s",
             (residency == 0 ? "No stationary dwell" : 
              (residency == 1 ? "Stationary dwell present" : "Read error")));

  // Publish the values to the corresponding ESPHome sensors
  if (this->human_presence_ != nullptr)
    this->human_presence_->publish_state(presence);
  if (this->human_movement_ != nullptr)
    this->human_movement_->publish_state(movement);
  if (this->fall_state_ != nullptr)
    this->fall_state_->publish_state(fall);
  if (this->residency_state_ != nullptr)
    this->residency_state_->publish_state(residency);

  delay(1000);
}

void DFRobotC1001Component::dump_config() {
  ESP_LOGCONFIG(TAG, "DFRobot C1001 Sensor:");
  if (human_presence_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Human Presence Sensor: %s", human_presence_->get_name().c_str());
  }
  if (human_movement_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Human Movement Sensor: %s", human_movement_->get_name().c_str());
  }
  if (fall_state_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Fall State Sensor: %s", fall_state_->get_name().c_str());
  }
  if (residency_state_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Residency State Sensor: %s", residency_state_->get_name().c_str());
  }
}

void DFRobotC1001Component::set_human_presence(esphome::sensor::Sensor *s) {
  human_presence_ = s;
}

void DFRobotC1001Component::set_human_movement(esphome::sensor::Sensor *s) {
  human_movement_ = s;
}

void DFRobotC1001Component::set_fall_state(esphome::sensor::Sensor *s) {
  fall_state_ = s;
}

void DFRobotC1001Component::set_residency_state(esphome::sensor::Sensor *s) {
  residency_state_ = s;
}

}  // namespace dfrobot_c1001
}  // namespace esphome
