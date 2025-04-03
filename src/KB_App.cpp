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
  registerCommand(COMMAND_KB_LAYOUT);
  registerCommand(COMMAND_KB_SLEEP);
}

void KB_App::setup() {
  PersistentMemory pmem(appname, true);
  String layout = pmem.readString("layout", "de_DE");
  pmem.commit();
  Keyboard.begin(layoutFromString(layout.c_str()));

  USB.begin();

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
    Keyboard.print(val);
    return {ACK, "", FLAG_KEEP_ALIVE};
  }

  if (COMMAND_KB_PRINTLN.check(message)) {
    String val = COMMAND_KB_PRINT.getParamVal(message, 0);
    Keyboard.println(val);
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

  if (COMMAND_KB_SLEEP.check(message)) {
    int duration = COMMAND_KB_SLEEP.getParamVal(message, 0).toInt();
    delay(duration);
    return {ACK, "", FLAG_KEEP_ALIVE};
  }

  return App::processMessage(message);
}

void KB_App::loop() {
}

String KB_App::getName() {
  return appname;
}

const uint8_t * KB_App::layoutFromString(const char * layout){
  KB_Layout kb_layouts[10] = {
    {"de_DE", KeyboardLayout_de_DE},
    {"en_US", KeyboardLayout_en_US},
    {"es_ES", KeyboardLayout_es_ES},
    {"fr_FR", KeyboardLayout_fr_FR},
    {"it_IT", KeyboardLayout_it_IT},
    {"pt_PT", KeyboardLayout_pt_PT},
    {"sv_SE", KeyboardLayout_sv_SE},
    {"da_DK", KeyboardLayout_da_DK},
    {"hu_HU", KeyboardLayout_hu_HU},
    {"pt_BR", KeyboardLayout_pt_BR}
  };

  for (KB_Layout kb_layout : kb_layouts){
    if(strcmp(kb_layout.name, layout) == 0){
      return kb_layout.layout;
    }
  }
  return KeyboardLayout_de_DE;
}
