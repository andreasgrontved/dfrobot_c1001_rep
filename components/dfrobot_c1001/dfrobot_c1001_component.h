#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"  // for esphome::sensor::Sensor
#include "DFRobot_HumanDetection.h"            // DFRobot sensor library

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public uart::UARTDevice, public Component {
 public:
  // Constructor: pass the UARTComponent pointer. We use reinterpret_cast to obtain a Stream*
  DFRobotC1001Component(uart::UARTComponent *parent)
      : uart::UARTDevice(parent),
        // Here, we assume that 'parent' is based on a HardwareSerial (a subclass of Stream)
        sensor_(reinterpret_cast<Stream*>(parent)) {}

  void setup() override;
  void loop() override;   // We'll use loop() to mimic Arduino's loop()
  void dump_config() override;

  // Setter methods for sensor objects.
  void set_human_presence(esphome::sensor::Sensor *s);
  void set_human_movement(esphome::sensor::Sensor *s);
  void set_fall_state(esphome::sensor::Sensor *s);
  void set_residency_state(esphome::sensor::Sensor *s);

 protected:
  esphome::sensor::Sensor *human_presence_{nullptr};
  esphome::sensor::Sensor *human_movement_{nullptr};
  esphome::sensor::Sensor *fall_state_{nullptr};
  esphome::sensor::Sensor *residency_state_{nullptr};

  // The DFRobot_HumanDetection object that implements the sensor protocol.
  DFRobot_HumanDetection sensor_;
};

}  // namespace dfrobot_c1001
}  // namespace esphome
