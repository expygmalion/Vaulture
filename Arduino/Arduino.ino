#include <Servo.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo motor;
const int mot = A0;
const int buz = 11;
const int trigPin = A2;
const int echoPin = A1;
const int ldrPin = A3;
const int ledPin = 10;

String password = "1234";
String hint = "4";
String input = "";
int incorrectCount = 0;
int successCount = 0;
bool doorLocked = true;
int ldrValue = 0;
int threshold = 500;

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void clearInput() {
  input = "";
  lcd.clear();
}

void displayHint() {
  lcd.clear();
  displayMessage("Hint: ***" + hint, 4000);
  displayMessage("Enter Password", 2000);
}

void checkPassword() {
  if (input == password && isSomeoneInFront()) {
    unlockDoor();
  } else if (input != password) {
    handleIncorrectPassword();
  } else {
    displayMessage("No one at door", 2000);
  }
}

void signalBuzzer(int times = 3) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buz, HIGH);
    delay(100);
    digitalWrite(buz, LOW);
    delay(100);
  }
}

void buzzOnKeyPress() {
  digitalWrite(buz, HIGH);
  delay(50);
  digitalWrite(buz, LOW);
}

void unlockDoor() {
  displayMessage("Password Correct", 1000);
  digitalWrite(buz, HIGH);
  displayMessage("Door Unlocked", 1000);
  motor.write(90);
  delay(1000);
  digitalWrite(buz, LOW);
  doorLocked = false;
  input = "";
  incorrectCount = 0;
  successCount++;
  Serial.println("Door Unlocked");  // Send message to NodeMCU
}

void lockDoor() {
  motor.write(0);
  displayMessage("Door Locked", 2000);
  doorLocked = true;
  input = "";
  Serial.println("Door Locked");  // Send message to NodeMCU
}

void handleIncorrectPassword() {
  incorrectCount++;
  displayMessage("Password Incorrect", 1000);

  if (incorrectCount < 3) {
    signalBuzzer();
    displayMessage("Try Again", 1000);
  } else {
    signalBuzzer(5);
    displayMessage("Wait 1 MIN", 60000);
    incorrectCount = 0;
  }

  displayMessage("Enter Password", 2000);
  input = "";
}

void changePassword() {
  displayMessage("Enter New Password", 2000);

  input = "";
  while (input.length() < 4) {
    char inputch = customKeypad.getKey();
    if (inputch >= '0' && inputch <= '9') {
      lcd.write('*');
      input += inputch;
    }
  }

  password = input;
  hint = input.substring(3);
  displayMessage("Password Changed", 2000);
  displayMessage("Enter Password", 2000);
  input = "";
}

void displayMessage(String message, int delayTime) {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (message.length() > 16) {
    String line1 = message.substring(0, 16);
    String line2 = message.substring(16);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
  } else {
    lcd.print(message);
  }

  delay(delayTime);
  lcd.clear();
}

bool isSomeoneInFront() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration * 0.034) / 2;

  return (distance > 0 && distance <= 20);
}

void smartLighting() {
  ldrValue = analogRead(ldrPin);

  if (ldrValue < threshold) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void setup() {
  Serial.begin(9600); // Initialize serial communication with NodeMCU
  Wire.begin();

  motor.attach(mot);
  motor.write(0);
  doorLocked = true;

  lcd.init();
  lcd.backlight();
  displayMessage("Enter Password", 2000);

  pinMode(buz, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  smartLighting();

  while (isSomeoneInFront()) {
    smartLighting();

    char inputch = customKeypad.getKey();

    if (inputch) {
      buzzOnKeyPress();

      if (inputch == 'A') {
        lockDoor();
      } else if (doorLocked) {
        if (inputch >= '0' && inputch <= '9' && input.length() < 4) {
          lcd.write('*');
          input += inputch;
        } else if (inputch == '#') {
          clearInput();
        } else if (inputch == '*' && successCount > 0) {
          displayHint();
        }

        if (input.length() == 4) {
          checkPassword();
        } else if (inputch == '#' && successCount > 0) {
          changePassword();
        }
      } else {
        displayMessage("Door Unlocked", 1000);
      }
    }
  }

  if (!isSomeoneInFront() && !doorLocked) {
    lockDoor(); // fail safe (door is always being locked)
    smartLighting();
    digitalWrite(buz, HIGH);
    delay(100);
    digitalWrite(buz, LOW);
    delay(100);

    digitalWrite(buz, HIGH);
    delay(100);
    digitalWrite(buz, LOW);
    delay(300);

    digitalWrite(buz, HIGH);
    delay(150);
    digitalWrite(buz, LOW);
    delay(150);

    digitalWrite(buz, HIGH);
    delay(150);
    digitalWrite(buz, LOW);
    delay(1000);
  }
}
