# Proposal ❤️

**Proposal** is a small interactive embedded project built with Arduino, created as a personalized marriage/relationship proposal device.

The project combines a TFT display, physical buttons, a real-time clock and EEPROM memory to create an interactive experience that ends with a persistent relationship day counter.

## ✨ Features

- ❤️ Pixel-art and geometric heart animations
- 🖥️ 1.8" ST7735 TFT display
- 💾 EEPROM data persistence
- 🕐 DS1302 Real-Time Clock
- 🔘 Physical **YES / NO** buttons
- 📅 Automatically calculates the number of days since the proposal
- 🎨 Custom graphical interface designed specifically for the project
- 🔌 Standalone Arduino-based device

## 🛠️ Hardware

| Component | Description |
|---|---|
| Arduino UNO | Main microcontroller |
| ST7735 | 1.8" 128x160 TFT display |
| DS1302 | Real-Time Clock module |
| Push buttons | YES / NO interaction |
| EEPROM | Stores the relationship start date |
| 3D printed enclosure | Custom housing |

## 🔌 Wiring

### ST7735 TFT

| TFT | Arduino UNO |
|---|---|
| BL | 5V |
| VCC | 3.3V |
| GND | GND |
| CS | D10 |
| DC | D9 |
| RST | D6 |
| DIN | D11 |
| SCK | D13 |

### Buttons

| Button | Arduino UNO |
|---|---|
| YES | D7 → GND |
| NO | D8 → GND |

The buttons use the Arduino's internal `INPUT_PULLUP` resistors.

### DS1302 RTC

| DS1302 | Arduino UNO |
|---|---|
| VCC | 5V |
| GND | GND |
| CLK | D2 |
| DAT | D3 |
| RST | D4 |

All components share the same **GND**.

## 📚 Libraries

The project uses the following Arduino libraries:

- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit ST7735 and ST7789 Library](https://github.com/adafruit/Adafruit-ST7735-Library)
- [Rtc by Makuna](https://github.com/Makuna/Rtc)

`SPI` and `EEPROM` are included with the Arduino environment.

## 🎮 How it works

When the device starts, it checks the EEPROM to determine whether a proposal date has already been stored.

### First launch

The device displays:

> **QUERES SER**  
> **MI NOVIA?**

with two interactive buttons:

**[ SI ] [ NO ]**

### ❤️ YES

When **SI** is pressed:

1. The current date is read from the DS1302 RTC.
2. The date is stored in EEPROM.
3. A short heart animation is displayed.
4. The device shows **TE AMO**.
5. The relationship counter starts.

### ❌ NO

When **NO** is pressed, the device displays:

> **SEGURA?**  
> ❤️  
> **PENSA BIEN...**

After a short delay, the proposal screen appears again.

### 📅 Relationship counter

Once the date has been saved, the device displays the number of days that have passed:

> **LLEVAMOS**  
> **XXX**  
> **DIAS**  
> **DESDE: DD/MM/YYYY**

The date is stored in EEPROM, so it persists even after the Arduino is powered off.

## 🧠 Technical details

The project was designed without an external battery for the Arduino or RTC system. The device is intended to remain connected to external power.

The relationship counter calculates the elapsed number of **calendar days** between the stored start date and the current date provided by the DS1302.

The start date is only written when the **SI** button is pressed for the first time.
