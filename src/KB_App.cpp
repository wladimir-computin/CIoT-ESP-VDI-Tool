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
  registerCommand(COMMAND_KB_LEDS);
  registerCommand(COMMAND_KB_LAYOUT);
  registerCommand(COMMAND_KB_SLEEP);
}

void KB_App::setup() {
  PersistentMemory pmem(appname, true);
  String layout = pmem.readString("layout", current_kb_layout->name);
  shift_key_reports =  pmem.readBool("shift_key_reports", shift_key_reports);
  pmem.commit();
  current_kb_layout = layoutFromString(layout.c_str());
  Keyboard.begin(current_kb_layout->layout);
  Keyboard.setShiftKeyReports(shift_key_reports);
  Keyboard.onEvent(ARDUINO_USB_HID_KEYBOARD_LED_EVENT, keyboardLedCallback);
  USB.begin();
}

String KB_App::getStatus() {
  char formats[] =
  "Current KB Layout: %s\n"
  "Shift Key Reports: %d\n"
  "\n"
  "Num Lock: %d\n"
  "Caps Lock: %d\n"
  "Scroll Lock: %d\n"
  "Compose: %d\n"
  "Kana: %d\n"
  ;

  char buf[512];
  snprintf(buf, sizeof(buf) / sizeof(buf[0]), formats,
           current_kb_layout->name,
           shift_key_reports,
           current_leds.numlock,
           current_leds.capslock,
           current_leds.scrolllock,
           current_leds.compose,
           current_leds.kana
  );
  return buf;
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

    if(duration == 0){
      duration = 100;
    }

    // Split the input by '+' to handle key combinations
    int keyCount = 0;
    uint8_t keys[10]; // Maximum 10 keys in combination
    bool validKeys = true;

    // Count number of '+' to determine key count
    int plusCount = 0;
    for(int i = 0; i < val.length(); i++) {
      if(val.charAt(i) == '+') plusCount++;
    }
    keyCount = plusCount + 1;

    // Parse each key
    int startPos = 0;
    for(int i = 0; i < keyCount; i++) {
      int endPos = val.indexOf('+', startPos);
      if(endPos == -1) endPos = val.length(); // Last key

      String keyStr = val.substring(startPos, endPos);
      keyStr.trim(); // Remove any whitespace

      uint8_t key = 0;
      if(keyStr.length() == 1) {
        // Single character key
        key = keyStr[0];
      } else {
        // Look up in kb_keys map
        key = kb_keys[keyStr];
      }

      if(key == 0) {
        validKeys = false;
        break;
      }

      keys[i] = key;
      startPos = endPos + 1;
    }

    if(validKeys) {
      // Press all keys
      for(int i = 0; i < keyCount; i++) {
        Keyboard.press(keys[i]);
        delay(duration);
      }

      delay(duration);

      // Release all keys
      Keyboard.releaseAll();

      return {ACK, "", FLAG_KEEP_ALIVE};
    } else {
      return {ERR, "INVALID_KEYCODE"};
    }
  }

  if (COMMAND_KB_PRESS.check(message)) {
    String val = COMMAND_KB_PRESS.getParamVal(message, 0);
    uint8_t key = 0;
    if(val.length() == 1){
      key = val[0];
    } else {
      key = kb_keys[val];
    }
    if(key != 0){
      Keyboard.press(key);
      return {ACK, "", FLAG_KEEP_ALIVE};
    } else {
      return {ERR, "INVALID_KEYCODE"};
    }
  }

  if (COMMAND_KB_RELEASE.check(message)) {
    String val = COMMAND_KB_RELEASE.getParamVal(message, 0);
    if(val != ""){
      uint8_t key = 0;
      if(val.length() == 1){
        key = val[0];
      } else {
        key = kb_keys[val];
      }
      if(key != 0){
        Keyboard.press(key);
        return {ACK, "", FLAG_KEEP_ALIVE};
      } else {
        return {ERR, "INVALID_KEYCODE"};
      }
    } else {
      Keyboard.releaseAll();
    }
    return {ACK, "", FLAG_KEEP_ALIVE};
  }

  if (COMMAND_KB_LEDS.check(message)) {
    String led = COMMAND_KB_LEDS.getParamVal(message, 0);
    String val = COMMAND_KB_LEDS.getParamVal(message, 1);
    if(led != "" && val != ""){
      return {ACK, "", FLAG_KEEP_ALIVE};
    } else if (led != "") {
      return {ACK, "", FLAG_KEEP_ALIVE};
    } else {
      JsonDocument json;
      JsonObject jsonObj = json.to<JsonObject>();
      jsonObj["NUM_LOCK"] = current_leds.numlock;
      jsonObj["CAPS_LOCK"] = current_leds.capslock;
      jsonObj["SCROLL_LOCK"] = current_leds.scrolllock;
      jsonObj["COMPOSE"] = current_leds.compose;
      jsonObj["KANA"] = current_leds.kana;

      String out;
      out.reserve(256);
      serializeJsonPretty(json, out);
      return {DATA, out, FLAG_KEEP_ALIVE};
    }
  }

  if (COMMAND_KB_LAYOUT.check(message)) {
    String val = COMMAND_KB_LAYOUT.getParamVal(message, 0);
    if(val != ""){
      PersistentMemory pmem(appname, true);
      pmem.writeString("layout", layoutFromString(val.c_str())->name);
      pmem.commit();
      return {ACK, "", FLAG_KEEP_ALIVE};
    } else {
      JsonDocument json;
      JsonArray jsonObj = json.to<JsonArray>();
      for (KB_Layout &kb_layout : kb_layouts){
        if(kb_layout.layout == current_kb_layout->layout){
          jsonObj.add("[" + String(kb_layout.name) + "]");
        } else {
          jsonObj.add(kb_layout.name);
        }
      }

      String out;
      out.reserve(256);
      serializeJsonPretty(json, out);
      return {DATA, out, FLAG_KEEP_ALIVE};
    }
  }

  if (COMMAND_KB_SLEEP.check(message)) {
    int duration = COMMAND_KB_SLEEP.getParamVal(message, 0).toInt();
    delay(duration);
    return {ACK, "", FLAG_KEEP_ALIVE};
  }

  return App::processMessage(message);
}


// Instance method to handle the LED event
void KB_App::keyboardLedCallback(void* handler_args, esp_event_base_t base, int32_t id, void* event_data) {
  arduino_usb_hid_keyboard_event_data_t* led_data = static_cast<arduino_usb_hid_keyboard_event_data_t*>(event_data);
  current_leds = *led_data;
}

void KB_App::loop() {
}

String KB_App::getName() {
  return appname;
}

KB_Layout * KB_App::layoutFromString(const char * name){
  for (KB_Layout &kb_layout : kb_layouts){
    if(strcmp(kb_layout.name, name) == 0){
      return &kb_layout;
    }
  }
  return &kb_layouts[0];
}
