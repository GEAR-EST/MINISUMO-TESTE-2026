/**
 * @file controller.h
 * @brief Interface com o controle Bluetooth via Bluepad32.
 *
 * Gerencia a conexão com um único controle (gamepad), lê os inputs analógicos
 * e digitais, e despacha comandos de movimento ou ações de batalha.
 *
 * Regra de prioridade (do maior para o menor):
 *   1. Stick analógico ativo  → drive_from_sticks() (cancela qualquer ação)
 *   2. Botão de batalha       → trigger_action()    (só dispara se stick inativo)
 *   3. Ação em andamento      → update_strategy()   executa o movimento
 *   4. Tudo inativo           → stop()              freio ativo
 */

#pragma once

#include <Bluepad32.h>
#include <uni.h>          // API interna do Bluepad32: allowlist, sscanf_bd_addr, etc.
#include "config.h"
#include "movement.h"
#include "strategy.h"

// ─── Controlador global ───

static ControllerPtr _controller = nullptr; ///< Referência ao único controle ativo

// ─── Funções auxiliares (privadas) ───

/**
 * @brief Verifica se um eixo analógico está fora da deadzone.
 * @param axis Valor do eixo (-512 a +512)
 * @return true se |axis| > STICK_DEADZONE
 */
static inline bool _outside_deadzone(int axis) {
    return abs(axis) > STICK_DEADZONE;
}

/**
 * @brief Verifica se qualquer eixo relevante está fora da deadzone.
 * @param ctl Ponteiro para o controlador ativo
 * @return true se qualquer stick estiver ativo
 */
static inline bool _stick_is_active(ControllerPtr ctl) {
    return _outside_deadzone(ctl->axisY())  ||
           _outside_deadzone(ctl->axisX())  ||
           _outside_deadzone(ctl->axisRX());
}

// ─── Callbacks Bluepad32 ───

/**
 * @brief Chamado automaticamente pelo Bluepad32 quando um controle se conecta.
 * Aceita apenas o primeiro controle (slot único).
 */
void on_controller_connected(ControllerPtr ctl) {
    if (_controller == nullptr) {
        _controller = ctl;
        digitalWrite(ONBOARD_LED, HIGH); // LED acende para indicar conexão
    }
}

/**
 * @brief Chamado automaticamente pelo Bluepad32 quando um controle se desconecta.
 * Para o robô imediatamente por segurança.
 */
void on_controller_disconnected(ControllerPtr ctl) {
    if (_controller == ctl) {
        stop();                          // Para o robô imediatamente por segurança
        _controller = nullptr;
        digitalWrite(ONBOARD_LED, LOW);  // LED apaga para indicar desconexão
    }
}

// ─── Declarações de funções públicas ───

/**
 * @brief Inicializa o Bluepad32, imprime diagnóstico serial e registra os callbacks.
 * Deve ser chamado no setup().
 */
void setup_controller();

/**
 * @brief Lê os botões de batalha (A/B/X/Y) e dispara a ação correspondente.
 * Não dispara nova ação se uma já estiver em andamento.
 * @param ctl Ponteiro para o controlador ativo
 */
void read_battle_buttons(ControllerPtr ctl);

/**
 * @brief Processa todo o input do controle e atualiza o estado do robô.
 *
 * Ordem de execução:
 *   1. Lê botões de batalha (se stick inativo e sem ação em andamento)
 *   2. Atualiza a máquina de estados (aplica movimento ou cancela se stick ativo)
 *   3. Se nenhuma ação em andamento: controle pelo analógico ou parada ativa
 *
 * Deve ser chamado a cada ciclo de controle (LOOP_INTERVAL_MS).
 */
void process_input();

// ─── Implementações ───

inline void setup_controller() {
    // Diagnóstico serial — útil para verificar a versão do firmware Bluepad32
    Serial.printf("Bluepad32 Firmware: %s\n", BP32.firmwareVersion());

    const uint8_t* local_addr = BP32.localBdAddress();
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n",
                  local_addr[0], local_addr[1], local_addr[2],
                  local_addr[3], local_addr[4], local_addr[5]);

    BP32.setup(&on_controller_connected, &on_controller_disconnected);
    BP32.forgetBluetoothKeys(); // Esquece chaves anteriores (garante pareamento limpo)

    // Configura o allowlist: apenas os MACs definidos em config.h podem se conectar
    bd_addr_t allowed_addr;
    sscanf_bd_addr(ALLOWED_CONTROLLER_1, allowed_addr); // Converte string "XX:XX:..." para bytes
    uni_bt_allowlist_add_addr(allowed_addr);             // Adiciona ao allowlist (suporta até 4 entradas)
    uni_bt_allowlist_set_enabled(true);                  // Ativa a filtragem — rejeita MACs desconhecidos

    Serial.printf("Allowlist ativo: apenas %s pode conectar\n", ALLOWED_CONTROLLER_1);
}

inline void read_battle_buttons(ControllerPtr ctl) {
    // Não dispara nova ação se uma já está em andamento (evita sobreposição)
    if (is_action_running()) return;

    if      (ctl->a()) trigger_action(BattleState::DASH_FWD);
    else if (ctl->b()) trigger_action(BattleState::EVADE);
    else if (ctl->x()) trigger_action(BattleState::SPIN);
    else if (ctl->y()) trigger_action(BattleState::DASH_BWD);
}

inline void process_input() {
    // Sem controle válido conectado: nada a processar
    if (_controller == nullptr      ||
        !_controller->isConnected() ||
        !_controller->isGamepad()) return;

    bool stick_active = _stick_is_active(_controller);

    // Botões de batalha só são lidos quando o analógico está em repouso
    // (evita disparar uma ação inadvertidamente ao mover o stick)
    if (!stick_active) {
        read_battle_buttons(_controller);
    }

    // Atualiza a máquina de estados:
    //   - aplica o movimento da ação atual, OU
    //   - cancela e para se o stick ficou ativo ou o timer expirou
    update_strategy(stick_active);

    // Se nenhuma ação de batalha está rodando, o analógico assume o controle
    if (!is_action_running()) {
        if (stick_active) {
            drive_from_sticks(
                _controller->axisY(),   // ly: velocidade frente/trás
                _controller->axisX(),   // lx: giro no lugar
                _controller->axisRX(),  // rx: diferencial de direção
                _controller->axisRY()   // ry: reservado para uso futuro
            );
        } else {
            // Analógico na deadzone e sem ação ativa: freio ativo
            stop();
        }
    }
}
