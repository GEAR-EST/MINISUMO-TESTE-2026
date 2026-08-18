#include <Bluepad32.h>
#include "movimento.h"

#define LED_BUILTIN 2

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      digitalWrite(LED_BUILTIN, 1);

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

  else para();

//!============= RIGHT JOYSTCK ==============
  //== ESQUERDA ==//
  if (ctl->axisRX() <= -25) esquerda_lenta();

  //== DIREITA ==//
  else if (ctl->axisRX() >= 25) direita_lenta();

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

void setup() {
  setupPinsMotor();

  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());

  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", 
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  BP32.setup(&onConnectedController, &onDisconnectedController);

  BP32.forgetBluetoothKeys();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  bool dataUpdated = BP32.update();

  if (dataUpdated) processControllers();

  delay(150);
}
