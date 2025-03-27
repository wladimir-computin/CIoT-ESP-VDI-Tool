# CIoT-ESP-VDI-Tool
CryptoIoT / VDI Copy&Paste Tool

## Build

```bash
git clone https://github.com/wladimir-computin/CIoT-ESP-VDI-Tool.git
git clone https://github.com/wladimir-computin/CryptoIoT-Core.git
cd CIoT-ESP-VDI-Tool
make
# for ESP32
make upload build=waveshare_s3_zero
```

## Related Projects

* [CryptoIoT-Core](https://github.com/wladimir-computin/CryptoIoT-Core.git)
	- The main core of the CryptoIoT smarthome system for ESP8266/ESP32
	- Handles WiFi, communication, encryption, persisten data storage etc.
* [CryptoIoT-Python](https://github.com/wladimir-computin/CryptoIoT-Python.git)
	- CLI client for interfacing with CryptoIoT devices.
* [Android-CryptoHouse](https://github.com/wladimir-computin/Android-CryptoHouse.git)
	- Natively written Android-App for controlling all CryptoIoT devices
