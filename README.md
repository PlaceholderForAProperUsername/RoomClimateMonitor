# RoomClimateMonitor (Under Development)

## Overview

The RoomClimateMonitor is a bare-metal C++ project for the Raspberry Pi Pico. It measures temperature, humidity, pressure,
luminance, light color, total volatile organic compounds (TVOC) and equivalent CO2 (eCO2), and shows the results on a
display.

The goal of this project is to practice embedded C++ development using templates, as described in 
[C++ in Embedded Systems](https://www.packtpub.com/en-us/product/c-in-embedded-systems-9781835881156). Specifically, 
the BitField (called BitType in this project) concept with template register access functions is adapted and expanded 
in this project (module rp2040/reg_access). A BitField/BitType describes the bits of a register (position, mask, allowed values, etc.)
and is used for static error checking, making using the bits safer and less error-prone at compile time.