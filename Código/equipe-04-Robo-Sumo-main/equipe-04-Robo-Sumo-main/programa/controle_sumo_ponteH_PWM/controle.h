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

      myControllers[i] = nullptr;
      foundController = true;

      break;
    }
  }
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
  if (ctl->buttons() == 0x0002) tocarFurElise();
  if (ctl->buttons() == 0x0004) tocarSuperMario();
  if (ctl->buttons() == 0x0008) tocarWeWishYou();
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
