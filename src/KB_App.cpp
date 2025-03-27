/*
  KB_App

  (implementation)
*/

#include "KB_App.h"

KB_App::KB_App(const char * name) {
  appname = name;
  registerCommand(COMMAND_KB_PRINT);
  registerCommand(COMMAND_KB_PRINTLN);
  registerCommand(COMMAND_KB_HIT);
  registerCommand(COMMAND_KB_PRESS);
  registerCommand(COMMAND_KB_RELEASE);
}

void KB_App::setup() {
  PersistentMemory pmem(appname, true);
  pmem.commit();
  Keyboard.begin();

}

String KB_App::getStatus() {
  return "";
}

String KB_App::getType() {
  return "VIRTUAL_KEYBOARD";
}


ProcessMessageStruct KB_App::processMessage(String &message) {
  if (COMMAND_KB_PRINT.check(message)) {
    String val = COMMAND_KB_PRINT.getParamVal(message, 0);
    return {ACK, "", FLAG_KEEP_ALIVE};
  }

  if (COMMAND_KB_PRINTLN.check(message)) {
    String val = COMMAND_KB_PRINT.getParamVal(message, 0);
    return {ACK, "", FLAG_KEEP_ALIVE};
  }

  if (COMMAND_KB_HIT.check(message)) {
    String val = COMMAND_KB_HIT.getParamVal(message, 0);
    int duration = COMMAND_KB_HIT.getParamVal(message, 1).toInt();
    return {ACK, "", FLAG_KEEP_ALIVE};
  }

  if (COMMAND_KB_PRESS.check(message)) {
    String val = COMMAND_KB_PRESS.getParamVal(message, 0);
    return {ACK, "", FLAG_KEEP_ALIVE};
  }

  if (COMMAND_KB_RELEASE.check(message)) {
    String val = COMMAND_KB_RELEASE.getParamVal(message, 0);
    return {ACK, "", FLAG_KEEP_ALIVE};
  }

  if (COMMAND_KB_LAYOUT.check(message)) {
    String val = COMMAND_KB_LAYOUT.getParamVal(message, 0);
    return {ACK, "", FLAG_KEEP_ALIVE};
  }

  return App::processMessage(message);
}

void KB_App::loop() {
}

String KB_App::getName() {
  return appname;
}
