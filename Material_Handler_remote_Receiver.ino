/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

const int fbwardPin = 32;
const int startStopPin = 33;
const int ledLightPin = 34;

// variable for storing the pushbutton status 
int newPinState1 = 0;
int newPinState2 = 0;
int newPinState3 = 0;
int oldPinState1 = 0;
int oldPinState2 = 0;
int oldPinState3 = 0;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    bool b;
    float c;
    String d;
    bool e;
    bool f;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Bool: ");
  Serial.println(myData.b);
  Serial.print("Float: ");
  Serial.println(myData.c);
  Serial.print("String: ");
  Serial.println(myData.d);
  Serial.print("Bool: ");
  Serial.println(myData.e);
  Serial.print("Bool: ");
  Serial.println(myData.f);

}
 
void setup() {

  // initialize the bfwordpin as output
  pinMode(fbwardPin, OUTPUT);
  pinMode(startStopPin, OUTPUT);


    
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

 /*  // set the output of the forward backward pin
  if( myData.b == true && newPinState1 == 0){
    digitalWrite(fbwardPin, HIGH);
    newPinState1 = 1;
  }
  else{
  digitalWrite(fbwardPin, LOW);
  newPinState1 = 0;
  }
  // set the output of the start stop pin
  if( myData.e == true && newPinState2 == 0){
    digitalWrite(startStopPin, HIGH);
    newPinState2 = 1;
  }
  else{
  digitalWrite(startStopPin, LOW);
  newPinState2 = 0;
  }
 // set the output of the LedLight On/Off pin
  if( myData.f == true && newPinState3 == 0){
    digitalWrite(ledLightPin, HIGH);
    newPinState3 = 1;
  }
  else{
  digitalWrite(ledLightPin, LOW);
  newPinState3 = 0;
  }
  */

   // set the output of the forward backward pin
  if( myData.b == true ){
    digitalWrite(fbwardPin, HIGH);
  }
  else
  {
      digitalWrite(fbwardPin, LOW);
      }
 
  // set the output of the start stop pin
  if( myData.e == true ) {
    digitalWrite(startStopPin, HIGH); 
  }
   else
  {
      digitalWrite(fbwardPin, LOW);
      }
 // set the output of the LedLight On/Off pin
  if( myData.f == true ){
    digitalWrite(ledLightPin, HIGH);
  }
  else
  {
      digitalWrite(fbwardPin, LOW);
      }
 
}
