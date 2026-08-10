# MCP4725_Driver
The MCP4725 Driver is a reusable embedded software library that provides a complete software interface for the Microchip MCP4725 12-bit Digital-to-Analog Converter (DAC).
# MCP4725 Driver Documentation

**Version:** 1.0.0
**Author:** **WilfriedEngineering**

---

## Design Philosophy

The **MCP4725 Driver** is designed according to the following software engineering principles:

* Modular architecture
* Hardware portability
* Code reusability
* Ease of maintenance
* Clear hardware abstraction layers
* Low memory footprint for embedded systems

**Programming Language:** C (MPLAB XC8)
**Target MCU:** Microchip PIC16F87XA Family (PIC16F873A, PIC16F874A, PIC16F876A, and PIC16F877A)
**Communication Interface:** I²C Standard Mode (100 kHz) and Fast Mode (400 kHz)

---

## 1. Driver Overview

The **MCP4725 Driver** is a reusable embedded software library that provides a software interface for the **Microchip MCP4725 12-bit Digital-to-Analog Converter (DAC)**.

The driver supports the following operations:

* Writing data to the DAC register
* Reading and writing EEPROM data
* Reading device status
* Managing power-down modes

The driver is designed to provide a simple, portable, and hardware-independent interface for applications requiring analog voltage generation.

---

## 2. Performance

The **MCP4725 Driver** supports the following device characteristics:

* **12-bit DAC resolution** — 4096 output levels
* **I²C communication interface**
* **EEPROM write time:** 25–50 ms
* **Operating supply voltage:** 2.7 V to 5.5 V

---

## 3. Limitations

The driver performance is subject to the following hardware limitations:

* The DAC update rate depends on the selected I²C bus speed.
* EEPROM memory has a limited write endurance; therefore, unnecessary EEPROM write operations should be avoided.
* The driver depends on the underlying I²C driver for communication with the MCP4725.

---

## 4. Features

The driver provides the following functionality:

* EEPROM and DAC register write
* EEPROM and DAC register read
* Fast DAC updates
* Power-down mode control
* Device status monitoring
* MPLAB XC8 compiler compatibility

---

## 5. Software Requirements

The following software tools and libraries are required:

* **MPLAB X IDE**
* **MPLAB XC8 Compiler**
* **PIC16F87XA microcontroller**
* **I²C Bus Driver**

> **Dependency:** The MCP4725 Driver requires the I²C Bus Driver for communication with the MCP4725 device.

---

## 6. Hardware Requirements

The following hardware components are required:

* PIC16F87XA microcontroller
* MCP4725 Digital-to-Analog Converter
* 5–10 kΩ pull-up resistors on the SDA and SCL lines
* Appropriate power supply for the MCP4725 and I²C bus

---

## 7. Driver Architecture

```text
+--------------------------+
|       Application        |
+--------------------------+
             |
             v
+--------------------------+
|      MCP4725 Driver      |
+--------------------------+
             |
             v
+--------------------------+
|       I²C Driver         |
+--------------------------+
             |
             v
+--------------------------+
|   PIC16F87XA I²C Hardware|
+--------------------------+
```

The **MCP4725 Driver** communicates with the hardware exclusively through the **I²C Driver**, providing a clean abstraction layer between the application software and the underlying hardware peripheral.

This modular architecture simplifies maintenance and allows the driver to be reused across different projects with minimal modifications.

---

# 8. Public API

## Data Structures

### `MCP4725_DATA`

The `MCP4725_DATA` structure stores the configuration parameters and runtime data required by the MCP4725 driver. It is also used to store information read from the device.

### Structure Members

| Member         | Type           | Description                                                                                                      |
| -------------- | -------------- | ---------------------------------------------------------------------------------------------------------------- |
| `mbus`         | `BUS_I2C *`    | Pointer to the I²C bus driver used for communication with the MCP4725 device.                                    |
| `address`      | `char`         | Seven-bit I²C slave address of the MCP4725 device. The configurable address bits are **A2**, **A1**, and **A0**. |
| `ready`        | `char`         | Stores the EEPROM write status. Indicates whether a new EEPROM write operation can be initiated.                 |
| `eeprom_data`  | `unsigned int` | Stores the value currently read from the device EEPROM.                                                          |
| `dac_reg_data` | `unsigned int` | Stores the value currently contained in the DAC register.                                                        |

---

## Device Address

The MCP4725 uses a seven-bit I²C device address:

```text
A7 A6 A5 A4 A3 A2 A1 A0
```

The address configuration is:

* **A7–A3:** Fixed to `0`
* **A2, A1, A0:** Configurable address bits
* Each configurable address bit can be either `0` or `1`.

The device address must be configured correctly before any read or write operation is performed.

---

## `MCP4725_FUNCTION`

The `MCP4725_FUNCTION` structure contains pointers to all public driver functions. Each function operates on an `MCP4725_DATA` object.

The function names follow the convention:

```text
MCP4725_<FunctionName>
```

