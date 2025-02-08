#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "DFRobot_HumanDetection.h"  // Include the DFRobot library header

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public PollingComponent, public uart::UARTDevice {
 public:
  // Cast 'this' to Stream* to match the expected constructor parameter
  DFRobotC1001Component(uart::UARTComponent *parent)
      : uart::UARTDevice(parent), sensor_(static_cast<Stream*>(this)) {}

  void setup() override;
  void update() override;

  void set_human_presence(sensor::Sensor *s) { human_presence_ = s; }
  void set_human_movement(sensor::Sensor *s) { human_movement_ = s; }
  void set_fall_state(sensor::Sensor *s) { fall_state_ = s; }
  void set_residency_state(sensor::Sensor *s) { residency_state_ = s; }

 protected:
  sensor::Sensor *human_presence_{nullptr};
  sensor::Sensor *human_movement_{nullptr};
  sensor::Sensor *fall_state_{nullptr};
  sensor::Sensor *residency_state_{nullptr};

  DFRobot_HumanDetection sensor_;
};

}  // namespace dfrobot_c1001
}  // namespace esphome
