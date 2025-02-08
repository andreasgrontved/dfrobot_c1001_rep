#pragma once
#pragma message("Using updated dfrobot_c1001_component.h with static_cast<Stream*>(this)")


#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "DFRobot_HumanDetection.h"  // Include the original DFRobot library header

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public uart::UARTDevice, public Component {
 public:
  // Constructor: Cast 'this' to Stream* so that DFRobot_HumanDetection receives a Stream pointer.
  DFRobotC1001Component(uart::UARTComponent *parent)
    : uart::UARTDevice(parent), sensor_(static_cast<Stream*>(this)) {}


  void setup() override;
  void update() override;
  void dump_config() override;

  // Setter methods to assign sensor objects from YAML
  void set_human_presence(sensor::Sensor *s);
  void set_human_movement(sensor::Sensor *s);
  void set_fall_state(sensor::Sensor *s);
  void set_residency_state(sensor::Sensor *s);

 protected:
  sensor::Sensor *human_presence_{nullptr};
  sensor::Sensor *human_movement_{nullptr};
  sensor::Sensor *fall_state_{nullptr};
  sensor::Sensor *residency_state_{nullptr};

  // DFRobot_HumanDetection object that does the sensor protocol handling.
  DFRobot_HumanDetection sensor_;
};

}  // namespace dfrobot_c1001
}  // namespace esphome

