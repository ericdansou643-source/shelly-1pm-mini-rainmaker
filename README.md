# Shelly 1PM Mini Gen3 - ESP RainMaker Firmware

Custom firmware for **Shelly 1PM Mini Gen3** with native **ESP RainMaker** integration, BL0942 energy metering, and OTA updates.

## 📋 Features

- ✅ **ESP RainMaker Integration** - Native cloud management via Espressif's RainMaker platform
- ✅ **BL0942 Energy Meter** - Precise voltage, current, power, and energy measurements
- ✅ **Relay Control** - Full relay switching with state feedback
- ✅ **OTA Updates** - Over-the-Air firmware updates via RainMaker
- ✅ **Schedules & Scenes** - Support for automation via RainMaker app
- ✅ **Real-time Monitoring** - Live power consumption data
- ✅ **Multi-parameter Support** - Voltage, Current, Power, Energy tracking

## 🛠️ Hardware

**Device**: Shelly 1PM Mini Gen3 (ESP32-C3)

**GPIO Pinout**:
| Function | GPIO |
|----------|------|
| LED Status | GPIO0 |
| Button | GPIO1 |
| NTC Temp | GPIO3 |
| Relay Output | GPIO4 |
| BL0942 TX | GPIO6 |
| BL0942 RX | GPIO7 |
| Switch Input | GPIO10 |
| Serial Debug | GPIO19 |

## 📦 Prerequisites

- **ESP-IDF** v4.3 or later
- **ESP RainMaker SDK**
- USB-to-Serial adapter (1.27mm pitch for Shelly serial header)

## 🚀 Installation

### 1. Clone and Setup

```bash
git clone https://github.com/ericdansou643-source/shelly-1pm-mini-rainmaker.git
cd shelly-1pm-mini-rainmaker

# Set ESP-IDF path
export IDF_PATH=/path/to/esp-idf
source $IDF_PATH/export.sh

# Set RainMaker SDK path (if not in IDF_PATH)
export RMAKER_PATH=/path/to/esp-rainmaker
```

### 2. Configure Project

```bash
idf.py menuconfig
```

**Important Settings**:
- Target Chip: ESP32-C3
- Partition Table: Two OTA
- RainMaker: Enable OTA support

### 3. Flash to Device

```bash
# Build
idf.py build

# Flash (replace /dev/ttyUSB0 with your serial port)
idf.py -p /dev/ttyUSB0 flash

# Monitor
idf.py -p /dev/ttyUSB0 monitor
```

## 🔌 Wiring (Serial Flashing)

Shelly 1PM Mini Gen3 has a 6-pin header on the back (1.27mm pitch):

```
Pin 1: ESP_DBG_UART (not needed)
Pin 2: U0RXD (RX)
Pin 3: U0TXD (TX)
Pin 4: +3.3V
Pin 5: GND (or Pin 7)
Pin 6: GPIO0 (pull to GND to enter flash mode)
Pin 7: GND
```

**Connection**:
- USB-Serial GND → Shelly Pin 7
- USB-Serial TX → Shelly Pin 2 (RXD)
- USB-Serial RX → Shelly Pin 3 (TXD)
- USB-Serial 3.3V → Shelly Pin 4 (optional, for power)
- USB-Serial DTR/RTS → Shelly Pin 6 (for auto-reset)

## 📱 RainMaker App Setup

1. Download **RainMaker** app from App Store or Google Play
2. Create a RainMaker account
3. Scan the QR code or use BLE to provision the device
4. Device will appear in the app as "Shelly 1PM Mini"
5. Control power, view real-time energy metrics, and set schedules

## 📊 Supported Parameters

| Parameter | Type | Access | Description |
|-----------|------|--------|-------------|
| Power | Boolean | R/W | Relay on/off control |
| Voltage | Float | Read | Current voltage (V) |
| Current | Float | Read | Current current (A) |
| Power Consumption | Float | Read | Active power (W) |
| Energy | Float | Read | Accumulated energy (Wh) |

## ⚙️ Configuration Files

- **`main/shelly_hw.h`** - Hardware GPIO and UART configuration
- **`main/bl0942_driver.c`** - BL0942 UART communication protocol
- **`main/relay_control.c`** - Relay switching logic
- **`main/power_monitor.c`** - Power data acquisition task
- **`main/rainmaker_params.c`** - RainMaker parameter definitions
- **`main/app_main.c`** - Main application entry point

## 🐛 Debugging

Enable debug logs:

```bash
idf.py menuconfig
# Component Config → Log Output → Default Log Level → Debug
idf.py build
```

Serial output will show detailed logs for WiFi, RainMaker, and BL0942 operations.

## 🔄 OTA Updates

### Push a Firmware Update

1. Build a new binary: `idf.py build`
2. Find binary at: `build/shelly-1pm-mini-rainmaker.bin`
3. Host on HTTP/HTTPS server
4. Open RainMaker app → Settings → OTA Update
5. Enter firmware URL and confirm

Device will download, verify, and reboot into new firmware.

## ⚠️ Important Notes

- **Warranty**: Flashing custom firmware may void Shelly warranty
- **Backup Original**: Save the original firmware before flashing
- **Testing**: Test OTA updates in a safe environment first
- **Power Supply**: Use proper power supply (110-240VAC) for relay testing

## 📚 References

- [ESP RainMaker Documentation](https://docs.espressif.com/projects/esp-rainmaker/en/latest/)
- [ESP-IDF Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [BL0942 Datasheet](https://www.lcsc.com/datasheet/C2837510.pdf)
- [Shelly 1PM Mini Gen3 Specs](https://www.shelly.com/products/shelly-1pm-mini-gen3)

## 📝 License

This project is provided as-is for educational and personal use.

## 🤝 Contributing

Feel free to submit issues and enhancement requests!

---

**Last Updated**: 2026-07-03
**Author**: JoLeBon
**Status**: Active Development