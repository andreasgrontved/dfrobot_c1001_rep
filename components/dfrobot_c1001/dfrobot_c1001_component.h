// dfrobot_c1001_component.h
#pragma once
#include "esphome.h"
#include "DFRobot_HumanDetection.h"

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public PollingComponent, public UARTDevice {
 public:
  // Sensor pointers that can be published in Home Assistant
  sensor::Sensor *human_presence_sensor{new sensor::Sensor()};
  sensor::Sensor *human_movement_sensor{new sensor::Sensor()};
  sensor::Sensor *fall_state_sensor{new sensor::Sensor()};
  sensor::Sensor *residency_state_sensor{new sensor::Sensor()};

  // Constructor: pass the UART pointer to the base class.
  DFRobotC1001Component(UARTComponent *parent) : UARTDevice(parent) {}

  // Setup: initialize the sensor.
  void setup() override;

  // Update: poll the sensor and publish new values.
  void update() override;

 protected:
  // Create an instance of the DFRobot_HumanDetection library,
  // passing "this" (the UARTDevice) so that it can use the UART stream.
  DFRobot_HumanDetection sensor_{this};
};

}  // namespace dfrobot_c1001
}  // namespace esphome
