/**
 * @file minisumo.ino
 * @brief Firmware do minisumo ESP32 com Bluepad32.
 *
 * Loop principal não-bloqueante: leitura do controle Bluetooth e
 * máquina de estados de batalha executadas sem delay() bloqueante.
 *
 * Hierarquia de inclusão:
 *   minisumo.ino
 *     └── controller.h
 *           ├── movement.h
 *           │     ├── motor.h
 *           │     └── config.h
 *           └── strategy.h
 *                 ├── movement.h  (via #pragma once)
 *                 └── config.h    (via #pragma once)
 */

#include "controller.h"

// Timestamp da última execução do ciclo de controle
static unsigned long _last_update_ms = 0;

// ─────────────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    pinMode(ONBOARD_LED, OUTPUT);

    setup_motors();     // Inicializa pinos e modo dos motores
    setup_controller(); // Inicializa Bluepad32 e registra callbacks de conexão
}

void loop() {
    // BP32.update() mantém a pilha Bluetooth ativa e deve rodar o mais frequente possível
    BP32.update();

    // Ciclo de controle com intervalo fixo definido por LOOP_INTERVAL_MS
    unsigned long now = millis();
    if (now - _last_update_ms >= LOOP_INTERVAL_MS) {
        _last_update_ms = now;

        // Lê input do controle, atualiza máquina de estados e comanda os motores
        process_input();
    }
}
