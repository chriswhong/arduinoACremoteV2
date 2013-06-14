/***
 * 	bitlashsocketio.ino: Bitlash-Commander Socket.IO Client for Arduino/Bitlash
 * 
 * 	Copyright (C) 2013 Bill Roy
 * 	MIT license: see LICENSE file.
 * 
 * 	This sketch listens for a Bitlash command from a socket.io server and
 * 	executes the command, returning its output to the server over the websocket.
 * 	
 * 	For testing, you will find a companion socket.io server in the file 
 * 	index.js in the same directory.
 * 
 * 	Run the server ("node index.js"), then boot up the Arduino with this sketch on it.	
 * 	Commands you type on the server console will be executed on the Arduino, 
 * 	and the resulting Bitlash output will be displayed on the server console.
 * 
 * 	You will need to adjust the hostname and port below to match your network.
 * 	By default the server runs on port 3000.
 * 
 ***/
#include "SocketIOClient.h"
#include "Ethernet.h"
#include "SPI.h"
#include "stdlib.h"
SocketIOClient client;

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char hostname[] = "67.20.99.95";
int port = 8000;
String dataStr;
int sensorPin = 0;
int lightPin = 1;
int IRledPin =  8;   
float temperatureF;

// buffer Bitlash's char-based output to cut down on sent packet count
#define OUTPUT_BUFFER_LEN 33
byte output_index = 0;
char output_buffer[OUTPUT_BUFFER_LEN];


void setup() {

  Serial.begin(9600);

  pinMode(sensorPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(IRledPin, OUTPUT);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Init Ethernet failed");
    for(;;)
      ;
  }

  client.setDataArrivedDelegate(ondata);
  if (!client.connect(hostname, port)) Serial.println(F("Not connected."));

  client.send("Hello","Client here!");
}

void loop() {
  client.monitor();


}

// transmit the output buffer
void sendbuffer(void) {
  if (output_index == 0) return;
  output_buffer[output_index] = 0;
  //Serial.println(output_buffer);
  if (client.connected()) client.send("hello",output_buffer);
  output_index = 0;
}

// bitlash serial output handler: 
// 	buffer an output character for forwarding to websocket
void sendchar(byte c) {
  output_buffer[output_index++] = c;
  if (output_index >= OUTPUT_BUFFER_LEN-1) sendbuffer();
}

// websocket message handler: do something with command from server
void ondata(SocketIOClient client, char *data) {

  Serial.println("ondata() was called");
  Serial.println(data);

  dataStr = data;
  Serial.println(dataStr);
  if(dataStr == "getStatus"){
    Serial.println("GetStatus was received");
    getTemp();
    getLight();


  }
  
  if(dataStr == "togglePower"){
    Serial.println("TogglePower was Received");
    togglePower();
  }
  

}

void getTemp(){
  int reading = analogRead(sensorPin);  
  float voltage = reading * 5.0;
  voltage /= 1024.0; 
  float temperatureC = (voltage - 0.5) * 100 ;

  temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  Serial.print(temperatureF); 
  Serial.println(" degrees F");
  String event = "temp";
  char data[5];
  client.send(event,dtostrf(temperatureF,4,0,data));

}

float getLight(){
  float lightReading = analogRead(lightPin);  

  Serial.print("Light Reading: ");
  Serial.println(lightReading);
  String event = "light";
    char data[5];
  client.send(event,dtostrf(lightReading,4,0,data));

}

void togglePower() {
  Serial.println("togglePower() was triggered");  
  pulseIR(8860);
  delayMicroseconds(4360);
  pulseIR(600);
  delayMicroseconds(1580);
  pulseIR(600);
  delayMicroseconds(500);
  pulseIR(600);
  delayMicroseconds(480);
  pulseIR(600);
  delayMicroseconds(500);
  pulseIR(600);
  delayMicroseconds(480);
  pulseIR(600);
  delayMicroseconds(500);
  pulseIR(600);
  delayMicroseconds(500);
  pulseIR(580);
  delayMicroseconds(1600);
  pulseIR(600);
  delayMicroseconds(480);
  pulseIR(600);
  delayMicroseconds(1600);
  pulseIR(600);
  delayMicroseconds(1580);
  pulseIR(600);
  delayMicroseconds(500);
  pulseIR(580);
  delayMicroseconds(520);
  pulseIR(580);
  delayMicroseconds(1600);
  pulseIR(600);
  delayMicroseconds(1580);
  pulseIR(600);
  delayMicroseconds(500);
  pulseIR(600);
  delayMicroseconds(1580);
  pulseIR(600);
  delayMicroseconds(500);
  pulseIR(580);
  delayMicroseconds(500);
  pulseIR(600);
  delayMicroseconds(500);
  pulseIR(600);
  delayMicroseconds(500);
  pulseIR(580);
  delayMicroseconds(500);
  pulseIR(600);
  delayMicroseconds(500);
  pulseIR(580);
  delayMicroseconds(1600);
  pulseIR(580);
  delayMicroseconds(520);
  pulseIR(580);
  delayMicroseconds(1600);
  pulseIR(600);
  delayMicroseconds(1600);
  pulseIR(520);
  delayMicroseconds(1660);
  pulseIR(520);
  delayMicroseconds(1660);
  pulseIR(520);
  delayMicroseconds(1680);
  pulseIR(580);
  delayMicroseconds(1600);
  pulseIR(520);
  delayMicroseconds(580);
  pulseIR(520);
  delayMicroseconds(41480);
  pulseIR(8840);
  delayMicroseconds(2200);
  pulseIR(540);
  delayMicroseconds(28564);
  pulseIR(8880);
  delayMicroseconds(2140);
  pulseIR(560);

}



void pulseIR(long microsecs) {
  // we'll count down from the number of microseconds we are told to wait

  cli();  // this turns off any background interrupts

  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
    digitalWrite(IRledPin, HIGH);  // this takes about 3 microseconds to happen
    delayMicroseconds(10);         // hang out for 10 microseconds
    digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
    delayMicroseconds(10);         // hang out for 10 microseconds

    // so 26 microseconds altogether
    microsecs -= 26;
  }

  sei();  // this turns them back on
}




