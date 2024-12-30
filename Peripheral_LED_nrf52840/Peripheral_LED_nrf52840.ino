/*
  LED

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to control an LED.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  ---------------------------------------
  P0.02.    A0 D0 |  USB | 5V
  P0.03     A1 D1 |------| GND
  P0.28     A2 D2 | nRF. | 3V3
  P0.29.    A3 D3 | Chip.| D10 MOSI P1.15
  P0.04 SDA A4 D4 |.     | D9  MISO P1.14
  P0.05 SCL A5 D5 |.     | D8  SCK  P1.13
  P1.11 TX.    D6 |.     | D7  RX   P1.12
  ---------------------------------------
*/

#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // pin to use for the LED
const int extled9 = 9;
const int extled10 = 10;
void setup() {
  Serial.begin(9600);
  while (!Serial);
 // int i=0;
  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);
  pinMode(extled9, OUTPUT);
  pinMode(extled10, OUTPUT);
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();
  int i=0;
  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   // any value other than 0
          Serial.println("LED off");
          digitalWrite(ledPin, LOW);         // will turn the LED on
          digitalWrite(extled9, LOW);
              while(i < 10){
              digitalWrite(extled10, HIGH);         // will turn the LED off
              delay(500);
              digitalWrite(extled10, LOW); 
              delay(500);        // will turn the LED off
            }
     
        } else {                              // a 0 value
          Serial.println(F("LED on"));
          digitalWrite(ledPin, HIGH); 
          digitalWrite(extled9, HIGH);         // will turn the LED off
          digitalWrite(extled10, HIGH);         // will turn the LED off

          }
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }

