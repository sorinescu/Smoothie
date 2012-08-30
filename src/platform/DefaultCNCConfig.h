/*
      This file contains the default hardware and software configuration of the actual CNC hardware                                                                                                                                             .
                                                                                                                                                                                                                                                .
      These values can be overridden by a configuration file, if SMOOTHIE_USE_FILES is enabled                                                                                                                                                  .
      If SMOOTHIE_USE_FILES is NOT enabled, these are the actual configuration of the hardware                                                                                                                                                  .
                                                                                                                                                                                                                                                .
      You should customize these values to match your particular hardware

      Pin values have the following format:
      PIN(is_inverting, port_number, pin_number)
      or
      PIN_NC
      where:
        PIN_NC means the pin is not connected
        is_inverting can be PIN_INVERTING or PIN_NON_INVERTING
        port_number can be 0 (GPIOA), 1 (GPIOB) etc.
        pin_number can be 0..15

      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _DEFAULT_CNC_CONFIG_H_
#define _DEFAULT_CNC_CONFIG_H_

// CartesianSolution
#define ALPHA_STEPS_PER_MM 50.0
#define BETA_STEPS_PER_MM  50.0
#define GAMMA_STEPS_PER_MM 50.0

// Planner
#define ACCELERATION       100.0
#define MAX_JERK           100.0
#define JUNCTION_DEVIATION 0.05

// Robot
#define DEFAULT_FEED_RATE   100.0
#define DEFAULT_SEEK_RATE   100.0
#define MM_PER_LINE_SEGMENT 0.1
#define MM_PER_ARC_SEGMENT  10.0
#define ARC_CORRECTION      5.0
#define X_AXIS_MAX_SPEED    0.0
#define Y_AXIS_MAX_SPEED    0.0
#define Z_AXIS_MAX_SPEED    0.0

// Stepper
#define MICROSECONDS_PER_STEP_PULSE   5.0
#define ACCELERATION_TICKS_PER_SECOND 100.0
#define MINIMUM_STEPS_PER_MINUTE      1200.0
#define BASE_STEPPING_FREQUENCY       100000.0

#define ALPHA_STEP_PIN      PIN(PIN_NON_INVERTING, 3, 12)
#define BETA_STEP_PIN       PIN(PIN_NON_INVERTING, 3, 13)
#define GAMMA_STEP_PIN      PIN(PIN_NON_INVERTING, 3, 14)
#define ALPHA_DIR_PIN       PIN(PIN_NON_INVERTING, 3, 15)
#define BETA_DIR_PIN        PIN(PIN_NON_INVERTING, 3, 11)
#define GAMMA_DIR_PIN       PIN(PIN_NON_INVERTING, 3, 10)
#define ALPHA_EN_PIN        PIN(PIN_NON_INVERTING, 0, 4)
#define BETA_EN_PIN         PIN(PIN_NON_INVERTING, 0, 10)
#define GAMMA_EN_PIN        PIN(PIN_NON_INVERTING, 0, 19)

// Extruder
#define EXTRUDER_MODULE_ENABLE               false
#define EXTRUDER_MICROSECONDS_PER_STEP_PULSE 5.0
#define EXTRUDER_STEPS_PER_MM                1.0
#define EXTRUDER_DEFAULT_FEED_RATE           1.0
#define EXTRUDER_ACCELERATION                1.0

#define EXTRUDER_STEP_PIN   PIN(PIN_NON_INVERTING, 1, 22)
#define EXTRUDER_DIR_PIN    PIN(PIN_NON_INVERTING, 1, 19)
#define EXTRUDER_EN_PIN     PIN(PIN_NON_INVERTING, 0, 19)

// Laser
#define LASER_MODULE_ENABLE false

// TemperatureControl
#define TEMPERATURE_CONTROL_MODULE_ENABLE       false
#define TEMPERATURE_CONTROL_READINGS_PER_SECOND 5
#define TEMPERATURE_CONTROL_R0   100000.0
#define TEMPERATURE_CONTROL_T0   25.0
#define TEMPERATURE_CONTROL_BETA 4066.0
#define TEMPERATURE_CONTROL_VADC 3.3
#define TEMPERATURE_CONTROL_VCC  3.3
#define TEMPERATURE_CONTROL_R1   0.0
#define TEMPERATURE_CONTROL_R2   4700

// CurrentControl
#define CURRENTCONTROL_MODULE_ENABLE false
#define ALPHA_CURRENT 0.8
#define BETA_CURRENT  0.8
#define GAMMA_CURRENT 0.8
#define DELTA_CURRENT 0.8

// PauseButton
#define PAUSE_BUTTON_PIN    PIN_NC
#define PAUSE_LED_PIN       PIN_NC

#endif // _DEFAULT_CNC_CONFIG_H_

