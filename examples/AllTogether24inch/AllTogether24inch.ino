/*

Ricardo Mena C
ricardo@crcibernetica.com
http://crcibernetica.com

  This example code is in public domain

*/

#include <SoftwareSerial.h>
#include <Nextion.h>

SoftwareSerial nextion(2, 3);// Nextion TX to pin 2 and RX to pin 3 of Arduino

Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

boolean button1State;
boolean button2State;

// ------------------EV-PG-BT-TY-END
String eventP0B1R = "65 00 01 00 FF FF FF"; // Event: Page 0, Button 1, Release
String eventP0B2R = "65 00 02 00 FF FF FF"; // Event: Page 0, Button 2, Release

int led1 = 13;
int led2 = 12;

// sensor "progress" bar
int x = 161;
int y = 198;
int width = 42;
int height = 105;

int old_sensor_value = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  myNextion.init();
}

void loop() {

  String message = myNextion.listen();

  if (message != "") { // if a message is received...
    Serial.println(message); //...print it out
  }

  if (message == eventP0B1R) {
    myNextion.buttonToggle(button1State, "b0", 0, 2);
    digitalWrite(led1, button1State);
  }

  if (message == eventP0B2R) {
    myNextion.buttonToggle(button2State, "b1", 0, 2);
    digitalWrite(led2, button2State);
  }

  int sensor = 0;

  for (int i = 0; i < 10 ; i ++) { // get average
    sensor += analogRead(A0);
  }
  sensor = sensor / 10;
  if (abs(sensor - old_sensor_value) > 20) {
    old_sensor_value = sensor;

    int scaled_value = map(sensor, 0, 1023, 0, 100); // always map value from 0 to 100

    myNextion.updateProgressBar(x, y, width, height, scaled_value, 0, 2, 1); // update the progress bar
    myNextion.setComponentText("t0", String(sensor));  // update text using original sensor value

  }
}