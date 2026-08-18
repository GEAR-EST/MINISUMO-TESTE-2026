/**
 * @file strategy.h
 * @brief Estratégias de batalha não-bloqueantes com máquina de estados e millis().
 *
 * Cada ação de batalha (botões A/B/X/Y) tem duração configurável em config.h.
 * O analógico sempre tem prioridade: se o stick sair da deadzone durante uma ação,
 * ela é cancelada imediatamente (via parâmetro stick_active em update_strategy).
 *
 * Mapeamento dos botões:
 *   A → DASH_FWD — dash para frente em velocidade máxima
 *   B → EVADE    — recuo rápido + giro (evasiva)
 *   X → SPIN     — spin no lugar (giro contínuo)
 *   Y → DASH_BWD — dash para trás em velocidade máxima
 */

#pragma once

#include <Arduino.h>
#include "config.h"
#include "movement.h"

// ─── Tipo de estado ───

/**
 * @brief Estados possíveis da máquina de batalha.
 */
enum class BattleState {
    IDLE,      ///< Controle pelo analógico (estado padrão)
    DASH_FWD,  ///< Botão A — dash para frente em velocidade máxima
    EVADE,     ///< Botão B — recuo rápido + giro (evasiva)
    SPIN,      ///< Botão X — spin no lugar
    DASH_BWD   ///< Botão Y — dash para trás em velocidade máxima
};

// ─── Estado interno da máquina ───

static BattleState   _battle_state = BattleState::IDLE; ///< Estado atual
static unsigned long _action_start = 0;                  ///< Timestamp de início (ms)
static unsigned long _action_end   = 0;                  ///< Timestamp de fim (ms)

// ─── Declarações de funções públicas ───

/**
 * @brief Retorna o estado atual da máquina de batalha.
 */
BattleState get_battle_state();

/**
 * @brief Retorna true se uma ação de batalha está em execução.
 */
bool is_action_running();

/**
 * @brief Dispara uma ação de batalha e inicia seu timer.
 * @param action Estado a ser ativado (DASH_FWD, EVADE, SPIN ou DASH_BWD)
 */
void trigger_action(BattleState action);

/**
 * @brief Atualiza a máquina de estados de batalha.
 *
 * Deve ser chamado a cada iteração do ciclo de controle.
 * - Se stick_active for true: cancela a ação e devolve controle ao analógico
 * - Se o timer expirar: cancela a ação e para o robô
 * - Caso contrário: executa o comando de movimento da ação atual
 *
 * @param stick_active true se qualquer stick estiver fora da deadzone
 */
void update_strategy(bool stick_active);

// ─── Implementações ───

inline BattleState get_battle_state() {
    return _battle_state;
}

inline bool is_action_running() {
    return _battle_state != BattleState::IDLE;
}

inline void trigger_action(BattleState action) {
    _battle_state = action;
    _action_start = millis();

    // Define a duração com base no tipo de ação
    unsigned long duration = 0;
    switch (action) {
        case BattleState::DASH_FWD: duration = DASH_DURATION_MS;  break;
        case BattleState::DASH_BWD: duration = DASH_DURATION_MS;  break;
        case BattleState::EVADE:    duration = EVADE_DURATION_MS; break;
        case BattleState::SPIN:     duration = SPIN_DURATION_MS;  break;
        default:
            _battle_state = BattleState::IDLE; // Estado inválido → cancela
            return;
    }

    _action_end = _action_start + duration;
}

inline void update_strategy(bool stick_active) {
    // Nada a fazer no estado IDLE
    if (_battle_state == BattleState::IDLE) return;

    bool timer_expired = (millis() >= _action_end);

    // Cancela a ação se o analógico foi acionado ou se o tempo expirou
    if (stick_active || timer_expired) {
        _battle_state = BattleState::IDLE;
        stop(); // Parada ativa antes de devolver controle ao analógico
        return;
    }

    // Executa o comando de movimento da ação atual
    switch (_battle_state) {
        case BattleState::DASH_FWD:
            move(MAX_POWER, MAX_POWER);   // Ambos motores a frente, potência máxima
            break;
        case BattleState::DASH_BWD:
            move(-MAX_POWER, -MAX_POWER); // Ambos motores em ré, potência máxima
            break;
        case BattleState::SPIN:
            move(-MAX_POWER, MAX_POWER);  // Motores em sentidos opostos: spin no lugar
            break;
        case BattleState::EVADE:
            move(-MAX_POWER, 0);          // Ré + giro: recua girando para um lado
            break;
        default:
            break;
    }
}
