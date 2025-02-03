#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "DFRobot_HumanDetection.h"

namespace esphome {
namespace dfrobot_c1001 {

class DFRobotC1001Component : public PollingComponent, public uart::UARTDevice {
public:
    DFRobotC1001Component(UARTComponent *uart) : uart::UARTDevice(uart) {}

    void setup() override;
    void update() override;
    void loop() override;  // Handle UART communication

    // Sensor entities exposed to ESPHome
    esphome::sensor::Sensor *human_presence_sensor{nullptr};
    esphome::sensor::Sensor *human_movement_sensor{nullptr};
    esphome::sensor::Sensor *fall_state_sensor{nullptr};
    esphome::sensor::Sensor *residency_state_sensor{nullptr};

private:
    DFRobot_HumanDetection sensor_;  
    uint32_t last_read_time_ = 0;  // Track last read time
};

}  // namespace dfrobot_c1001
}  // namespace esphome
