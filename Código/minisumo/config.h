/**
 * @file config.h
 * @brief Parâmetros configuráveis do minisumo — pinos, constantes e timings.
 *
 * Centraliza todos os valores ajustáveis do firmware em um único lugar,
 * facilitando a calibração do robô sem precisar alterar a lógica do código.
 */

#pragma once

// ─── Pinos do motor direito ───

#define RIGHT_MOTOR_PWM   32  ///< Pino PWM de velocidade do motor direito
#define RIGHT_MOTOR_IN1   33  ///< Pino IN1 de direção do motor direito
#define RIGHT_MOTOR_IN2   25  ///< Pino IN2 de direção do motor direito

// ─── Pinos do motor esquerdo ───

#define LEFT_MOTOR_PWM    18  ///< Pino PWM de velocidade do motor esquerdo
#define LEFT_MOTOR_IN1    19  ///< Pino IN1 de direção do motor esquerdo
#define LEFT_MOTOR_IN2    21  ///< Pino IN2 de direção do motor esquerdo

// ─── LED embutido ───

#define ONBOARD_LED       2   ///< GPIO do LED embutido do ESP32

// ─── Controle analógico ───

#define STICK_DEADZONE    25  ///< Zona morta dos sticks (valor absoluto; range do eixo: 0–512)
#define MAX_POWER         255 ///< Potência máxima aplicada aos motores (range: 0–255)

// ─── Durações das ações de batalha ───

#define DASH_DURATION_MS  300 ///< Duração do dash (frente ou trás) em milissegundos
#define EVADE_DURATION_MS 400 ///< Duração da evasiva (recuo + giro) em milissegundos
#define SPIN_DURATION_MS  400 ///< Duração do spin no lugar em milissegundos

// ─── Timing do loop principal ─────────────────────────────────────────────────

#define LOOP_INTERVAL_MS  10  ///< Intervalo do ciclo de controle em ms (~100 Hz)

// ─── Allowlist Bluetooth ──────────────────────────────────────────────────────
// Lista de controles permitidos (até 4 entradas).
// Formato: string "XX:XX:XX:XX:XX:XX" com o endereço MAC do controle.
// Para descobrir o MAC do seu controle, ligue-o com enableNewBluetoothConnections(true)
// e leia o endereço no monitor serial ("Ignoring device, not in allow-list: XX:XX:...").

#define ALLOWED_CONTROLLER_1  "14:CB:65:F9:B6:0A"  ///< Controle principal
#define ALLOWED_CONTROLLER_2  "20:19:12:09:2E:7C"  ///< Controle principal
