/*
 *  KB_App
 *
 *  Class for emulating a virtual USB HID Keyboard
 */

#pragma once

#include <Arduino.h>
#include <Ticker.h>
#include <ArduinoJson.h>

#include "AllConfig.h"
#include <App.h>
#include <PersistentMemory.h>

#include <USB.h>
#include <USBHIDKeyboard.h>


static Param COMMAND_KB_PRINT_PARAMS[] = {{"string", DATATYPE_STRING, false, 1, 1024}};
static Command COMMAND_KB_PRINT("print", ARRAY_LEN(COMMAND_KB_PRINT_PARAMS), COMMAND_KB_PRINT_PARAMS);
static Command COMMAND_KB_PRINTLN("println", ARRAY_LEN(COMMAND_KB_PRINT_PARAMS), COMMAND_KB_PRINT_PARAMS);

static Param COMMAND_KB_HIT_PARAMS[] = {{"keys", DATATYPE_STRING, false, 1, 64}, {"duration", DATATYPE_INT, true, 1, 1000}};
static Command COMMAND_KB_HIT("hit", ARRAY_LEN(COMMAND_KB_HIT_PARAMS), COMMAND_KB_HIT_PARAMS);

static Param COMMAND_KB_PRESS_PARAMS[] = {{"keys", DATATYPE_STRING, false, 1, 64}};
static Command COMMAND_KB_PRESS("press", ARRAY_LEN(COMMAND_KB_PRESS_PARAMS), COMMAND_KB_PRESS_PARAMS);

static Param COMMAND_KB_RELEASE_PARAMS[] = {{"keys", DATATYPE_STRING, true, 1, 64}};
static Command COMMAND_KB_RELEASE("release", ARRAY_LEN(COMMAND_KB_RELEASE_PARAMS), COMMAND_KB_RELEASE_PARAMS);

static Param COMMAND_KB_LAYOUT_PARAMS[] = {{"layout", DATATYPE_STRING, true, 1, 10}};
static Command COMMAND_KB_LAYOUT("layout", ARRAY_LEN(COMMAND_KB_LAYOUT_PARAMS), COMMAND_KB_LAYOUT_PARAMS);

static Param COMMAND_KB_SLEEP_PARAMS[] = {{"millies", DATATYPE_INT, false, 0, 1000*60*10}}; //10 minutes
static Command COMMAND_KB_SLEEP("sleep", ARRAY_LEN(COMMAND_KB_SLEEP_PARAMS), COMMAND_KB_SLEEP_PARAMS);

struct KB_Layout {
  const char name[10];
  const uint8_t * layout;
};

// Supported keyboard layouts
extern const uint8_t KeyboardLayout_de_DE[];
extern const uint8_t KeyboardLayout_en_US[];
extern const uint8_t KeyboardLayout_es_ES[];
extern const uint8_t KeyboardLayout_fr_FR[];
extern const uint8_t KeyboardLayout_it_IT[];
extern const uint8_t KeyboardLayout_pt_PT[];
extern const uint8_t KeyboardLayout_sv_SE[];
extern const uint8_t KeyboardLayout_da_DK[];
extern const uint8_t KeyboardLayout_hu_HU[];
extern const uint8_t KeyboardLayout_pt_BR[];


class KB_App : public App{

public:

  KB_App(const char * name);
  ProcessMessageStruct processMessage(String &message);
  void setup();
  void loop();
  String getName();
  String getStatus();
  String getType();

private:
  String appname;
  USBHIDKeyboard Keyboard;


  const uint8_t * layoutFromString(const char * layout);



  //static void geigerClick();
  //Ticker geigerTicker;
  //static void geigerTick(void * context);
};
