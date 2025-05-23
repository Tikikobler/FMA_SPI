# FMA_SPI: Honeywell FMA Force Sensor Arduino SPI Library

![Arduino](https://img.shields.io/badge/Platform-Arduino-blue?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)

A modern C++ library for seamless SPI interfacing with Honeywell FMA force sensors. Effortlessly read force (mN) and temperature (°C) with calibration and error checking, optimized for Arduino/PlatformIO projects.

## Features

- 🟢 Simple, robust initialization & data reading
- 📐 Raw-to-physics (mN, °C) conversion included
- 🌡️ Accurate temperature readings & on-the-fly compensation
- ⚠️ Status bit checking for reliable hardware monitoring
- 🛠️ User-calibration for best results

## Supported Hardware

- Arduino-compatible boards
- Honeywell FMA series force sensors (SPI)

## Installation

1. **Copy Files:**
   - Place [`FMA_SPI.h`](FMA_SPI.h) and [`FMA_SPI.cpp`](FMA_SPI.cpp) into your `lib` or `src` directory.
2. **Include:**  
   ```cpp
   #include "FMA_SPI.h"
   ```

## Usage

### Initialization

```cpp
#include <SPI.h>
#include "FMA_SPI.h"

FMA_SPI forceSensor(SS); // Use your sensor’s SS pin

void setup() {
    Serial.begin(115200);
    SPI.begin();
    forceSensor.begin();
    // Optional calibration for precise results:
    // parameter : roomTemp, outputMax, outputMin, fullScale, Ratio
    forceSensor.calibrate(25.0, 16383, 4159, 5.0, 1.0); // Adjust as needed
    // Set zero point (tare) after mounting sensor, before measurements:
    forceSensor.set_zero();
}
```

### Reading Data

```cpp
void loop() {
    if (forceSensor.read()) {
        Serial.print("Force [mN]: ");
        Serial.println(forceSensor.getForce());
        Serial.print("Temp [°C]: ");
        Serial.println(forceSensor.getTemp());
    } else {
        Serial.println("Sensor read error");
    }
    delay(500);
}
```

### Printing Raw Data

```cpp
forceSensor.printRawData();
```

## Calibration

Call `calibrate(outputMax, outputMin, fullScale, Ratio, roomTemp)` for best accuracy:

- `outputMax`, `outputMin`: raw 14-bit sensor codes (from datasheet or experiment)
- `fullScale`: sensor's physical range (N)
- `Ratio`: mechanical ratio if using a lever system
- `roomTemp`: current ambient temperature (°C)

## API Reference

| Method        | Description                                                 |
|---------------|-------------------------------------------------------------|
| `begin()`     | Initializes SPI and configures sensor SS pin                |
| `calibrate(...)` | Adjusts scaling and compensation for your setup       |
| `read()`      | Reads force & temp, returns `true` if status is OK          |
| `getForce()`  | Returns last force reading (mN)                             |
| `getTemp()`   | Returns last temp reading (°C)                              |
| `printRawData()` | Prints raw SPI data to Serial                          |
| `set_zero()`   | Sets current sensor force as the new zero reference (tare)  |


## Notes

- Designed for Honeywell FMA SPI force sensors. Other sensors may require adjustments!
- Refer to the Honeywell FMA datasheet for details on sensor pinout & codes.

## Credits

- Inspired by work from JayasinghePasan
- Developed by TK, 2025

---

**MIT License**
