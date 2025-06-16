/*
 *  KB_App
 *
 *  Class for emulating a virtual USB HID Keyboard
 */

#pragma once

#include <Arduino.h>
#include <Ticker.h>
#include <ArduinoJson.h>
#include <map>

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

static Param COMMAND_KB_LEDS_PARAMS[] = {{"led", DATATYPE_STRING, true, 1, 32}, {"value", DATATYPE_BOOL, true}};
static Command COMMAND_KB_LEDS("leds", ARRAY_LEN(COMMAND_KB_LEDS_PARAMS), COMMAND_KB_LEDS_PARAMS);

static Param COMMAND_KB_LAYOUT_PARAMS[] = {{"layout", DATATYPE_STRING, true, 1, 10}};
static Command COMMAND_KB_LAYOUT("layout", ARRAY_LEN(COMMAND_KB_LAYOUT_PARAMS), COMMAND_KB_LAYOUT_PARAMS);

static Param COMMAND_KB_SLEEP_PARAMS[] = {{"millies", DATATYPE_INT, false, 0, 1000*60*10}}; //10 minutes
static Command COMMAND_KB_SLEEP("sleep", ARRAY_LEN(COMMAND_KB_SLEEP_PARAMS), COMMAND_KB_SLEEP_PARAMS);

struct KB_Layout {
  const char name[10];
  const uint8_t * layout;
};

struct KB_Keys {
  const char name[10];
  const uint8_t keycode;
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

static KB_Layout kb_layouts[] = {
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

static std::map<String, uint8_t> kb_keys = {
  {"L_CTRL"      , KEY_LEFT_CTRL    },
  {"L_SHIFT"     , KEY_LEFT_SHIFT   },
  {"L_ALT"       , KEY_LEFT_ALT     },
  {"L_GUI"       , KEY_LEFT_GUI     },
  {"R_CTRL"      , KEY_RIGHT_CTRL   },
  {"R_SHIFT"     , KEY_RIGHT_SHIFT  },
  {"R_ALT"       , KEY_RIGHT_ALT    },
  {"R_GUI"       , KEY_RIGHT_GUI    },

  {"UP"          , KEY_UP_ARROW     },
  {"DOWN"        , KEY_DOWN_ARROW   },
  {"LEFT"        , KEY_LEFT_ARROW   },
  {"RIGHT"       , KEY_RIGHT_ARROW  },
  {"MENU"        , KEY_MENU         },
  {"SPACE"       , KEY_SPACE        },
  {"BACKSPACE"   , KEY_BACKSPACE    },
  {"TAB"         , KEY_TAB          },
  {"RETURN"      , KEY_RETURN       },
  {"ESC"         , KEY_ESC          },
  {"INSERT"      , KEY_INSERT       },
  {"DELETE"      , KEY_DELETE       },
  {"PAGE_UP"     , KEY_PAGE_UP      },
  {"PAGE_DOWN"   , KEY_PAGE_DOWN    },
  {"HOME"        , KEY_HOME         },
  {"END"         , KEY_END          },
  {"NUM_LOCK"    , KEY_NUM_LOCK     },
  {"CAPS_LOCK"   , KEY_CAPS_LOCK    },
  {"F1"          , KEY_F1           },
  {"F2"          , KEY_F2           },
  {"F3"          , KEY_F3           },
  {"F4"          , KEY_F4           },
  {"F5"          , KEY_F5           },
  {"F6"          , KEY_F6           },
  {"F7"          , KEY_F7           },
  {"F8"          , KEY_F8           },
  {"F9"          , KEY_F9           },
  {"F10"         , KEY_F10          },
  {"F11"         , KEY_F11          },
  {"F12"         , KEY_F12          },
  {"F13"         , KEY_F13          },
  {"F14"         , KEY_F14          },
  {"F15"         , KEY_F15          },
  {"F16"         , KEY_F16          },
  {"F17"         , KEY_F17          },
  {"F18"         , KEY_F18          },
  {"F19"         , KEY_F19          },
  {"F20"         , KEY_F20          },
  {"F21"         , KEY_F21          },
  {"F22"         , KEY_F22          },
  {"F23"         , KEY_F23          },
  {"F24"         , KEY_F24          },
  {"PRINT_SCREEN", KEY_PRINT_SCREEN },
  {"SCROLL_LOCK" , KEY_SCROLL_LOCK  },
  {"PAUSE"       , KEY_PAUSE        },

  {"CTRL"        , KEY_LEFT_CTRL    },
  {"SHIFT"       , KEY_LEFT_SHIFT   },
  {"ALT"         , KEY_LEFT_ALT     },
  {"GUI"         , KEY_LEFT_GUI     },
  {"ENTER"       , KEY_RETURN       }
};

static arduino_usb_hid_keyboard_event_data_t current_leds;

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
  KB_Layout * current_kb_layout = &kb_layouts[0];
  bool shift_key_reports = true;
  static void keyboardLedCallback(void* handler_args, esp_event_base_t base, int32_t id, void* event_data);

  KB_Layout * layoutFromString(const char * name);



  //static void geigerClick();
  //Ticker geigerTicker;
  //static void geigerTick(void * context);
};
