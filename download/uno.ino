/*
  I know I could just give you the entire thing, 
  but that's no fun. So add the servo code or other stuff yourself.
  Good luck!
*/

#include <SoftwareSerial.h>

SoftwareSerial espSerial(2, 3);

String label = "";

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  Serial.println("System ready, waiting for ESP32-CAM...");
}

void loop() {
  if (espSerial.available()) {
    String data = espSerial.readStringUntil('\n');
    data.trim();

    if (data.startsWith("DETECTED:")) {
      label = data.substring(9);
      Serial.print("AI Detection Received: ");
      Serial.println(label);

      if (label == "circle") {
        //circle
      } else if (label == "square") {
        //square
      }

      delay(3000);
    }
  }
}
