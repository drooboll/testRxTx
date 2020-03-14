#define motor_RPWM_L 2
#define motor_LPWM_L 3
#define motor_RPWM_R 4
#define motor_LPWM_R 5

#define motor_LL_EN 48
#define motor_LR_EN 49

#define motor_RL_EN 46
#define motor_RR_EN 47

#define rx_ready 45

#define servo_arm_pin 9
#define servo_claw_pin 8

#define leftSensorPin 1
#define rightSensorPin 2

#define CLAW_MAX_OPEN 50
#define CLAW_MAX_CLOSE 120 
#define ARM_MAX_UP 10
#define ARM_MAX_DOWN 60 

#define STOP 0
#define DOWN 1
#define UP 2
#define CLOSE 0
#define OPEN 1

#include <Servo.h>

#define motor_power 255

#define CW 1
#define STOP 0
#define CCW -1

int last_arm_angle;
int last_claw_angle;
int servo_step = 10;

void servoControl(int16_t arm_command,int16_t claw_command)
{
  switch (arm_command)
  {
    case DOWN: 
      last_arm_angle = (last_arm_angle + servo_step >= ARM_MAX_DOWN)?  ARM_MAX_DOWN : last_arm_angle + servo_step;
      break;
    case UP:
      last_arm_angle = (last_arm_angle - servo_step >= ARM_MAX_UP)?  ARM_MAX_UP: last_arm_angle - servo_step;
      break;
  }
  switch (claw_command)
  {
    case CLOSE:
      last_claw_angle = (last_claw_angle + servo_step >= CLAW_MAX_CLOSE)?  CLAW_MAX_CLOSE : last_claw_angle + servo_step;
      break;
    case OPEN:
      last_claw_angle = (last_claw_angle -  servo_step <= CLAW_MAX_OPEN)?  CLAW_MAX_CLOSE : last_claw_angle - servo_step;
      break;
  }
}

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

Servo arm;
Servo claw;

int lValue, rValue, vLeft, vRight;
int v0 = 60;
float k = 0.05f;

void auto_mode()
{
  lValue = analogRead(leftSensorPin);
  rValue = analogRead(rightSensorPin);
  vLeft = 1*(v0 - (lValue - rValue)*k);
  vRight = 1*(v0 + (lValue - rValue)*k);

  runMotors(vLeft, vRight);
  Serial.println("LEFT");
  Serial.println(vLeft);
  Serial.println("RIGHT");
  Serial.println(vRight);
}

void setup() {
  arm.attach(servo_arm_pin);
  claw.attach(servo_claw_pin);
  arm.write(ARM_MAX_UP);
  claw.write(CLAW_MAX_CLOSE);
  last_arm_angle = ARM_MAX_UP;
  last_claw_angle = CLAW_MAX_CLOSE;

  pinMode(rx_ready, OUTPUT);

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

  Serial.begin(115200);
  Serial1.begin(115200);
  delay(1000);
  Serial1.flush();
  digitalWrite(rx_ready, HIGH);
}

void loop() {
  
  if (Serial1.available() >= 8) {
    digitalWrite(rx_Ready, LOW);
    
    uint8_t buffer[7];
    for (uint8_t i = 0; i < 7; i++)
    {
      buffer[i] = Serial1.read();
    }
    
    uint8_t arm_command = buffer[4];
    uint8_t claw_command = buffer[5];
    int16_t mode = buffer[6];
    
    int16_t leftM = (int16_t)buffer[1] << 8 | buffer[0];
    int16_t rightM = (int16_t)buffer[3] << 8 | buffer[2];
    
    digitalWrite(rx_ready, HIGH);
    Serial1.flush();
    
    if (mode == 1)
    {
      auto_mode();
    }
    else
    {
      runMotors(leftM, rightM);
      servoControl(arm_command,claw_command); 
      // Serial.print(leftM, DEC);
      // Serial.write(' ');
      // Serial.println(rightM, DEC);
    }
  }
}
