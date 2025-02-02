import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import CONF_ID, UNIT_EMPTY, ICON_RADIO_TOWER

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor", "uart"]

# Create a namespace for your component.
dfrobot_c1001_ns = cg.esphome_ns.namespace("dfrobot_c1001")
DFRobotC1001Component = dfrobot_c1001_ns.class_(
    "DFRobotC1001Component", cg.Component, uart.UARTDevice
)

# Define sensors
CONF_HUMAN_PRESENCE = "human_presence"
CONF_HUMAN_MOVEMENT = "human_movement"
CONF_FALL_STATE = "fall_state"
CONF_RESIDENCY_STATE = "residency_state"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DFRobotC1001Component),
        cv.Optional(CONF_HUMAN_PRESENCE): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY, icon=ICON_RADIO_TOWER, accuracy_decimals=0
        ),
        cv.Optional(CONF_HUMAN_MOVEMENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY, icon=ICON_RADIO_TOWER, accuracy_decimals=0
        ),
        cv.Optional(CONF_FALL_STATE): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY, icon=ICON_RADIO_TOWER, accuracy_decimals=0
        ),
        cv.Optional(CONF_RESIDENCY_STATE): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY, icon=ICON_RADIO_TOWER, accuracy_decimals=0
        ),
    }
).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_HUMAN_PRESENCE in config:
        sens = await sensor.new_sensor(config[CONF_HUMAN_PRESENCE])
        cg.add(var.set_human_presence_sensor(sens))

    if CONF_HUMAN_MOVEMENT in config:
        sens = await sensor.new_sensor(config[CONF_HUMAN_MOVEMENT])
        cg.add(var.set_human_movement_sensor(sens))

    if CONF_FALL_STATE in config:
        sens = await sensor.new_sensor(config[CONF_FALL_STATE])
        cg.add(var.set_fall_state_sensor(sens))

    if CONF_RESIDENCY_STATE in config:
        sens = await sensor.new_sensor(config[CONF_RESIDENCY_STATE])
        cg.add(var.set_residency_state_sensor(sens))
