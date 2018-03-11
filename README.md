# 3D-printed Single-axis solar tracker with Energy Storage and Bluetooth Monitoring

Solar energy is converted by the solar array and charges the Battery pack. The Battery pack is supplying a controller which optimize the orientation of the solar array towards the sunlight, based on the light sensor attached to the solar array. In addition to powering the control system, the battery pack has enough capacity to concurrently charge a smartphone or a tablet. It takes approximately about 10 hours (under STC) -or about 1.5 day- to charge the battery pack completely.

![solar tracker](https://github.com/nicolasfguillaume/solar-tracker-iot-arduino/blob/master/solar-tracker-3.jpg)

## Overall technical Specs

- Solar Array: 5Wp (= 5 x 1W solar panels in parallel)
- Energy Storage: 1 x 3000 mAh Lithiun Ion battery
- Controller: Arduino Uno Rev3, featuring a sun-tracking algorithm
  * Input: light sensor (2 LDR mounted at 45 degrees)
  * Output: servo motor (9g, 1.5 kg.cm)
- Monitoring: Bluetooth communication protocol (serial UART), sending data every 10 seconds
- Mounting system: 3D printed structure, featuring a fixed structure (the "base") and a moving structure (the "axis")

## Principle

![solar tracker](https://github.com/nicolasfguillaume/3D-Solar-Tracker-IoT-Arduino/blob/master/principle.jpg)
![solar tracker](https://github.com/nicolasfguillaume/solar-tracker-iot-arduino/blob/master/solar-tracker.jpg)

## Pictures

### Solar panels

![solar tracker](https://github.com/nicolasfguillaume/3D-Solar-Tracker-IoT-Arduino/blob/master/photos/solar_panels_front.jpg)
![solar tracker](https://github.com/nicolasfguillaume/3D-Solar-Tracker-IoT-Arduino/blob/master/photos/solar_panels_back.jpg)

### Support and DC step motor

![solar tracker](https://github.com/nicolasfguillaume/3D-Solar-Tracker-IoT-Arduino/blob/master/photos/support.jpg)
![solar tracker](https://github.com/nicolasfguillaume/3D-Solar-Tracker-IoT-Arduino/blob/master/photos/support_step_motor.jpg)

## USB Charger

![solar tracker](https://github.com/nicolasfguillaume/3D-Solar-Tracker-IoT-Arduino/blob/master/usb_charger.jpeg)

## Arduino files

https://github.com/nicolasfguillaume/3D-Solar-Tracker-IoT-Arduino/blob/master/solar-tracker.c

## 3D print files

![solar tracker](https://github.com/nicolasfguillaume/solar-tracker-iot-arduino/blob/master/solar-tracker-2.jpg)

https://github.com/nicolasfguillaume/3D-Solar-Tracker-IoT-Arduino/blob/master/3d_print_files/Complete_Single_axis_Tracker_final.skp

Note : Sketchup files need to converted to STL files in order to sent to a 3D printer. You can use the following extension to do so : https://extensions.sketchup.com/en/content/sketchup-stl
