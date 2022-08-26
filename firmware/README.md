# Ma Lab Real-time Environment Monitoring System

This repository contains all the Arduino firmware code used by the real-time environment monitoring system.

The PCB hardware design can be accessed [here](https://oshwhub.com/t-k-233/urap-sensor-node-v1). 

## Design Rationale

### Background

While conducting physical experiments in a lab, there are multiple variables from the lab environment that can affect the data collected in the experiment, for example, temperature, humidity, mechanical vibration, magnetic field etc. It would be beneficial, therefore, to establish a system that can monitor and record the common environmental data from the lab and thus offer the possiblilty that, in the case of experiment data mismatch, researchers can go into the database and see the possible environmental factors that affects the experiment.

We propose a realtime environment monitoring system that can sample the environmental data in realtime and record in a database system for future reference.



## Requirements

There are severial requirements on this system:

### Low transmission power

do not want to interfere with the physical experiments in the lab, We want low peak power instead of low average power to minimize effects on sensitive instruments

### Low operating power

want to last long if using battery as power source

### Reliable

the packet loss rate must not be too high

### Real time

want to be able to retrieve real time data

### Accessible

easy to browse data & search for past data

### Expandable

easy to set up in a new environment & easy to add more nodes

### Affordable

We expect this system to be low-cost, so that other research labs can set up the system.



## System Overview

Firstly, because the sensor nodes will be placed at various locations across the lab, and potientially across multiple rooms, a wireless ccommunication is required. There are various wireless protocols that is available and accesssible in the embedded systems, ranging from several kHz to 2.4GHz. 

### WiFi

WiFi is a set of communication protocols defined by [IEEE_802.11 standard](https://www.ieee802.org/11/). It is available at 2.4 GHz or 5 GHz band, and features very high data rate. The transmission power is typically 10 - 20dBm.

A disadvantage, however, is that most campus prohibits faculties from setting up personal WiFi hotspots due to security reasons, and the campus-wide network often have strict firewall settings that does not allow device discovery or direct data communication with each other. 

### LoRaWAN

LoRaWAN is a low power, wide area networking protocol. It uses a star network topology which supports up to 120 devices. The maximum transmission power is 9dBm. The duty cycle of LoRaWAN is 1%, which too low for our realtime application requirement.

### ZigBee

// TODO

### nRF24

// TODO

### BLE

[Bluetooth Low Energy (BLE)](https://bluetoothle.wiki/specifications) is a wireless personal area network technology, which is not compatible with classic [Bluetooth](https://www.bluetooth.com/specifications/specs/) protocol.

A concerning factor of BLE protocol is the number of device it supports. This varies from device to device, and will be explained in later section.



## Microcontroller

We decide to use the [nRF52840](https://www.nordicsemi.com/products/nrf52840) as the microprocessor in the system, as it not only supports many BLE application layer protocols, but it also supports ZigBee and other 2.4GHz stacks, which offers the possiblity of switching to other wireless protocols in the future, if needed. Another advantage of this chip is that Adafruit has a set of products featuring this chip, including the [Adafruit ItsyBitsy nRF52840 Express](https://learn.adafruit.com/adafruit-itsybitsy-nrf52840-express) and the [Adafruit nRF52840 USB Key](https://www.adafruit.com/product/5199).

Among the numerous BLE services available in the nRF52840 software stack, we choose the [Bluetooth LE GATT Nordic UART Service](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/1.4.0/nrf/include/bluetooth/services/nus.html). 

the Bluetooth LE GATT Nordic UART Service is a custom service that receives and writes data and serves as a bridge to the UART interface, which can be easily integrated in an application that uses [Arduino serial communication](https://www.arduino.cc/reference/en/language/functions/communication/serial/).

// TODO: number of connections this service supports.



## Network Topology

The system network is composed of one BLE-to-USB dongle node using the nRF52840 USB Key, and several sensor nodes using the ItsyBitsy nRF52840 Express board.

The sensor node will read from all the on-board I2C sensors every 15 seconds, and transmit the data packet to the USB dongle, which in turn transmits to the PC using normal USB serial communication.



## Sensors

We select sensors according to the following considerations:

- Accuracy (sensor-to-sensor, can calibrate)

- Precision

- Robustness (drift)

- Ease of use (interfacing with MCU)

- Availability

- Cost



### Temperature

// TODO: reformat

Integrated sensor on Adafruit Sense Feather only has +-1C accuracy -- Not good enough

Consider Pt resistor or higher-end digital sensors by Adafruit (1, 2 with humidity, )

On nearby components heating up -- likely need to design our own carrier board to move these as far away as possible from heat-generating components

This will also allow us to use a wider selection of sensors, like STS35, SHT35-DIS-F, TMP117 and MMC5983A (for magnetic field) 

> Note: humidity sensors will drift overtime, so recommended to recalibrate every 6 month/a year
   Not a huge drift, so can accept



// TODO

### Sensor I2C Mapping

| I2C Address | Sensor     |
| ----------- | ---------- |
| 0x12        | PMSA003I   |
| 0x29        | TSL2591    |
| 0x30        | MMC5983MA  |
| 0x44        | SHT-35     |
| 0x5A        | ISM330DHCX |
| 0x77        | DPS310     |

## Power


## Database

### Google Spreadsheet

### SQL database

### csv database



