#include <SPI.h>
#include "nRF24L01.h"
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.begin(9600);
}
void loop() {
  char buffer[4];
  bool readyToSend = false;
  if (Serial.available() >= 4){
    for (uint8_t i = 0; i < 4; ++i){
      buffer[i] = Serial.read();  
    }
    readyToSend = true;
  }
  if (readyToSend){
    radio.write(buffer, 4);
    readyToSend = 0;
  }
}
