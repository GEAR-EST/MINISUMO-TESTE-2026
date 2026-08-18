#include <Bluepad32.h>
#include "movimento.h"
#include "buzzer.h"

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      digitalWrite(LED_BUILTIN, 1);
      tocarEntrada();

      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);

      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);

      myControllers[i] = ctl;
      foundEmptySlot = true;

      break;
    }
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      digitalWrite(LED_BUILTIN, 0);
      tocarSaida();

      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);

      myControllers[i] = nullptr;
      foundController = true;

      break;
    }
  }
}

void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
    "axis L: %4d, %4d, axis R: %4d, %4d\n",
    ctl->axisX(), //direita: 508, esquerda: -512
    ctl->axisY(), //frente: -512, costa: 508
    ctl->axisRX(), //direita: 508, esquerda: -512
    ctl->axisRY()
    );
}

void processGamepad(ControllerPtr ctl) {
//!============= JOYSTCK ESQUERDO ==============
  //== CIMA ==//
  if (ctl->axisY() <= -25) frente();

  //== BAIXO ==//
  else if (ctl->axisY() >= 25) costa();

  //== ESQUERDA ==//
  else if (ctl->axisX() <= -25) esquerda();

  //== DIREITA ==//
  else if (ctl->axisX() >= 25) direita();

  else if (ctl->axisRY() >= 25) costa_forte();

  else if (ctl->axisRY() <= -25) frente_forte();

  else para();

//!============= RIGHT JOYSTCK ==============
  //== ESQUERDA ==//
  if (ctl->axisRX() <= -25) esquerda_lenta();

  //== DIREITA ==//
  else if (ctl->axisRX() >= 25) direita_lenta();

  if (ctl->buttons() == 0x0001) tocarStarWars();

  dumpGamepad(ctl);
}

void processControllers() {
  for (auto myController : myControllers)
    if (myController && 
        myController->isConnected() && 
        myController->hasData() && 
        myController->isGamepad()
        )
            processGamepad(myController);
}