---

# Function Descriptions

## `Config`

```c
void (*Config)(MCP4725_DATA *mcp, unsigned int reg_val);
```

Writes the same value to both the EEPROM and the DAC register.

After a device reset, the value stored in EEPROM is automatically loaded into the DAC register and becomes the default output value.

### Parameters

* `mcp` — Pointer to an `MCP4725_DATA` structure.
* `reg_val` — Value to be written to both the EEPROM and the DAC register.

**Valid range:** `0–4095`

---

## `Write`

```c
void (*Write)(MCP4725_DATA *mcp, unsigned int dac_val);
```

Updates the DAC register without modifying the EEPROM contents.

### Parameters

* `mcp` — Pointer to an `MCP4725_DATA` structure.
* `dac_val` — Value to be written to the DAC register.

**Valid range:** `0–4095`

---

## `Read`

```c
void (*Read)(MCP4725_DATA *mcp);
```

Reads the current device information and stores the retrieved values in the supplied `MCP4725_DATA` structure.

The following information is read:

* EEPROM data
* EEPROM write status
* DAC operating mode
* EEPROM operating mode

The retrieved values are stored in the corresponding members of the `MCP4725_DATA` structure.

The `eeprom_data` and `dac_reg_data` members contain values in the range:

```text
0–4095
```

### EEPROM Write Status

The `ready` member indicates the EEPROM write status:

| `ready` | Description                                                                                                                             |
| ------: | --------------------------------------------------------------------------------------------------------------------------------------- |
|     `0` | An EEPROM write operation is currently in progress. A new EEPROM write request will be ignored until the current operation is complete. |
|     `1` | The previous EEPROM write operation has completed, and the device is ready to accept a new EEPROM write request.                        |

---

## `Power_Down`

```c
void (*Power_Down)(MCP4725_DATA *mcp, char x_res);
```

Places the MCP4725 into **Power-Down mode** to reduce power consumption when the DAC output is not required.

During Power-Down mode, the DAC output is internally connected to ground through a selectable resistor.

### Parameters

* `mcp` — Pointer to an `MCP4725_DATA` structure.
* `x_res` — Selects the output pull-down resistor.

### Supported Values

| Constant         | Description                                                  |
| ---------------- | ------------------------------------------------------------ |
| `MCP4725_RES1`   | Connects the output to ground through a **1 kΩ** resistor.   |
| `MCP4725_RES100` | Connects the output to ground through a **100 kΩ** resistor. |
| `MCP4725_RES500` | Connects the output to ground through a **500 kΩ** resistor. |

---

# 9. Driver Function Prototypes

The following public functions are provided by the driver:

```c
void MCP4725_Config(MCP4725_DATA *mcp, unsigned int reg_val);

void MCP4725_Write(MCP4725_DATA *mcp, unsigned int dac_val);

void MCP4725_Read(MCP4725_DATA *mcp);

void MCP4725_Power_Down(MCP4725_DATA *mcp, char x_res);
```

---

# 10. Important Notes

Before using the driver, ensure that the following conditions are satisfied:

* Configure the MCP4725 device address (**A2, A1, and A0**) correctly before performing any communication with the device.
* Select an appropriate I²C bus frequency, particularly when multiple I²C devices share the same bus.
* Avoid unnecessary EEPROM write operations, as EEPROM memory has a limited write endurance.
* Verify that the I²C bus is properly initialized before calling any driver function.
* Ensure that the required **I²C Driver** dependency is available and correctly configured.
* Ensure that the MCP4725 supply voltage and I²C signal levels are compatible with the connected hardware.

---
# Demo Directory Guide

This directory contains the files required to understand and reproduce the **MCP4725 Driver** test.

## Directory Contents

```text
demo/
│
├── MCP4725_Demo.c
├── MCP4725_Demo.hex
├── MCP4725_Demo.pdsprj
├── MCP4725_Demo_Schematic.pdf
├── MCP4725_Demo_Flowchart.pdf
├── MCP4725_Demo.mp4

```

## Files

* `MCP4725_Demo.c` — Test source code
* `MCP4725_Demo.hex` — Compiled firmware
* `MCP4725_Demo.pdsprj` — Proteus simulation project
* `MCP4725_Demo_Schematic.pdf` — Test schematic
* `MCP4725_Demo_Flowchart.pdf` — Test flowchart
* `MCP4725_Demo.mp4` — Test demonstration video

---

# 12. References

The following documents should be consulted for detailed hardware information:

* **Microchip PIC16F87XA Family Data Sheet**
* **Microchip MCP4725 12-Bit DAC Data Sheet** — available in the `reference` folder
* **MPLAB XC8 Compiler Documentation**
* **MPLAB X IDE Documentation**

---

# License

This project is licensed under the **MIT License**.

See the [`LICENSE`](LICENSE) file for the complete license text.

---

# Author

**WilfriedEngineering**

Embedded software and hardware development focused on microcontroller-based systems.
