// dfrobot_c1001_component.h
#pragma once
#include "esphome.h"
#include "DFRobot_HumanDetection.h"

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public PollingComponent, public UARTDevice {
 public:
  // Sensor pointers to publish values to Home Assistant.
  sensor::Sensor *human_presence_sensor{new sensor::Sensor()};
  sensor::Sensor *human_movement_sensor{new sensor::Sensor()};
  sensor::Sensor *fall_state_sensor{new sensor::Sensor()};
  sensor::Sensor *residency_state_sensor{new sensor::Sensor()};

  // Constructor: pass the UART component pointer to the UARTDevice base class.
  DFRobotC1001Component(UARTComponent *parent) : UARTDevice(parent) {}

  // Setup is called once during startup.
  void setup() override;

  // Update is called at a regular interval (set via set_update_interval).
  void update() override;

 protected:
  // Instantiate the underlying DFRobot_HumanDetection library, passing our UART stream.
  DFRobot_HumanDetection sensor_{this};
};

}  // namespace dfrobot_c1001
}  // namespace esphome
