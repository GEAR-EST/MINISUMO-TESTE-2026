/**
 * @file movement.h
 * @brief Funções de movimento do minisumo com controle proporcional de velocidade.
 *
 * Implementa:
 * - Controle direto de potência dos dois motores com clamping automático
 * - Mistura arcade (arcade mix) para controle com o stick esquerdo
 * - Diferencial de direção via stick direito
 * - Parada ativa com freio elétrico
 */

#pragma once

#include "config.h"
#include "motor.h"

// ─── Instâncias dos motores ───

Motor right_motor; ///< Motor do lado direito do robô
Motor left_motor;  ///< Motor do lado esquerdo do robô

// ─── Declarações de funções públicas ───

/**
 * @brief Inicializa os pinos GPIO dos dois motores.
 * Deve ser chamado no setup() antes de qualquer comando de movimento.
 */
void setup_motors();

/**
 * @brief Aplica potências individuais aos dois motores com clamping automático.
 * @param left_power  Potência do motor esquerdo (-MAX_POWER a MAX_POWER)
 * @param right_power Potência do motor direito  (-MAX_POWER a MAX_POWER)
 */
void move(int left_power, int right_power);

/**
 * @brief Parada ativa com freio elétrico nos dois motores.
 * Ambos os motores recebem IN1 = IN2 = HIGH (freio regenerativo).
 */
void stop();

/**
 * @brief Calcula e aplica o movimento a partir dos quatro eixos analógicos.
 *
 * Mistura arcade com diferencial de direção:
 * - Stick esquerdo Y: velocidade base frente/trás (proporcional, ly– = frente)
 * - Stick esquerdo X: giro no lugar (proporcional, lx+ = direita)
 * - Stick direito  X: diferencial de direção — curva suave enquanto anda
 * - Stick direito  Y: reservado para uso futuro (ignorado)
 *
 * A deadzone é aplicada individualmente por eixo via STICK_DEADZONE.
 *
 * @param ly Eixo Y do stick esquerdo  (range Bluepad32: -512 a +512)
 * @param lx Eixo X do stick esquerdo  (range Bluepad32: -512 a +512)
 * @param rx Eixo X do stick direito   (range Bluepad32: -512 a +512)
 * @param ry Eixo Y do stick direito   (range Bluepad32: -512 a +512, não utilizado)
 */
void drive_from_sticks(int ly, int lx, int rx, int ry);

// ─── Implementações ───

inline void setup_motors() {
    right_motor.setup(RIGHT_MOTOR_PWM, RIGHT_MOTOR_IN1, RIGHT_MOTOR_IN2);
    left_motor.setup(LEFT_MOTOR_PWM,   LEFT_MOTOR_IN1,  LEFT_MOTOR_IN2);
}

inline void move(int left_power, int right_power) {
    // Clamp: garante que os valores não ultrapassem os limites do motor
    left_power  = constrain(left_power,  -MAX_POWER, MAX_POWER);
    right_power = constrain(right_power, -MAX_POWER, MAX_POWER);

    left_motor.set_power(left_power);
    right_motor.set_power(right_power);
}

inline void stop() {
    left_motor.stop();
    right_motor.stop();
}

inline void drive_from_sticks(int ly, int lx, int rx, int ry) {
    // Aplica deadzone individualmente por eixo — zera ruído próximo ao centro
    if (abs(ly) <= STICK_DEADZONE) ly = 0;
    if (abs(lx) <= STICK_DEADZONE) lx = 0;
    if (abs(rx) <= STICK_DEADZONE) rx = 0;

    // ry reservado para uso futuro (ex: turbo, modo de tração) — ignorado por enquanto
    (void)ry;

    // Mapeia os eixos do range do Bluepad32 (-512 a +512) para potência (-MAX_POWER a MAX_POWER)
    // Nota: ly negativo = stick para cima = frente (convenção padrão de joystick)
    int base_speed = map(-ly, -512, 512, -MAX_POWER, MAX_POWER); // Velocidade base frente/trás
    int turn_rate  = map( lx, -512, 512, -MAX_POWER, MAX_POWER); // Taxa de giro (lx+ = direita)
    int steer_bias = map( rx, -512, 512, -MAX_POWER, MAX_POWER); // Diferencial de direção (rx+ = curva direita)

    // Mistura arcade + diferencial de direção:
    //   left  = base + turn + steer → motor esquerdo tem mais potência ao virar direita
    //   right = base - turn - steer → motor direito  tem menos potência ao virar direita
    int left_power  = base_speed + turn_rate + steer_bias;
    int right_power = base_speed - turn_rate - steer_bias;

    move(left_power, right_power);
}
