# Lab-Digital-Twin
Inventory management and environment monitoring system for a STEM lab.

## System Overview

## Environment Monitoring

While conducting physical experiments in a lab, there are multiple variables from the lab environment that can affect the data collected in the experiment, for example, temperature, humidity, mechanical vibration, magnetic field etc. It would be beneficial, therefore, to establish a system that can monitor and record the common environmental data from the lab and thus offer the possiblilty that, in the case of experiment data mismatch, researchers can go into the database and see the possible environmental factors that affects the experiment.

Striving to solve these issues, we design and build a realtime environment monitoring system that can sample the environmental data in realtime and record in a database system for future reference.

The system consist of several Adafruit nRF52840 microcontrollers, which communicate with a nRF52840 USB dongle with BLE. The sensor nodes will read environmental data from the onboard I2C sensors, and transmit the data to the USB dongle, which in turn transmit to the host PC, every 15 seconds.

The PCB hardware design can be accessed [here](https://oshwhub.com/t-k-233/urap-sensor-node-v1). 

More detailed description can be found [here](https://github.com/Ma-Lab-Cal/Lab-Digital-Twin/blob/main/firmware/README.md)

## Inventory Management



## Usage

1. Clone the entire repository to local drive.

2. Run the .bat scripts in the `scripts` folder.

## TODOs

- [ ] Switch BLE nodes to WiFi nodes to improve stability and solve the dongle disconnect issue.
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 