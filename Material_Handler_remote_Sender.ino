#include "SSD1306.h"
# include <esp_now.h>
# include <WiFi.h>
#include <SPI.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SDA 21
#define SCL 22

SSD1306 display(0x3c, SDA, SCL);

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x3C, 0x61, 0x05, 0x14, 0x56, 0x18};
int progress = 0;

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 34;

// variable for storing the potentiometer value
int potValue = 0;
float power = 0;

// set pin numbers
const int buttonPin1 =  36;    // the number of the button pin
const int buttonPin2 =  35;    // the number of the switch pin
const int buttonPin3 =  25;    // the number of the LED light pin


// variable for storing the pushbutton status 
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

// Structure example to send data
// Must match the receiver structure
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

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup(){
  display.init();
display.setFont(ArialMT_Plain_10);
display.drawString(0, 0, "Hello World");
   // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // initialize the pushbutton pin as an input
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
   pinMode(buttonPin3, INPUT);
 

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //set up oled Display on i2c 0x3c
  // if(!display.init()); { // Address 0x3D for 128x64
 //   Serial.println(F("SSD1306 allocation failed"));
 //   for(;;);
 // }

   // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

}
 
void loop(){
  // read the state of the pushbutton value
  buttonState1 = digitalRead(buttonPin1);
  Serial.println(buttonState1);
  buttonState2 = digitalRead(buttonPin2);
  Serial.println(buttonState2);
  buttonState3 = digitalRead(buttonPin3);
  Serial.println(buttonState3);
  
// Reading potentiometer value
  potValue = analogRead(potPin);
  Serial.println(potValue);
  
  
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  if( buttonState1 == HIGH){
  myData.b = false;}
  else
  {myData.b = true;}
  
  myData.c = potValue;
  
  myData.d = "Hello";
  if (buttonState2 == HIGH){
    myData.e = false;
  }
  else
  {
    myData.e = true;
  }
   
  if (buttonState3 == HIGH){
    myData.f = false;
  }
  else
  {
    myData.f = true;
  }

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }

      display.clear();
  //display.setTextSize(1);
  //display.setTextColor(WHITE);
 //// display.setCursor(0, 10);
  // Display static text
 // display.println("Roller Speed is");

 //Display Progress Bar
  display.setFont(ArialMT_Plain_10);
display.drawString(10, 5, "Roller Speed is");
display.drawString(85, 5, String(int(power)));
display.drawString(105, 5, "%");

// dirplay direction of Roller
display.setFont(ArialMT_Plain_10);
display.drawString(30, 40, "Direction");
if (buttonState3 == HIGH) {
  display.drawString(85, 40, "A");
}
else{
   display.drawString(85, 40, "V");
}



////  display.setCursor(95, 10);
  if (potValue >0){
    power = float(potValue)/4095*100;
//    display.println(int(power));
  //display.println(potValue/4095);
  }
  else
  {
    display.println(0);
    }
  //// display.setCursor(115, 10);
      display.println("%");
  
   display.drawProgressBar(10,20,100,20,power);
  
      
  display.display(); 
 
      delay(200);
}
