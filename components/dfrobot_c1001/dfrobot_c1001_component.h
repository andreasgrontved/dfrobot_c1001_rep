#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "DFRobot_HumanDetection.h"

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public PollingComponent, public uart::UARTDevice {
 public:
  DFRobotC1001Component(uart::UARTComponent *uart);

  void setup() override;
  void update() override;
  void loop() override {}

  // Sensor objects (will be linked via YAML configuration)
  sensor::Sensor *human_presence_sensor{nullptr};
  sensor::Sensor *human_movement_sensor{nullptr};
  sensor::Sensor *fall_state_sensor{nullptr};
  sensor::Sensor *residency_state_sensor{nullptr};

  // Setters for the sensors
  void set_human_presence(sensor::Sensor *sensor) { human_presence_sensor = sensor; }
  void set_human_movement(sensor::Sensor *sensor) { human_movement_sensor = sensor; }
  void set_fall_state(sensor::Sensor *sensor) { fall_state_sensor = sensor; }
  void set_residency_state(sensor::Sensor *sensor) { residency_state_sensor = sensor; }

 private:
  DFRobot_HumanDetection *sensor_;  // Pointer to sensor object
  uart::UARTComponent *uart_parent_;  // Store reference to UART
};

}  // namespace dfrobot_c1001
}  // namespace esphome
