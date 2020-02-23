#define motor_RPWM_L 10
#define motor_LPWM_L 12
#define motor_RPWM_R 11
#define motor_LPWM_R 13

#define motor_LL_EN 48
#define motor_LR_EN 49

#define motor_RL_EN 46
#define motor_RR_EN 47

#define servo_d_pin 10
#define servo_u_pin 11

#include <Servo.h>

#define motor_power 255

#define CW 1
#define STOP 0
#define CCW -1

void runMotors(int16_t leftM, int16_t rightM)
{
  if (leftM >= 0)
  {
    analogWrite(motor_RPWM_L, 0);
    analogWrite(motor_LPWM_L, abs(leftM));
  }
  else 
  {
     analogWrite(motor_RPWM_L, abs(leftM));
     analogWrite(motor_LPWM_L, 0);
  }

  if (rightM >= 0)
  {
    analogWrite(motor_RPWM_R, 0);
    analogWrite(motor_LPWM_R, abs(rightM));
  }
  else 
  {
     analogWrite(motor_RPWM_R, abs(rightM));
     analogWrite(motor_LPWM_R, 0);
  }
}

Servo servo_u;
Servo servo_d;

#include <SPI.h>
#include "nRF24L01.h"
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup() {

  pinMode(motor_RPWM_L, OUTPUT);
  pinMode(motor_LPWM_L, OUTPUT);
  pinMode(motor_RPWM_R, OUTPUT);
  pinMode(motor_LPWM_R, OUTPUT);

  pinMode(motor_LL_EN, OUTPUT);
  pinMode(motor_LR_EN, OUTPUT);

  pinMode(motor_RL_EN, OUTPUT);
  pinMode(motor_RR_EN, OUTPUT);

  digitalWrite(motor_LL_EN, HIGH);
  digitalWrite(motor_LR_EN, HIGH);

  digitalWrite(motor_RL_EN, HIGH);
  digitalWrite(motor_RR_EN, HIGH);

  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}



void loop() {
  
  if (radio.available()) {
    char buffer[4];
    radio.read(buffer, 4); // Add more for buttons
    int16_t leftM = buffer[1] << 8 | buffer[0];
    int16_t rightM = buffer[3] << 8 | buffer[2];
    Serial.print(leftM, DEC);
    Serial.write(' ');
    Serial.println(rightM, DEC);

    runMotors(leftM, rightM);
  }
}
