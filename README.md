# MAX30100 I2C Device Driver for BeagleBone Black
CDAC PG-DESD FEB-2025 Batch Project

Project Overview

This project implements a Linux kernel-based I2C device driver for the MAX30100 pulse oximeter and heart-rate sensor, specifically for the BeagleBone Black single-board computer. The solution includes both the device driver source code and the necessary Device Tree Overlay (DTO) to configure the hardware. This allows the MAX30100 to be used as a standard character device, accessible from user space applications. Additionally, the repository includes a bare metal code implementation for an STM32 microcontroller.

Key Features
Custom I2C Device Driver: A kernel module that handles all communication with the MAX30100 sensor over the I2C bus.

Device Tree Overlay: A .dts file that configures the BeagleBone Black's I2C bus and enables the driver to automatically probe and attach to the hardware.

User-Space Interface: The driver exposes the sensor data via a standard character device file (e.g., /dev/max30100), allowing easy access from C/C++ or other user-space programs.

Detailed Explanation
The Device Driver
The device driver acts as an intermediary between the operating system and the MAX30100 sensor. It encapsulates the low-level I2C commands required to initialize the sensor, read its registers, and acquire heart rate and pulse oximetry data. By implementing the driver, we've abstracted away the hardware-specific complexity, allowing any user-space application to simply open and read from a file-like interface to get the sensor data. This driver is a kernel module, meaning it can be loaded and unloaded without rebooting the system.

The Device Tree
The Device Tree is a data structure used by the Linux kernel to describe the hardware of a system, like the BeagleBone Black. Our project uses a Device Tree Overlay (DTO) to dynamically configure the system at boot time. Instead of hardcoding the sensor's I2C address and bus in the driver, the DTO specifies that a MAX30100 sensor is connected to a specific I2C bus (e.g., I2C2) at a particular address. The kernel uses this information to match the hardware with our device driver, which then automatically starts managing the sensor. This approach is flexible and portable, as it separates hardware configuration from the driver code.

Getting Started
Hardware Requirements
BeagleBone Black

MAX30100 Pulse Oximeter Sensor

Jumper wires for I2C connection

Software Requirements
A cross-compilation toolchain for ARM architecture (if compiling on a separate machine).

BeagleBone Black with a recent Linux kernel.

Device Tree Compiler (dtc).

Group Members

Varad Kalekar - kalekarvarad2@gmail.com

Sourabh Divate - sourabhdivate2727@gmail.com

Satyam Patil - satyamkp19032002@gmail.com

Shrushti Nakate - srushtinakate@gmail.com

Project Link
All required files are on GitHub.
git clone https://github.com/Varadk2002/MAX30100_I2C_Device_Driver_for_BeagleBone_Black.git

Project Details
Date: 7-8-2025

Place: Sunbeam Infotech, Pune
