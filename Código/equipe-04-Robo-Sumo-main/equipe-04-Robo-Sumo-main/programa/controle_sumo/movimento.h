#include <ESP32Servo.h>

#define motorD_pin 32
#define motorE_pin 33

Servo motorD, motorE;

//! FUNÇÕES DE MOVIMENTAÇÃO
void setupPinsMotor(){
	motorD.attach(motorD_pin);
	motorE.attach(motorE_pin);
}

void frente(){
  motorD.write(180);
  motorE.write(180);
  Serial.print("Indo para frente");
}

void para(){
  motorD.write(90);
  motorE.write(90);
  Serial.print("Parado\n");
}

void costa(){
  motorD.write(0);
  motorE.write(0);
  Serial.print("Dando ré\n");
}

void esquerda(){
  motorD.write(180);
  motorE.write(0);
  Serial.print("Indo para esquerda\n");
}

void direita(){
  motorD.write(0);
  motorE.write(180);
  Serial.print("Indo para direita\n");
}

void esquerda_lenta(){
  motorD.write(180);
  motorE.write(90);
  Serial.print("Indo para esquerda lentamente\n");
}

void direita_lenta(){
  motorD.write(90);
  motorE.write(180);
  Serial.print("Indo para direita lentamente\n");
}
