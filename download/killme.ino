#include <SoftwareSerial.h>
#include <Servo.h>

const int  servoScanPin = 9, servoPathPin = 10, ultraTrigPin = 6, ultraEchoPin = 7;

Servo servoScan;
Servo servoPath;

SoftwareSerial espSerial(2, 3);

bool activateDetect = true;
bool activateMove = false;

// Global variables for sensor readings and detection label
long duration = 0;
float distance = 0.0;
String label = "";

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  pinMode(ultraTrigPin, OUTPUT);
  pinMode(ultraEchoPin, INPUT);
  servoScan.attach(servoScanPin);
  servoPath.attach(servoPathPin);

  delay(50);
  servoScan.write(50);
  servoPath.write(90);

  Serial.println("System ready, waiting for ESP32-CAM...");
}

void loop() {
  // 1. Detection Phase
  if (activateDetect) {
    detection();

    // Check if object is within 3 cm to 7 cm range
    if (distance <= 50) {
      Serial.print("Object detected in range: ");
      Serial.print(distance);
      Serial.print(" cm ");
      Serial.print("| ");
      Serial.println(espSerial.available());

      
      servoPath.write(90);
      servoScan.write(50);

      // Check for incoming ESP32-CAM data
      if (espSerial.available()) {
        delay(4000);
        String data = espSerial.readStringUntil('\n');
        data.trim();

        if (data.startsWith("DETECTED:")) {
          label = data.substring(9);
          Serial.print("AI Detection Received: ");
          Serial.println(label);

          delay(2000);
          activateDetect = false;
          activateMove = true;
        }
      }
    }
  }

  // 2. Movement Phase
  if (activateMove) {
    moveShit(); // Reset state after executing movement
  }
}

void detection() {
  digitalWrite(ultraTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraTrigPin, LOW);

  duration = pulseIn(ultraEchoPin, HIGH);
  distance = (duration * 0.0343) / 2;
  delay(100);
}

void moveShit() {
  servoScan.write(15);
  Serial.println("inert");
  
  if (label == "circle") {
    servoPath.write(110);
  } else if (label == "square") {
    servoPath.write(60);
  }

  delay(5000);
  activateDetect = true;
  activateMove = false;
}