#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "DFRobot_HumanDetection.h"

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public PollingComponent, public uart::UARTDevice {
 public:
  // Constructor: Pass the UART component (which acts as a Stream) to the base.
  DFRobotC1001Component(uart::UARTComponent *uart) : uart::UARTDevice(uart) {}

  void setup() override;
  void update() override;
  void loop() override;  // Called continuously to process UART data

  // Sensor pointers that will be set via configuration.
  sensor::Sensor *human_presence_sensor{nullptr};
  sensor::Sensor *human_movement_sensor{nullptr};
  sensor::Sensor *fall_state_sensor{nullptr};
  sensor::Sensor *residency_state_sensor{nullptr};

  // Setter methods called by code-generation.
  void set_human_presence(sensor::Sensor *sensor) { human_presence_sensor = sensor; }
  void set_human_movement(sensor::Sensor *sensor) { human_movement_sensor = sensor; }
  void set_fall_state(sensor::Sensor *sensor) { fall_state_sensor = sensor; }
  void set_residency_state(sensor::Sensor *sensor) { residency_state_sensor = sensor; }

 private:
  // The sensor object from the DFRobot_HumanDetection library.
  DFRobot_HumanDetection sensor_;
  uint32_t last_read_time_{0};
};

}  // namespace dfrobot_c1001
}  // namespace esphome
