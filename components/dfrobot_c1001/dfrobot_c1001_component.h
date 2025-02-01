#pragma once
#include "esphome.h"
#include "DFRobot_HumanDetection.h"

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public PollingComponent, public UARTDevice {
 public:
  // These sensor pointers let you publish sensor values to Home Assistant.
  // (You can add additional ones as needed.)
  Sensor *human_presence_sensor = new Sensor();
  Sensor *human_movement_sensor = new Sensor();
  Sensor *fall_state_sensor = new Sensor();
  Sensor *residency_state_sensor = new Sensor();

  // Pass the UART pointer to our UARTDevice base class.
  DFRobotC1001Component(UARTComponent *parent) : UARTDevice(parent), sensor(this) {}

  // This is called by ESPHome during setup.
  void setup() override {
    ESP_LOGI("dfrobot_c1001", "Initializing DFRobot C1001 sensor...");

    // Try to initialize the sensor; the original library’s begin() returns 0 on success.
    while (sensor.begin() != 0) {
      ESP_LOGE("dfrobot_c1001", "Sensor initialization error – retrying in 1 second...");
      delay(1000);  // Consider reducing this delay if needed.
    }

    // Configure work mode (using falling mode in this example)
    while (sensor.configWorkMode(DFRobot_HumanDetection::eFallingMode) != 0) {
      ESP_LOGE("dfrobot_c1001", "Work mode switch error – retrying...");
      delay(1000);
    }

    // Configure LED outputs so they remain off (1 = off in the library)
    sensor.configLEDLight(DFRobot_HumanDetection::eFALLLed, 1);
    sensor.configLEDLight(DFRobot_HumanDetection::eHPLed, 1);

    // Set sensor parameters (adjust these values as needed for your installation)
    sensor.dmInstallHeight(270);         // installation height in centimeters
    sensor.dmFallTime(5);                // fall time (seconds)
    sensor.dmUnmannedTime(1);            // unmanned time (seconds)
    sensor.dmFallConfig(DFRobot_HumanDetection::eResidenceTime, 200);  // dwell time (seconds)
    sensor.dmFallConfig(DFRobot_HumanDetection::eFallSensitivityC, 3); // fall sensitivity (0–3)

    // Reset the sensor (required after configuration)
    sensor.sensorRet();

    // Log the initial working mode and LED status
    uint8_t mode = sensor.getWorkMode();
    if (mode == 1) {
      ESP_LOGI("dfrobot_c1001", "Work mode: Fall detection mode");
    } else if (mode == 2) {
      ESP_LOGI("dfrobot_c1001", "Work mode: Sleep detection mode");
    } else {
      ESP_LOGW("dfrobot_c1001", "Work mode: Unknown");
    }
    ESP_LOGI("dfrobot_c1001", "Initialization complete.");
  }

  // This update() method is called periodically (per the polling interval)
  void update() override {
    // Read sensor data from the library
    uint16_t presence   = sensor.smHumanData(DFRobot_HumanDetection::eHumanPresence);
    uint16_t movement   = sensor.smHumanData(DFRobot_HumanDetection::eHumanMovement);
    uint16_t body_move  = sensor.smHumanData(DFRobot_HumanDetection::eHumanMovingRange);
    uint16_t fall_state = sensor.getFallData(DFRobot_HumanDetection::eFallState);
    uint16_t residency  = sensor.getFallData(DFRobot_HumanDetection::estaticResidencyState);

    ESP_LOGI("dfrobot_c1001", "Presence: %d, Movement: %d, Body move: %d, Fall: %d, Residency: %d",
             presence, movement, body_move, fall_state, residency);

    // Publish the sensor readings so that they show up in Home Assistant.
    human_presence_sensor->publish_state(presence);
    human_movement_sensor->publish_state(movement);
    fall_state_sensor->publish_state(fall_state);
    residency_state_sensor->publish_state(residency);
  }

 protected:
  // Create an instance of the sensor from the DFRobot_HumanDetection library.
  // Notice that we pass "this" to its constructor because our UARTDevice is a Stream.
  DFRobot_HumanDetection sensor;
};

}  // namespace dfrobot_c1001
}  // namespace esphome
