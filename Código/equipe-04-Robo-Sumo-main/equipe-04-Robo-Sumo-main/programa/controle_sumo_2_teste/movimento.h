#include <ESP32Servo.h>

#define motorD_pin 32
#define motorE_pin 33

#define _MAX_JOYSTICK_VALUE 508
#define _MIN_JOYSTICK_VALUE -512

Servo motorD, motorE;

//! FUNÇÕES DE MOVIMENTAÇÃO
void setupPinsMotor(){
	motorD.attach(motorD_pin);
	motorE.attach(motorE_pin);
}

void movimento_Y(int vel){
  vel = map(vel, _MIN_JOYSTICK_VALUE, _MAX_JOYSTICK_VALUE, 180, 0);
  motorE.write(vel); motorD.write(vel);
  Serial.printf("Movendo em Y com vel = %d\n", vel);
}

void esquerda(int vel){
  vel = map(vel, _MIN_JOYSTICK_VALUE, _MAX_JOYSTICK_VALUE, 0, 180);
  motorE.write(-vel); motorD.write(vel);
  Serial.printf("Movendo para esquerda vel = %d\n", vel);
}

void direita(int vel){
  vel = map(vel, _MIN_JOYSTICK_VALUE, _MAX_JOYSTICK_VALUE, 0, 180);
  motorE.write(vel); motorD.write(-vel);
  Serial.printf("Movendo para direita vel = %d\n", vel);
}

void esquerda_lenta(int vel){
  vel = map(vel, _MIN_JOYSTICK_VALUE, _MAX_JOYSTICK_VALUE, 0, 180);
  motorE.write(90); motorD.write(vel);
  Serial.printf("Movendo para esquerda vel = %d\n", vel);
}

void direita_lenta(int vel){
  vel = map(vel, _MIN_JOYSTICK_VALUE, _MAX_JOYSTICK_VALUE, 0, 180);
  motorE.write(vel); motorD.write(90);
  Serial.printf("Movendo para direita vel = %d\n", vel);
}

void para(){
  motorE.write(90); motorD.write(90);
  Serial.printf("Parado\n");
}
