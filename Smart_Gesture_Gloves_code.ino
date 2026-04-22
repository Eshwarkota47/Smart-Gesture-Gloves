#include <Wire.h>
#include <SoftwareSerial.h>
#include <MPU6050.h>
#include <math.h>

// === Flex Sensors ===
const int flexPin1 = A0;
const int flexPin2 = A1;
const int flexPin3 = A2;
int flexValue1 = 0;
int flexValue2 = 0;
int flexValue3 = 0;

// === Flex Rest Values (from your latest serial readings) ===
const int flexBase1 = 33;
const int flexBase2 = 1011;
const int flexBase3 = 1010;

// === Flex Thresholds ===
const int flexThreshold1 = 4;
const int flexThreshold2 = 4;
const int flexThreshold3 = 4;

// === Bluetooth ===
SoftwareSerial bluetooth(10, 11);

// === MPU6050 ===
MPU6050 mpu(0x68);

// === Touch Sensor ===
const int touchPin = 2;
bool readEnabled = true;
bool lastTouchState = LOW;

// === MPU Offsets ===
int16_t gxOffset = 0, gyOffset = 0, gzOffset = 0;
int16_t axOffset = 0, ayOffset = 0, azOffset = 0;

// === Tilt Calibration ===
float pitchOffset = 0.0;
float rollOffset = 0.0;

// === Tilt Thresholds ===
const float leftTiltThreshold = -15.0;
const float rightTiltThreshold = 15.0;
const float forwardTiltThreshold = -12.0;
const float strongForwardTiltThreshold = -25.0;
const float backwardTiltThreshold = 12.0;

// === Garbage Filter ===
const int gyroThreshold = 400;
const int accelerationThreshold = 20000;

bool i2cDevicePresent(uint8_t address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}

void checkMPUConnection() {
  bool found68 = i2cDevicePresent(0x68);
  bool found69 = i2cDevicePresent(0x69);

  Serial.print("I2C 0x68: ");
  Serial.println(found68 ? "FOUND" : "NOT FOUND");
  Serial.print("I2C 0x69: ");
  Serial.println(found69 ? "FOUND" : "NOT FOUND");

  if (!found68 && !found69) {
    Serial.println("MPU6050 not detected. Check VCC, GND, SDA, SCL.");
    bluetooth.println("MPU6050 not detected");
    while (true) {
      delay(1000);
    }
  }

  if (!found68 && found69) {
    Serial.println("MPU6050 detected at 0x69.");
    Serial.println("Change MPU6050 mpu(0x68) to MPU6050 mpu(0x69).");
    bluetooth.println("MPU at 0x69. Change code.");
    while (true) {
      delay(1000);
    }
  }

  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 found, but testConnection() failed.");
    bluetooth.println("MPU testConnection failed");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("MPU6050 connection OK.");
  bluetooth.println("MPU6050 connection OK.");
}

void computeTiltAngles(int16_t ax, int16_t ay, int16_t az, float &pitch, float &roll) {
  pitch = atan2((float)-ax, sqrt((float)ay * (float)ay + (float)az * (float)az)) * 180.0 / PI;
  roll = atan2((float)ay, (float)az) * 180.0 / PI;
}

void calibrateMPU() {
  Serial.println("Calibrating MPU6050... Keep glove STILL and FLAT.");
  bluetooth.println("Calibrating MPU... Keep still.");

  long gxSum = 0, gySum = 0, gzSum = 0;
  long axSum = 0, aySum = 0, azSum = 0;
  const int samples = 200;

  for (int i = 0; i < samples; i++) {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    axSum += ax;
    aySum += ay;
    azSum += az;
    gxSum += gx;
    gySum += gy;
    gzSum += gz;
    delay(5);
  }

  gxOffset = gxSum / samples;
  gyOffset = gySum / samples;
  gzOffset = gzSum / samples;
  axOffset = axSum / samples;
  ayOffset = aySum / samples;
  azOffset = (azSum / samples) - 16384;

  Serial.print("Gyro Offsets -> X: "); Serial.print(gxOffset);
  Serial.print(" Y: "); Serial.print(gyOffset);
  Serial.print(" Z: "); Serial.println(gzOffset);

  Serial.print("Accel Offsets -> X: "); Serial.print(axOffset);
  Serial.print(" Y: "); Serial.print(ayOffset);
  Serial.print(" Z: "); Serial.println(azOffset);

  Serial.println("MPU calibration complete!");
  bluetooth.println("MPU calibration done.");
}

