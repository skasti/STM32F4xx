/*
  generic_map.h - driver code for STM32F4xx ARM processors

  Part of grblHAL

  Copyright (c) 2020-2021 Terje Io

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#if N_ABC_MOTORS > 2
#error "Axis configuration is not supported!"
#endif

#if TRINAMIC_ENABLE
#error "Trinamic plugin not supported!"
#endif

#define BOARD_NAME "FlexiCNC"
#ifdef I2C_ENABLE
  #define FMP_I2C 1
#endif

//#define EEPROM_ENABLE 1
#define STM32F446xx 1
//#define IOEXPAND_ENABLE 1
#define HAS_IOPORTS 1
//#define HAS_BOARD_INIT 1

#if KEYPAD_ENABLE
#define I2C_STROBE_ENABLE 1
#endif

// Define step pulse output pins.
#define X_STEP_PORT             GPIOC
#define X_STEP_PIN              2
#define Y_STEP_PORT				      GPIOC
#define Y_STEP_PIN              15
#define Z_STEP_PORT				      GPIOB
#define Z_STEP_PIN              13
#define STEP_OUTMODE            GPIO_BITBAND
//#define STEP_PINMODE            PINMODE_OD // Uncomment for open drain outputs

// Define step direction output pins.
#define X_DIRECTION_PORT        GPIOC
#define X_DIRECTION_PIN         0
#define Y_DIRECTION_PORT        GPIOC
#define Y_DIRECTION_PIN         1
#define Z_DIRECTION_PORT        GPIOA
#define Z_DIRECTION_PIN         4
#define DIRECTION_OUTMODE       GPIO_BITBAND
//#define DIRECTION_PINMODE       PINMODE_OD // Uncomment for open drain outputs

// Define stepper driver enable/disable output pin.
#define STEPPERS_ENABLE_PORT       GPIOA
#define STEPPERS_ENABLE_PIN        14
#define STEPPERS_ENABLE_OUTMODE    GPIO_BITBAND

// Define homing/hard limit switch input pins.
#define X_LIMIT_PORT            GPIOA
#define X_LIMIT_PIN             5
#define Y_LIMIT_PORT            GPIOC
#define Y_LIMIT_PIN             11
#define Z_LIMIT_PORT            GPIOC
#define Z_LIMIT_PIN             13
#define LIMIT_INMODE            GPIO_BITBAND

// Define ganged axis or A axis step pulse and step direction output pins.
#if N_ABC_MOTORS == 1
#define M3_AVAILABLE
#define M3_STEP_PORT            GPIOB
#define M3_STEP_PIN             14
#define M3_DIRECTION_PORT       GPIOB
#define M3_DIRECTION_PIN        15
#if N_AUTO_SQUARED
#define M3_LIMIT_PORT           GPIOB
#define M3_LIMIT_PIN            6
#endif
#endif

// Define ganged axis or A axis step pulse and step direction output pins.
#if N_ABC_MOTORS == 2
#define M34AVAILABLE
#define M4_STEP_PORT            GPIOC
#define M4_STEP_PIN             8
#define M4_DIRECTION_PORT       GPIOC
#define M4_DIRECTION_PIN        9
#if N_AUTO_SQUARED > 1
#define M4_LIMIT_PORT           GPIOC
#define M4_LIMIT_PIN            14
#endif
#endif

// Define spindle enable and spindle direction output pins.
#define SPINDLE_ENABLE_PORT         GPIOB
#define SPINDLE_ENABLE_PIN          2
#define SPINDLE_DIRECTION_PORT      GPIOB
#define SPINDLE_DIRECTION_PIN       1
#define SPINDLE_OUTMODE             GPIO_BITBAND

// Define spindle PWM output pin.
#define SPINDLE_PWM_PORT_BASE   GPIOA_BASE
#define SPINDLE_PWM_PIN         8

// Define flood and mist coolant enable output pins.
#define COOLANT_FLOOD_PORT          GPIOC
#define COOLANT_FLOOD_PIN           12
#define COOLANT_MIST_PORT           GPIOA
#define COOLANT_MIST_PIN            7
#define COOLANT_OUTMODE             GPIO_BITBAND

#define AUXOUTPUT0_PORT 		GPIOD
#define AUXOUTPUT0_PIN 			2
#define AUXOUTPUT1_PORT 		GPIOB
#define AUXOUTPUT1_PIN 			0
#define AUXOUTPUT2_PORT 		GPIOA
#define AUXOUTPUT2_PIN 			3
#define AUXOUTPUT3_PORT 		GPIOA
#define AUXOUTPUT3_PIN 			6
#define AUXOUTPUT_OUTMODE       GPIO_BITBAND

//this is broken because of interrupt contention
#define AUXINPUT0_PORT          GPIOA
#define AUXINPUT0_PIN           0
#define AUXINPUT1_PORT          GPIOA
#define AUXINPUT1_PIN           1
#define AUXINPUT2_PORT          GPIOA
#define AUXINPUT2_PIN           2


// Define user-control controls (cycle start, reset, feed hold) input pins.
#define RESET_PORT            	GPIOB
#define RESET_PIN               12
#define FEED_HOLD_PORT          GPIOB
#define FEED_HOLD_PIN           8
#define CYCLE_START_PORT      	GPIOB
#define CYCLE_START_PIN         9
#define CONTROL_INMODE 			GPIO_BITBAND

// Define probe switch input pin.
#define PROBE_PORT              GPIOB
#define PROBE_PIN               7

#if I2C_STROBE_ENABLE
#define I2C_STROBE_PORT         GPIOB
#define I2C_STROBE_PIN          10
#endif

#if SDCARD_ENABLE
#define SD_CS_PORT              GPIOA
#define SD_CS_PIN               3
#define SPI_PORT                1 // GPIOA, SCK_PIN = 5, MISO_PIN = 6, MOSI_PIN = 7
#endif