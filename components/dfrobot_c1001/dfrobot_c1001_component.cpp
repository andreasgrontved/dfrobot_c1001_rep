#include "dfrobot_c1001_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

// Constructor: store UART reference
DFRobotC1001Component::DFRobotC1001Component(uart::UARTComponent *uart)
    : uart::UARTDevice(uart), uart_parent_(uart), sensor_(nullptr) {}

void DFRobotC1001Component::setup() {
  ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");

  if (uart_parent_ == nullptr) {
    ESP_LOGE(TAG, "UART component is null!");
    return;
  }

  // Instantiate the sensor with the UART stream
  sensor_ = new DFRobot_HumanDetection(reinterpret_cast<Stream*>(uart_parent_));

  if (!sensor_->begin()) {
    ESP_LOGE(TAG, "Failed to initialize DFRobot C1001 Sensor!");
  } else {
    ESP_LOGI(TAG, "DFRobot C1001 Sensor initialized successfully.");
  }
}

void DFRobotC1001Component::update() {
  if (sensor_ == nullptr) {
    ESP_LOGE(TAG, "Sensor is not initialized!");
    return;
  }

  ESP_LOGI(TAG, "Reading DFRobot C1001 sensor data...");

  // Read sensor values
  int presence = sensor_->smHumanData(DFRobot_HumanDetection::eHumanPresence);
  int movement = sensor_->smHumanData(DFRobot_HumanDetection::eHumanMovement);
  int fall_state = sensor_->getFallData(DFRobot_HumanDetection::eFallState);
  int residency_state = sensor_->getStaticResidencyTime();

  ESP_LOGI(TAG, "Presence: %d, Movement: %d, Fall: %d, Residency: %d",
           presence, movement, fall_state, residency_state);

  // Publish data to Home Assistant
  if (human_presence_sensor) human_presence_sensor->publish_state(presence);
  if (human_movement_sensor) human_movement_sensor->publish_state(movement);
  if (fall_state_sensor) fall_state_sensor->publish_state(fall_state);
  if (residency_state_sensor) residency_state_sensor->publish_state(residency_state);
}

}  // namespace dfrobot_c1001
}  // namespace esphome
