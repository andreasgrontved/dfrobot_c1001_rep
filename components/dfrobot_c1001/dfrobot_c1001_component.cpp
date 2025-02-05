#include "dfrobot_c1001_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

// Correct constructor: Pass UART as a stream
DFRobotC1001Component::DFRobotC1001Component(uart::UARTComponent *uart)
    : uart::UARTDevice(uart), sensor_(reinterpret_cast<Stream *>(uart)) {}

void DFRobotC1001Component::setup() {
  ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");

  if (!sensor_.begin()) {
    ESP_LOGE(TAG, "Failed to initialize DFRobot C1001 Sensor!");
  } else {
    ESP_LOGI(TAG, "DFRobot C1001 Sensor initialized successfully.");
  }
}

void DFRobotC1001Component::update() {
  ESP_LOGI(TAG, "Reading DFRobot C1001 sensor data...");

  // Correct getData() call
  uint8_t cmd = 0x80;
  uint8_t response[10] = {0};
  uint8_t data[1] = {0};

  if (sensor_.getData(0x80, 0x81, 1, data, response) == 0) {
    int presence = response[6];  // Extract the human presence value
    ESP_LOGI(TAG, "Human presence detected: %d", presence);
    if (human_presence_sensor) human_presence_sensor->publish_state(presence);
  }

  int movement = sensor_.smHumanData(DFRobot_HumanDetection::eHumanMovement);
  int fall_state = sensor_.getFallData(DFRobot_HumanDetection::eFallState);
  int residency_state = sensor_.getStaticResidencyTime();

  ESP_LOGI(TAG, "Movement: %d, Fall: %d, Residency: %d",
           movement, fall_state, residency_state);

  if (human_movement_sensor) human_movement_sensor->publish_state(movement);
  if (fall_state_sensor) fall_state_sensor->publish_state(fall_state);
  if (residency_state_sensor) residency_state_sensor->publish_state(residency_state);
}

}  // namespace dfrobot_c1001
}  // namespace esphome
