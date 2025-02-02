#pragma once
#include "esphome.h"
#include "DFRobot_HumanDetection.h"  // Assumes you have this library file

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public PollingComponent, public UARTDevice {
 public:
  // Sensor pointers to publish values.
  sensor::Sensor *human_presence_sensor{new sensor::Sensor()};
  sensor::Sensor *human_movement_sensor{new sensor::Sensor()};
  sensor::Sensor *fall_state_sensor{new sensor::Sensor()};
  sensor::Sensor *residency_state_sensor{new sensor::Sensor()};

  // Constructor: pass the UART component pointer to the base class.
  DFRobotC1001Component(UARTComponent *parent) : UARTDevice(parent) {}

  // Called once during startup.
  void setup() override;

  // Called periodically; use this to poll the sensor.
  void update() override;

 protected:
  // Create an instance of the underlying DFROBOT sensor library.
  // Passing "this" allows the library to use our UART stream.
  DFRobot_HumanDetection sensor_{this};
};

}  // namespace dfrobot_c1001
}  // namespace esphome
