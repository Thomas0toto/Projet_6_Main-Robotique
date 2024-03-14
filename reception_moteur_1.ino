/*
* LAB Name: ESP32->ESP32 Bluetooth Communication (SLAVE CODE)
*   [ SLAVE CODE ]
* Author: Khaled Magdy
* DeepBlueMbedded 2023
* For More Info Visit: www.DeepBlueMbedded.com
*/
#include <ESP32Servo.h>
#include <Wire.h>
#include "BluetoothSerial.h"
#include "m5rotate8.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif


// Bluetooth Serial Object (Handle)
BluetoothSerial SerialBT;

Servo myservoPouce; //
Servo myservoINDEX;
Servo myservoMajeur;
Servo myservoAnnulaire;
Servo myservoAuriculaire;

char pos = 0;
char pos1 = 0;
char pos2 = 0;
char pos3 = 0;
char pos4 = 0;

M5ROTATE8 MM;
// ESP32 Bluetooth (Slave) Device Information
const char *pin = "1234";  // Change this to more secure PIN.
String device_name = "ESP32-BT-Slave";
int servoPouce = 13;
int servoINDEX = 12;
int servoMAJEUR = 27;
int servoAnnulaire = 33;
int servoAuriculaire = 15;

char receiveData[7];
#define NB_data 7

void setup() {

  myservoPouce.attach(servoPouce);
  myservoINDEX.attach(servoINDEX);
  myservoMajeur.attach(servoMAJEUR);
  myservoAnnulaire.attach(servoAnnulaire);
  myservoAuriculaire.attach(servoAuriculaire);

  Wire.begin();

  Serial.begin(115200);
  SerialBT.begin(device_name);  
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  SerialBT.setPin(pin);
  Serial.println("Using PIN");

  MM.begin();
  MM.resetAll();
}

void loop() {

  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    receiveData[0] = SerialBT.read();
    if (receiveData[0] == '#') {
      for (char cptSend = 1; cptSend < NB_data; cptSend++) {
        while (!SerialBT.available())   ;
        receiveData[cptSend] = SerialBT.read();
      }
    }
    Serial.println(receiveData[0]);
    Serial.printf("a:%d\n", receiveData[1]);
    Serial.printf("b:%d\n", receiveData[2]);
    Serial.printf("c:%d\n", receiveData[3]);
    Serial.printf("d:%d\n", receiveData[4]);
    Serial.printf("e:%d\n", receiveData[5]);
    Serial.printf("f:%d\n", receiveData[6]);

    myservoPouce.write(receiveData[1]);
    myservoINDEX.write(receiveData[2]);
    myservoMajeur.write(receiveData[3]);
    myservoAnnulaire.write(receiveData[4]);
    myservoAuriculaire.write(receiveData[5]);

    if (receiveData[0] == '#' && receiveData[6] == (receiveData[1] ^ receiveData[2] ^ receiveData[3] ^ receiveData[4] ^ receiveData[5]))
      Serial.print("Data OK\n");
    else
      Serial.print("Data INVALIDE\n");

    receiveData[0] = 0;  //on efface le buffer de reception
  }
}
