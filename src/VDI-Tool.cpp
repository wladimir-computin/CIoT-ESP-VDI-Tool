/*
  VDI-Tool - Main

  Contains the main functionality.
  Entrypoint is setup()
*/

#include "AllConfig.h"
#include <CryptoIoT.h>
#include "KB_App.h"

//Apps
KB_App kb_app("KB");
App * apps[] = {&kb_app};

CryptoIoT cryptoIoT(apps, sizeof(apps) / sizeof(apps[0]));

void loop() {
  cryptoIoT.loop();
}

//Here's the entrypoint.
void setup() {
  cryptoIoT.setup();
  cryptoIoT.setDeviceType(DEVICETYPE);
}
