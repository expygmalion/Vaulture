# Vaulture
Vaulture is a Smart Lock IoT system
=======
documentation_content = """
# Smart Door Lock System with Keypad and Sensors

## Overview

This code implements a **Smart Door Lock System** using an Arduino, a keypad, a servo motor, a buzzer, a distance sensor (ultrasonic), and an LDR sensor (for smart lighting). The system allows the user to lock/unlock the door using a password entered via a 4x4 keypad, with feedback provided via an LCD. Additionally, it incorporates smart lighting that responds to ambient light levels and includes features such as password changes, door locking/unlocking, and incorrect password handling.

The **Smart Door Lock System** uses an **ESP8266** for connectivity and control, combined with sensors and actuators for its core functionality. The ESP8266 facilitates both local control via hardware and remote management using a web interface.



## Arduino Board's Key Features

- **Password Protection**: The door remains locked unless the correct password is entered.
- **Keypad Input**: Users enter their password using a 4x4 matrix keypad.
- **Servo Motor Control**: Unlocks the door when the correct password is entered.
- **Buzzer Feedback**: The buzzer sounds for incorrect password entries and other interactions.
- **LCD Display**: Displays messages to guide users, show hints, and provide feedback on actions.
- **Distance Sensor (Ultrasonic)**: Detects whether someone is standing in front of the door.
- **Smart Lighting**: Uses a Light Dependent Resistor (LDR) to control an LED based on ambient light levels.

## ESP8266 Key Features

- **WiFi Connectivity**: The ESP8266 connects to a local WiFi network to enable remote control and email notifications.
- **Web Server**: Hosts a simple web interface for password submission and status checks.
- **Email Notification**: Sends alerts for incorrect password attempts and password changes.


## Hardware Setup

1. **Servo Motor** (`motor`): Controls the locking/unlocking of the door.
   - **Pin**: `A0`
2. **Buzzer** (`buz`): Provides audio feedback for user interactions.
   - **Pin**: `11`
3. **Ultrasonic Distance Sensor** (`trigPin`, `echoPin`): Detects presence in front of the door.
   - **Trig Pin**: `A2`
   - **Echo Pin**: `A1`
4. **Light Dependent Resistor (LDR)** (`ldrPin`): Monitors ambient light levels for controlling smart lighting.
   - **Pin**: `A3`
5. **LED** (`ledPin`): Provides smart lighting functionality.
   - **Pin**: `10`
6. **LCD Display** (`lcd`): A 16x2 LCD with I2C interface for displaying messages to the user.
   - **I2C Address**: `0x27`
7. **Keypad** (`customKeypad`): 4x4 matrix keypad for user input.
   - **Pins**: `9, 8, 7, 6` for rows; `5, 4, 3, 2` for columns.
8. **ESP8266** (`Serial`):
   - Connects to the Arduino using SoftwareSerial for data exchange.
   - **Pins**: `D6` (RX) and `D5` (TX).

## Global Variables

- `password`: The password used to unlock the door.
- `hint`: The hint for the password, revealing the last digit.
- `input`: The current input entered by the user on the keypad.
- `incorrectCount`: Tracks the number of incorrect password attempts.
- `successCount`: Tracks the number of successful password entries.
- `doorLocked`: A boolean flag that determines if the door is locked or unlocked.
- `ldrValue`: The current reading from the LDR sensor to monitor ambient light levels.
- `threshold`: The light threshold to trigger the LED for smart lighting.

## Core Functions

1. **clearInput()**
   - Resets the user input and clears the LCD screen.

2. **displayHint()**
   - Displays a hint for the password (last digit) on the LCD for a short duration.

3. **checkPassword()**
   - Compares the entered password with the stored password.
   - Unlocks the door if the correct password is entered and there is someone in front of the door.

4. **signalBuzzer()**
   - Activates the buzzer for a specified number of times to indicate incorrect password attempts or other notifications.

5. **buzzOnKeyPress()**
   - Provides a quick buzz whenever a key is pressed on the keypad.

6. **unlockDoor()**
   - Unlocks the door by setting the servo motor to 90 degrees.
   - Displays a success message on the LCD and sends a signal via serial communication to NodeMCU.

7. **lockDoor()**
   - Locks the door by setting the servo motor back to 0 degrees.
   - Displays a locked message on the LCD and sends a signal to NodeMCU.

8. **handleIncorrectPassword()**
   - Manages incorrect password attempts. After three incorrect attempts, the system locks the user out for one minute.

9. **changePassword()**
   - Allows the user to change the password by entering a new one on the keypad.

10. **displayMessage()**
    - Displays a message on the LCD with automatic scrolling if the message length exceeds 16 characters.
    - Provides visual feedback to the user.

11. **isSomeoneInFront()**
    - Uses the ultrasonic sensor to determine if someone is in front of the door.
    - Returns `true` if the distance is less than 20 cm, indicating presence.

12. **smartLighting()**
    - Monitors ambient light levels using the LDR sensor.
    - Turns the LED on if the light level is below the threshold, otherwise turns it off.

## Setup and Initialization

- **Serial Communication**: The system begins by initializing the serial communication for interaction with NodeMCU at a baud rate of 9600.
- **Servo Motor**: The servo motor is attached to pin `A0` and initially set to 0 degrees (door locked).
- **LCD Setup**: The LCD is initialized and the backlight is enabled. A message prompts the user to enter the password.
- **Pin Modes**: The pins for the buzzer, ultrasonic sensor, LDR, and LED are set up accordingly.

## WiFi Configuration

   - Sets up a **Soft Access Point (AP)** with default credentials.
     - **SSID**: `TEST`
     - **Password**: `12345678`
   - Configures WiFi as a client for network access using the following credentials:
     - **SSID**: `12345678`
     - **Password**: `11223344`

## Email Notifications

   - Sends alerts via **SMTP** using Gmail's server (`smtp.gmail.com`).
   - Port: `465` (SSL).
   - Example Notifications:
     - Warning for more than 3 incorrect password attempts.
     - Confirmation of password changes.

## ESP8266 Web Server

  - `/` (Root): Displays the password submission form.
  - `/submit`: Processes the submitted password and provides feedback.

## Main Loop

1. **Smart Lighting**: Continuously monitors ambient light levels using the LDR and controls the LED.
2. **User Interaction**: Checks if someone is standing in front of the door using the ultrasonic sensor. If true, it processes keypad input.
   - If the user enters a key, the system processes it:
     - If the key is 'A', the door is locked.
     - If the door is locked and the input is numeric, it adds the number to the entered password.
     - If the input is the '#' key, the input is cleared.
     - If the input is '*', a hint for the password is displayed (only after one successful entry).
     - When the password is entered fully (4 digits), the system checks it.
   - If the input is incorrect, it handles the incorrect password attempt.
   - If the correct password is entered, the door unlocks.
3. **Fail Safe**: If no one is detected in front of the door, the system locks the door again and provides an audio signal via the buzzer.

## License

MIT License

## Author

Taj Othman
"""

