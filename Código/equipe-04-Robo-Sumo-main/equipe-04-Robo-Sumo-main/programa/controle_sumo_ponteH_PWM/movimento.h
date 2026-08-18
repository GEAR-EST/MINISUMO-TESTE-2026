#include "Motor.h"

//! PINS
//MOTORES
#define PWM_MotorD 27
#define IN1_MotorD 25
#define IN2_MotorD 26

#define IN1_MotorE 32
#define IN2_MotorE 33
#define PWM_MotorE 14

//! MISCELÂNEA
#define MOTOR_OFF 0
#define POT_MAX 255
#define POT_D 255
#define POT_E 120

#define DELAY_FORTE 100

//! OBJETOS
Motor motorD, motorE;

//! FUNÇÕES
void setupPinsMotor(){
    motorD.pinout(PWM_MotorD, IN1_MotorD, IN2_MotorD);
    motorE.pinout(PWM_MotorE, IN1_MotorE, IN2_MotorE);
}

void para(){
    motorD.write(MOTOR_OFF);
    motorE.write(MOTOR_OFF);
}

void frente(){
    motorD.write(POT_D);
    motorE.write(POT_E);
}

void esquerda(){
    motorD.write(-POT_MAX);
    motorE.write(POT_MAX);
}

void direita(){
    motorD.write(POT_MAX);
    motorE.write(-POT_MAX);
}

void esquerda_lenta(){
    motorD.write(MOTOR_OFF);
    motorE.write(POT_MAX);
}

void direita_lenta(){
    motorD.write(POT_MAX);
    motorE.write(MOTOR_OFF);
}

void costa(){
    motorD.write(-POT_D);
    motorE.write(-POT_E);
}

void frente_forte(){
    frente();
    delay(DELAY_FORTE);
    para();
}

void costa_forte(){
    costa();
    delay(DELAY_FORTE);
    para();
}
