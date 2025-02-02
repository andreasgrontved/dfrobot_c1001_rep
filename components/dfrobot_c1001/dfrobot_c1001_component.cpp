#include "dfrobot_c1001.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_c1001 {

static const char *TAG = "dfrobot_c1001";

void DFRobotC1001Component::setup() {
  ESP_LOGI(TAG, "Initializing DFRobot C1001 Sensor...");
  sensor_.begin();  // Initialize the sensor
}

void DFRobotC1001Component::update() {
  ESP_LOGI(TAG, "Updating Sensor Data...");
  
  // Example: Read sensor data
  int presence = sensor_.smHumanData(eHumanPresence);
  int movement = sensor_.smHumanData(eHumanMovement);

  ESP_LOGI(TAG, "Presence: %d, Movement: %d", presence, movement);
}

}  // namespace dfrobot_c1001
}  // namespace esphome
