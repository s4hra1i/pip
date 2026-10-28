# Pin map (DRAFT)

Not final. This is a proposal to test on a breadboard.

| Pin | Name | Planned use | ISP use | Notes |
|---|---|---|---|---|
| 1 | PB5 | RESET | RESET | 10k pull-up to VCC. Never disable. |
| 2 | PB3 | Button ladder (ADC3) | none | Read by the ADC while awake. Wake from sleep would use a pin-change interrupt (PCINT3). Untested. |
| 3 | PB4 | Spare | none | Keep free for now |
| 4 | GND | GND | GND | |
| 5 | PB0 | OLED SDA (USI/I²C) | MOSI | Pull-up value to be checked (see below) |
| 6 | PB1 | Buzzer | MISO | Buzzer may click while programming |
| 7 | PB2 | OLED SCL (USI/I²C) | SCK | Pull-up value to be checked (see below) |
| 8 | VCC | VCC | VCC | 100nF capacitor close to the pin |

The USI is a shift register and not a complete I²C peripheral. The I²C behaviour comes from the firmware or library so the driver has to be chosen to match these pins.

## Pull-ups

The planned 4.7k pull-ups on SDA and SCL are not confirmed. Check them against:

- The OLED module. Many already have pull-ups fitted so adding more may lower the total resistance too far.
- The Arduino ISP programmer. SDA and SCL share lines with MOSI and SCK so the pull-ups load them during programming.

## To test on the breadboard

- OLED over USI and the ISP lines on the same pins at the same time. Check that programming still works with the OLED connected and that the shared lines don't interfere with each other.
- Whether every button in the ladder pulls PB3 far enough to trigger the pin-change interrupt including the one behind the largest resistor.
- Whether the chip wakes from power-down on a button press and then reads the ladder correctly with the ADC switched back on.
- Ladder readings at low battery voltage and not just at 5V.
