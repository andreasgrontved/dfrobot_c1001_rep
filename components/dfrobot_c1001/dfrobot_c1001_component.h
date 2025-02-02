#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "DFRobot_HumanDetection.h"

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public PollingComponent, public uart::UARTDevice {
public:
    DFRobotC1001Component(UARTComponent *uart) : uart::UARTDevice(uart), sensor_(uart) {}

    void setup() override;
    void update() override;

    // Expose sensors as properties
    esphome::sensor::Sensor *human_presence_sensor{nullptr};
    esphome::sensor::Sensor *human_movement_sensor{nullptr};
    esphome::sensor::Sensor *fall_state_sensor{nullptr};
    esphome::sensor::Sensor *residency_state_sensor{nullptr};

private:
    DFRobot_HumanDetection sensor_;
};

}  // namespace dfrobot_c1001
}  // namespace esphome