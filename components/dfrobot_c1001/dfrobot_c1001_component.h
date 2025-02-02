#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "DFRobot_HumanDetection.h"  // Ensure this library is included

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public PollingComponent, public uart::UARTDevice {
 public:
  DFRobotC1001Component(UARTComponent *uart) : uart::UARTDevice(uart), sensor_(uart) {}

  void setup() override;
  void update() override;

 private:
  DFRobot_HumanDetection sensor_;
};

}  // namespace dfrobot_c1001
}  // namespace esphome
