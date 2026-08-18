/**
 * @file motor.h
 * @brief Abstração de motor DC com controle de sentido e potência via ponte H.
 *
 * Encapsula a comunicação com uma ponte H (ex: L298N, TB6612).
 * O sentido de rotação é controlado pelos pinos IN1/IN2 e a velocidade por PWM.
 */

#pragma once

#include <Arduino.h>

// ─── Declaração da classe ───

class Motor {
public:
    /**
     * @brief Configura os pinos GPIO do motor.
     * @param pin_pwm  Pino de controle de velocidade (PWM)
     * @param in1      Pino de direção IN1 da ponte H
     * @param in2      Pino de direção IN2 da ponte H
     */
    void setup(int pin_pwm, int in1, int in2);

    /**
     * @brief Define a potência e a direção de rotação do motor.
     * @param power Potência desejada: positivo = frente, negativo = ré.
     *              O valor absoluto deve estar entre 0 e MAX_POWER (255).
     */
    void set_power(int power);

    /**
     * @brief Parada ativa com freio elétrico (IN1 = IN2 = HIGH).
     *
     * Colocar ambos os pinos de direção em HIGH cria um curto-circuito
     * controlado na ponte H, gerando freio regenerativo imediato.
     * Mais eficaz do que apenas zerar o PWM (parada passiva).
     */
    void stop();

private:
    int _pwm; ///< Pino PWM de velocidade
    int _in1; ///< Pino IN1 de direção
    int _in2; ///< Pino IN2 de direção
};

// ─── Implementação da classe ───

inline void Motor::setup(int pin_pwm, int in1, int in2) {
    _pwm = pin_pwm;
    _in1 = in1;
    _in2 = in2;

    pinMode(_pwm, OUTPUT);
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
}

inline void Motor::set_power(int power) {
    if (power > 0) {
        // Sentido horário (frente)
        analogWrite(_pwm, power);
        digitalWrite(_in1, HIGH);
        digitalWrite(_in2, LOW);
    } else if (power < 0) {
        // Sentido anti-horário (ré)
        analogWrite(_pwm, -power);
        digitalWrite(_in1, LOW);
        digitalWrite(_in2, HIGH);
    } else {
        // Parada passiva: sem corrente nos enrolamentos
        analogWrite(_pwm, 0);
        digitalWrite(_in1, LOW);
        digitalWrite(_in2, LOW);
    }
}

inline void Motor::stop() {
    // Parada ativa: ambos pinos HIGH criam freio regenerativo na ponte H
    analogWrite(_pwm, 0);
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, HIGH);
}
