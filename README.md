# Anjay-stm32-azurertos-client [<img align="right" height="50px" src="https://avsystem.github.io/Anjay-doc/_images/avsystem_logo.png">](http://www.avsystem.com/)


## Overview

This repository contains LwM2M Client application based on open-source [Anjay](https://github.com/AVSystem/Anjay) library and [X-Cube-Cellular](https://www.st.com/en/embedded-software/x-cube-cellular.html) package which includes cellular modem drivers, [Azure RTOS ThreadX](https://github.com/azure-rtos/threadx) as well as [mbedtls](https://github.com/ARMmbed/mbedtls). This example supports the [B-U585I-IOT02A Discovery kit](https://www.st.com/en/evaluation-tools/b-u585i-iot02a.html) with [Quectel BG96](https://www.quectel.com/product/lpwa-bg96-cat-m1-nb1-egprs) modem (provided with P-L496G-CELL02 devkit)

The project was developed using [STM32Cube tools](https://www.st.com/en/ecosystems/stm32cube.html) ([STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) in particular).

The following LwM2M Objects are supported:

- Security (/0),
- Server (/1),
- Device (/3),
- Firmware Update (/5),
- Temperature (/3303),
- Humidity (/3304),
- Accelerometer (/3313),
- Magnetometer (/3314),
- Barometer (/3315),


## Cloning the repository

```
git clone --recursive https://github.com/AVSystem/Anjay-stm32-azurertos-client
```

## Compilation guide

 - Download [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)
 - Import the cloned project to your workspace
 - To build and run the project please follow the instructions in `Projects/B-U585I-IOT02A/README.md`

## Connecting to the LwM2M Server

To connect to [Coiote IoT Device Management](https://www.avsystem.com/products/coiote-iot-device-management-platform/) LwM2M Server, please register at https://eu.iot.avsystem.cloud/. Then have a look at the Configuration menu to configure security credentials.

You must upgrade the firmware of the modem to at least `BG96MAR02A08M1G` revision. Older versions may cause an unexpected loss of connection.

[Guide showing basic usage of Coiote DM](https://iotdevzone.avsystem.com/docs/Coiote_IoT_DM/Quick_Start/Connect_device_quickstart/) is available on IoT Developer Zone.


## Configuration menu

While connected to a serial port interface, and during bootup, the device shows:

```
Press any key in 3 seconds to enter config menu...
```

You can then press any key on your keyboard to enter the configuration menu. After that, you'll see a few configuration options that can be altered and persisted within the non-volatile memory for future bootups.

