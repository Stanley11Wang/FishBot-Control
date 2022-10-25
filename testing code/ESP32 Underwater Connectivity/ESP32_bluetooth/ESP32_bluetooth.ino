//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define ONBOARD_LED 13

BluetoothSerial SerialBT;

int msg_in = 0;

void setup() {
  pinMode(ONBOARD_LED,OUTPUT);
  
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    msg_in = SerialBT.read();
    if (msg_in == 48) {
      Serial.println("LOW");
      digitalWrite(ONBOARD_LED,LOW);
    }
    else if (msg_in == 49) {
      Serial.println("HIGH");
      digitalWrite(ONBOARD_LED,HIGH);
    }
    Serial.println(msg_in);
  }
  delay(20);
}
