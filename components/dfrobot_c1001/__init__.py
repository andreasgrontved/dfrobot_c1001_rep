import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["uart"]

# Create a namespace for your component.
dfrobot_c1001_ns = cg.esphome_ns.namespace("dfrobot_c1001")
# Declare your custom component class.
DFRobotC1001Component = dfrobot_c1001_ns.class_("DFRobotC1001Component", cg.Component, uart.UARTDevice)

# Define a minimal configuration schema.
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DFRobotC1001Component),
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    # Create an instance of your component using the UART device.
    var = cg.new_Pvariable(config[CONF_ID], await cg.get_variable(config[CONF_ID]))
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