void calibrateTilt() {
  Serial.println("Calibrating tilt... Keep glove in neutral position.");
  bluetooth.println("Calibrating tilt... Keep neutral.");

  float pitchSum = 0.0;
  float rollSum = 0.0;
  const int samples = 100;

  for (int i = 0; i < samples; i++) {
    int16_t rawAx, rawAy, rawAz, rawGx, rawGy, rawGz;
    float pitch, roll;

    mpu.getMotion6(&rawAx, &rawAy, &rawAz, &rawGx, &rawGy, &rawGz);

    int16_t ax = rawAx - axOffset;
    int16_t ay = rawAy - ayOffset;
    int16_t az = rawAz - azOffset;

    computeTiltAngles(ax, ay, az, pitch, roll);

    pitchSum += pitch;
    rollSum += roll;
    delay(5);
  }

  pitchOffset = pitchSum / samples;
  rollOffset = rollSum / samples;

  Serial.print("Tilt Offsets -> Pitch: "); Serial.print(pitchOffset);
  Serial.print(" Roll: "); Serial.println(rollOffset);
  bluetooth.println("Tilt calibration done.");
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  pinMode(flexPin1, INPUT);
  pinMode(flexPin2, INPUT);
  pinMode(flexPin3, INPUT);
  pinMode(touchPin, INPUT);

  Wire.begin();
  delay(100);

  checkMPUConnection();
  calibrateMPU();
  calibrateTilt();

  Serial.print("Using Flex Rest Values -> F1: ");
  Serial.print(flexBase1);
  Serial.print(" F2: ");
  Serial.print(flexBase2);
  Serial.print(" F3: ");
  Serial.println(flexBase3);

  Serial.println("System Ready.");
  bluetooth.println("System Ready.");
}

void loop() {
  bool touchState = digitalRead(touchPin);
  if (touchState == HIGH && lastTouchState == LOW) {
    readEnabled = !readEnabled;
    Serial.println(readEnabled ? "Reading ENABLED" : "Reading DISABLED");
    bluetooth.println(readEnabled ? "Sensor reading ENABLED" : "Sensor reading DISABLED");
    delay(1000);
  }
  lastTouchState = touchState;

  if (!readEnabled) {
    delay(300);
    return;
  }

  // === Flex Readings ===
  flexValue1 = analogRead(flexPin1);
  flexValue2 = analogRead(flexPin2);
  flexValue3 = analogRead(flexPin3);

  int delta1 = flexValue1 - flexBase1;
  int delta2 = flexValue2 - flexBase2;
  int delta3 = flexValue3 - flexBase3;

  bool flex1Bent = abs(delta1) >= flexThreshold1;
  bool flex2Bent = abs(delta2) >= flexThreshold2;
  bool flex3Bent = abs(delta3) >= flexThreshold3;

  // === MPU Readings ===
  int16_t rawAx, rawAy, rawAz, rawGx, rawGy, rawGz;
  mpu.getMotion6(&rawAx, &rawAy, &rawAz, &rawGx, &rawGy, &rawGz);

  if (rawAx == 0 && rawAy == 0 && rawAz == 0 &&
      rawGx == 0 && rawGy == 0 && rawGz == 0) {
    Serial.println("MPU data is all zero. Check wiring.");
    bluetooth.println("MPU data all zero");
    delay(500);
    return;
  }

  int16_t ax = rawAx - axOffset;
  int16_t ay = rawAy - ayOffset;
  int16_t az = rawAz - azOffset;
  int16_t gx = rawGx - gxOffset;
  int16_t gy = rawGy - gyOffset;
  int16_t gz = rawGz - gzOffset;

  float pitch, roll;
  computeTiltAngles(ax, ay, az, pitch, roll);
  pitch -= pitchOffset;
  roll -= rollOffset;

  Serial.print("Flex1: "); Serial.print(flexValue1);
  Serial.print(" | Flex2: "); Serial.print(flexValue2);
  Serial.print(" | Flex3: "); Serial.print(flexValue3);
  Serial.print(" | D1: "); Serial.print(delta1);
  Serial.print(" | D2: "); Serial.print(delta2);
  Serial.print(" | D3: "); Serial.print(delta3);
  Serial.print(" | B1: "); Serial.print(flex1Bent);
  Serial.print(" | B2: "); Serial.print(flex2Bent);
  Serial.print(" | B3: "); Serial.println(flex3Bent);

  Serial.print("Gyro X: "); Serial.print(gx);
  Serial.print(" Y: "); Serial.print(gy);
  Serial.print(" Z: "); Serial.println(gz);

  Serial.print("Pitch: "); Serial.print(pitch);
  Serial.print(" | Roll: "); Serial.println(roll);

  if (abs(gx) > gyroThreshold || abs(gy) > gyroThreshold || abs(gz) > gyroThreshold ||
      abs(ax) > accelerationThreshold || abs(ay) > accelerationThreshold || abs(az) > accelerationThreshold) {
    Serial.println("Garbage values detected - skipping.");
    bluetooth.println("Command: Garbage Value Detected from MPU");
    delay(1000);
    return;
  }

  if (pitch <= strongForwardTiltThreshold) {
    bluetooth.println("I am hungry.");
    Serial.println("Sent: I am hungry.");
  }
  else if (roll >= rightTiltThreshold) {
    bluetooth.println("I am sick.");
    Serial.println("Sent: I am sick.");
  }
  else if (flex1Bent || roll <= leftTiltThreshold) {
    bluetooth.println("Hi, how are you?");
    Serial.println("Sent: Hi, how are you?");
  }
  else if (flex2Bent || pitch <= forwardTiltThreshold) {
    bluetooth.println("I need some water.");
    Serial.println("Sent: I need some water.");
  }
  else if (flex3Bent || pitch >= backwardTiltThreshold) {
    bluetooth.println("Good morning!");
    Serial.println("Sent: Good morning!");
  }
  else {
    Serial.println("Hello");
  }

  delay(500);
}
