# ARM Cortex-M3 Drivers

Welcome to the ARM Cortex-M3 Firmware modules repository! This repository contains drivers for the STM32F103 ARM Cortex-M3 hardware modules.

## Layers

1. **Library Layer**: This layer includes common utility modules such as `std_types` and `common_macros` for standardized types and macros.

2. **HAL (Hardware Abstraction Layer) Layer**: This layer offers higher-level drivers for various specific modules, including:
   - TFT display
   - Button
   - Ultrasonic
   - STP (Seven-Segment LED Display)
   - Oximeter
   - LED Matrix
   - LM35 Temperature Sensor
   - IR Sensor
   - Encoder
   - EEPROM (Electrically Erasable Programmable Read-Only Memory)
   - Battery Indicator

## Requirements

ARM Cortex M3 device drivers.

## Module Structure

Each module within the layers follows a consistent structure:

- `config.h`: Configuration settings for the module.
- `interface.h`: Public APIs and user interface functions.
- `private.h`: Private register definitions and functions.
- `program.c`: Implementation of the module.


## Contribution

Contributions to this repository are welcome! If you find any issues, have enhancements, or want to add support for more modules, please feel free to create pull requests.

## License

This repository is open-source and distributed under the [MIT License](LICENSE).

