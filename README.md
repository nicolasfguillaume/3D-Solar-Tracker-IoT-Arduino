# 3D-printed Single-axis solar tracker with Energy Storage and Bluetooth Monitoring

Solar energy is converted by the solar array and charges the Battery pack. The Battery pack is supplying a controller which optimize the orientation of the solar array towards the sunlight, based on the light sensor attached to the solar array. In addition to powering the control system, the battery pack has enough capacity to concurrently charge a smartphone or a tablet. It takes approximately about 10 hours (under STC) -or about 1.5 day- to charge the battery pack completely.

![solar tracker](https://github.com/nicolasfguillaume/solar-tracker-iot-arduino/blob/master/solar-tracker.jpg)

Overall technical Specs:
- Solar Array: 5Wp (= 5 x 1W solar panels in parallel)
- Energy Storage: 1 x 3000 mAh Lithiun Ion battery
- Controller: Arduino Uno Rev3, featuring a sun-tracking algorithm
  * Input: light sensor (2 LDR mounted at 45 degrees)
  * Output: servo motor (9g, 1.5 kg.cm)
- Monitoring: Bluetooth communication protocol (serial UART), sending data every 10 seconds
- Mounting system: 3D printed structure, featuring a fixed structure (the "base") and a moving structure (the "axis")
