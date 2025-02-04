#include "dfrobot_c1001_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

void DFRobotC1001Component::setup() {
  ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");
  // Create the sensor object using the underlying UART component.
  // We assume that uart_parent_ (provided from the constructor) is a Stream.
  sensor_ = new DFRobot_HumanDetection(uart_parent_);
  if (!sensor_->begin()) {
    ESP_LOGE(TAG, "Failed to initialize DFRobot C1001 Sensor!");
  } else {
    ESP_LOGI(TAG, "DFRobot C1001 Sensor initialized successfully.");
  }
}

void DFRobotC1001Component::loop() {
  // Not needed: the DFRobot_HumanDetection library uses blocking calls in update()
  // Alternatively, you could leave this empty.
}

void DFRobotC1001Component::update() {
  // Update at most once per second (adjust as needed).
  uint32_t now = millis();
  if (now - last_read_time_ < 1000)
    return;
  last_read_time_ = now;

  ESP_LOGI(TAG, "Updating DFRobot C1001 sensor data...");

  // Query the sensor for data. These functions internally call getData() with proper parameters.
  int presence = sensor_->smHumanData(DFRobot_HumanDetection::eHumanPresence);
  int movement = sensor_->smHumanData(DFRobot_HumanDetection::eHumanMovement);
  int fall_state = sensor_->getFallData(DFRobot_HumanDetection::eFallState);
  int residency_state = sensor_->getStaticResidencyTime();

  ESP_LOGI(TAG, "Presence: %d, Movement: %d, Fall: %d, Residency: %d",
           presence, movement, fall_state, residency_state);

  // Publish sensor states if the corresponding sensor objects have been set.
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
