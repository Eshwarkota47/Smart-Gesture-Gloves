# Smart Gesture Glove for Assistive Communication and Device Control

## Overview
The **Smart Gesture Glove** is a wearable embedded system that detects hand gestures and converts them into meaningful digital outputs. The project is designed mainly for **assistive communication**, where users can express basic needs through simple hand movements. It can also be extended for **robotics control, home automation, and human-computer interaction**.

The glove uses:
- **Flex sensors** to detect finger bending
- **MPU6050** to detect hand tilt and motion
- **Capacitive touch sensor** to enable or disable gesture reading
- **HC-05 Bluetooth module** to send output messages wirelessly
- **Arduino Uno** to process sensor data and classify gestures

The system recognizes predefined gestures and sends related messages such as:
- `Hi, how are you?`
- `I need some water.`
- `Good morning!`
- `I am hungry.`
- `I am sick.`

---

## Problem Statement
People with speech impairments or communication difficulties may find it hard to express basic needs quickly and effectively. Traditional communication methods may require writing, screens, or assistance from another person.

This project addresses that problem by creating a **low-cost smart wearable glove** that detects specific hand gestures and converts them into text-based output messages in real time.

---

## Objectives
The main objectives of this project are:
- To design a wearable glove capable of recognizing predefined hand gestures
- To interface multiple sensors with a microcontroller
- To process sensor values in real time for gesture classification
- To transmit the detected gesture message wirelessly using Bluetooth
- To build an affordable assistive communication prototype

---

## Features
- Real-time gesture detection
- Three flex sensor inputs for finger bend tracking
- MPU6050-based motion and tilt sensing
- Touch sensor based enable/disable control
- Wireless Bluetooth communication
- Serial Monitor debugging support
- Simple rule-based gesture classification
- Low-cost and portable design

---

## Hardware Components
The following components were used in this project:

- Arduino Uno
- 3 Flex Sensors
- MPU6050 Accelerometer and Gyroscope Module
- Capacitive Touch Sensor
- HC-05 Bluetooth Module
- Resistors
- Jumper Wires
- Breadboard
- Glove
- Power Supply / USB Cable

---

## Software Requirements
- Arduino IDE
- Embedded C/C++
- `Wire.h` library
- `SoftwareSerial.h` library
- `MPU6050.h` library

---

## Working Principle
The Smart Gesture Glove works in the following stages:

1. **Sensor Input Collection**
   - Flex sensors measure the bending of fingers.
   - MPU6050 detects hand orientation and movement.
   - Touch sensor is used to toggle gesture reading on and off.

2. **Calibration**
   - At startup, the system calibrates the flex sensors and MPU6050.
   - Baseline values are stored for accurate comparison during operation.

3. **Gesture Detection**
   - The Arduino continuously reads sensor values.
   - Threshold conditions are applied to identify specific finger bends and hand tilts.
   - A rule-based logic matches the sensor pattern to a predefined gesture.

4. **Output Generation**
   - Once a gesture is identified, a text message is sent through:
     - Serial Monitor
     - Bluetooth module to a mobile device or connected system

---

## Sensor Connections

### Flex Sensors
- `Flex Sensor 1` → `A0`
- `Flex Sensor 2` → `A1`
- `Flex Sensor 3` → `A2`

### Touch Sensor
- `Touch Sensor OUT` → `D2`

### HC-05 Bluetooth Module
- `HC-05 TX` → `Arduino Pin 10`
- `HC-05 RX` → `Arduino Pin 11`
- `VCC` → `5V`
- `GND` → `GND`

### MPU6050
- `SDA` → `A4`
- `SCL` → `A5`
- `VCC` → `5V`
- `GND` → `GND`

> Note: For some HC-05 modules, voltage level protection for RX may be required.

---

## Gesture Mapping
The glove is programmed to recognize a set of gestures and map them to corresponding messages.

| Gesture Condition | Output Message |
|---|---|
| Flex and/or tilt condition 1 | Hi, how are you? |
| Flex and/or tilt condition 2 | I need some water. |
| Flex and/or tilt condition 3 | Good morning! |
| Flex and/or tilt condition 4 | I am hungry. |
| Flex and/or tilt condition 5 | I am sick. |

The exact thresholds can be adjusted in code depending on sensor calibration and hand movement.

---

## Algorithm
The project uses a **rule-based gesture detection algorithm**.

### Steps:
1. Initialize all sensors and modules
2. Calibrate MPU6050 and flex sensors
3. Read current sensor values
4. Compare values against calibrated thresholds
5. Identify the gesture based on sensor conditions
6. Send output message through Bluetooth and Serial Monitor

### Why rule-based logic?
- Lightweight and easy to implement on Arduino
- No need for high computation
- Suitable for a limited number of predefined gestures
- Easy to debug and tune manually

---

## Code Logic Summary
The Arduino code includes:
- Sensor initialization
- Bluetooth serial communication
- Flex sensor reading
- MPU6050 motion reading
- Sensor calibration
- Noise filtering
- Gesture mapping
- Message transmission

##Main libraries used:
'''cpp
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MPU6050.h>


###How to Run the Project
Step 1: Assemble the Circuit
Connect all sensors and modules to the Arduino according to the circuit diagram.

Step 2: Upload the Code
Open the Arduino sketch in Arduino IDE and upload it to the Arduino Uno.

Step 3: Open Serial Monitor
Set the baud rate to:

9600
Step 4: Calibrate the Glove
Keep the glove steady and fingers straight during startup so the system records proper baseline values.

Step 5: Perform Gestures
Bend the fingers or tilt the hand according to the programmed gesture conditions.

Step 6: Observe Output
The corresponding message will appear in:

Arduino Serial Monitor
Bluetooth-connected mobile device
Sample Serial Monitor Output
Flex1: 34 | Flex2: 1010 | Flex3: 1010 | D1: 1 | D2: -17 | D3: -18 | B1: 0 | B2: 0 | B3: 0
Gyro X: -2 Y: -6 Z: -18
Pitch: -0.74 | Roll: -3.12
Hello

Flex1: 33 | Flex2: 1004 | Flex3: 1003 | B1: 1 | B2: 0 | B3: 0
Gyro X: 278 Y: 36 Z: 238
Pitch: 4.38 | Roll: 25.02
Sent: Hi, how are you?
Applications
This project can be used in:

Assistive communication for speech-impaired individuals
Human-computer interaction
Robotics control
Virtual reality and gaming
Home automation
Smart wearable systems
Challenges Faced
Some of the major challenges during development were:

Getting stable readings from flex sensors
Calibrating MPU6050 accurately
Handling noise in real-time sensor values
Choosing reliable thresholds for different gestures
Ensuring Bluetooth communication works consistently
Future Scope
This project can be improved further by:

Adding more flex sensors for full finger tracking
Using machine learning for advanced gesture recognition
Converting messages into speech output
Integrating with IoT devices for smart home control
Supporting full sign language translation
Building a compact PCB-based wearable version
Project Outcome
The project successfully demonstrates a working smart glove prototype that can recognize hand gestures and convert them into useful output messages. It shows how low-cost embedded systems and wearable sensors can be combined to build a practical assistive communication device.

Team Members
C Vishwa - PES2UG23CS139
Dhruv Maheshwari - PES2UG23CS173
Divyansh Sharma - PES2UG23CS181
Eshwar B N - PES2UG23CS187

Conclusion
The Smart Gesture Glove is a practical embedded systems project that combines sensors, microcontroller programming, and wireless communication to solve a real-world communication problem. It demonstrates the potential of wearable technology in assistive applications and provides a strong base for future improvements.

License
This project is developed for academic and educational purposes.

