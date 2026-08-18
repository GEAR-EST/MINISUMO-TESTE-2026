#include <Bluepad32.h>
#include "controle.h"

#define LED_BUILTIN 2

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  setupPinsMotor();
  tocarEntrada();

  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());

  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", 
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  BP32.setup(&onConnectedController, &onDisconnectedController);

  BP32.forgetBluetoothKeys();
}

void loop() {
  bool dataUpdated = BP32.update();

  if (dataUpdated) processControllers();

  delay(150);
}
