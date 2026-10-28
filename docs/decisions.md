# PocketBit V1 decisions

## Decided

| Decision | PocketBit V1 |
|---|---|
| Microcontroller | ATtiny85 (DIP-8 for prototyping) |
| Firmware | Embedded C/C++ |
| Display | SSD1306 OLED (128×64 for now) |
| Graphics | Page-based rendering, no full framebuffer |
| Controls | Three buttons using an ADC resistor ladder |
| Sound | Optional buzzer |
| Programming | Arduino Uno as ISP |
| PCB design | KiCad |
| Simulation | Wokwi, where the required components are supported |
| Unit testing | Native C++ tests on a laptop |
| Timekeeping | Pet ages while powered |
| Power | Choose a battery after measuring power consumption |

## Why the ATtiny85?

I'm building the shield to learn how to program ATtiny chips, so it makes sense to use one for the first version of the pet too.

The ATtiny85 has 8 KB of flash and 512 bytes of RAM. That means I'll need to be careful with memory, graphics and pin allocation, especially with an OLED and multiple buttons.

## Design rules

- Keep the pet logic separate from the hardware code so it can be tested on a laptop.
- Store sprites in flash rather than RAM.
- Keep the reset pin available so ISP programming still works.

## Still to decide

- Display size: 128×64 or 128×32. Smaller sprites might be fine on the smaller display.
- Battery type. A CR2032 might struggle with the OLED's current peaks.
- Whether the pet freezes when switched off or continues ageing using a low-power timekeeping approach.
- Whether to save the pet's state in EEPROM, potentially using a ring of records to spread the write cycles.
- Brown-out detection fuse settings.
- Pin allocation for the OLED, buttons, buzzer and ISP connections. Check the available pins and interface requirements before finalising the layout.
- Pet states: egg, awake, asleep, sick and dead. Decide whether the pet can be reborn.
- Update interval for the pet logic, so tests are repeatable.
- How much flash to reserve for sprites and animation frames.
- Licences for the firmware and hardware designs.
