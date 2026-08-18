#include "esp32-hal.h"
#include "Motor.h"

//! PINS
//MOTORES
#define IN1_MotorD 33
#define IN2_MotorD 32

#define IN1_MotorE 26
#define IN2_MotorE 25

//! OBJETOS
Motor motorD, motorE;

//! FUNÇÕES
void setupPinsMotor(){
    motorD.pinout(IN1_MotorD, IN2_MotorD);
    motorE.pinout(IN1_MotorE, IN2_MotorE);
}

void para(){
    motorD.write(0);
    delay(10);
    motorE.write(0);
    delay(10);
}

void frente(){
    motorD.write(1);
    delay(10);
    motorE.write(1);
    delay(10);
}

void esquerda(){
    motorD.write(1);
    delay(10);
    motorE.write(-1);
    delay(10);
}

void direita(){
    motorD.write(-1);
    delay(10);
    motorE.write(1);
    delay(10);
}

void esquerda_lenta(){
    motorD.write(1);
    delay(10);
    motorE.write(0);
    delay(10);
}

void direita_lenta(){
    motorD.write(0);
    delay(10);
    motorE.write(1);
    delay(10);
}

void costa(){
    motorD.write(-1);
    delay(10);
    motorE.write(-1);
    delay(10);
}

void frente_forte(){
    frente();
    delay(250);
    para();
}

void costa_forte(){
    costa();
    delay(250);
    para();
}
