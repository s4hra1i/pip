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
| Timekeeping | Pet ages while powered on. The physical switch pauses it. |
| Power | Choose a battery after measuring power consumption |

## Why the ATtiny85?

I'm building the shield to learn how to program ATtiny chips, so it makes sense to use one for the first version of the pet too.

The ATtiny85 has 8 KB of flash and 512 bytes of RAM. That means I'll need to be careful with memory, graphics and pin allocation, especially with an OLED and multiple buttons.

## Design rules

- Keep the pet logic separate from the hardware code so it can be tested on a laptop.
- Store sprites in flash rather than RAM.
- Keep the reset pin available so ISP programming still works.

## Pet rules (provisional, to be tested and balanced)

### Time and saving

- The pet ages while powered on, including when the screen is asleep.
- The physical switch pauses the pet on purpose. This is a feature of V1.
- Starting from full stats an ignored pet left powered on and awake has two mistakes and is sick by hour 60. See the test case.
- State is saved to EEPROM at sensible intervals. After a power loss the pet resumes from the last save.
- The watchdog is an approximate clock. Tests advance simulated time directly.
- All decay rates and timers are configurable so they can be tuned in tests.
- A well-looked-after pet can live indefinitely. There is no fixed lifespan.
- Running a real device for two to three weeks is a playtest to check the mechanics stay enjoyable.

### Life stages

- The pet has three life stages: egg, alive and dead.
- While alive the pet can independently be asleep or sick or both. These are flags and not separate states.
- The egg hatches after 30 minutes of powered-on time including while the screen is off. It gets no hunger, happiness, sickness or neglect before it hatches.
- On hatching all three stats are 255 with zero mistakes and cleared timers.
- On death the screen shows the pet's final age in days. Holding the Feed button for about three seconds restarts the pet.
- Restarting creates a new egg with fresh stats and zero mistakes and cleared timers. Nothing from the previous pet carries over except a small record of its age.
- The first press wakes the screen and a sleeping pet. That press does nothing else. Feeding and playing only happen on a later press.

### Stats

- Each stat is one byte from 0 to 255. Higher is better.
- Changes are capped at 255 and floored at 0. Overflow is discarded.
- Hunger (fullness) and happiness are critical at 40 or below and safe at 100 or above.
- Feeding gives +60 fullness. Playing gives +50 happiness and costs 30 energy. Sleeping gives +1 energy every 3 minutes.
- Feeding and playing have no cooldowns.
- Hunger decays 1 point per 7 minutes (about 25 hours from 255 to 40).
- Happiness decays 1 point per 9 minutes (about 32 hours from 255 to 40).
- Energy decays 1 point per 15 minutes while awake (about 64 hours from 255 to 0). It does not decay while asleep.
- Playing needs at least 30 energy. With less it is refused with no happiness gain and no penalty.
- Sleeping is always possible while hungry.
- Energy does not count towards neglect or mistakes.
- Decay uses an integer accumulator for each stat. Each minute it adds a weight and when it reaches a threshold one point is lost and the threshold is subtracted. The remainder carries over when the pet falls asleep or wakes or gets sick. Nothing resets it. No floats. The weights are in states.md.

### Sleep

- While asleep hunger and happiness decay four times slower. They never stop.
- Sleep and sickness modifiers stack. A sick pet that is asleep loses happiness at half the normal awake rate.
- The neglect clock keeps running while the pet is asleep.
- Sleep is refused at full energy (255).
- Sleep ends automatically when energy reaches 255.
- Fully recovering from zero energy takes 12 hours 45 minutes.

### Neglect and mistakes

- A neglect period opens when either need first goes critical. It closes when both needs are at 100 or above for one hour of powered-on time.
- One accumulated critical-time counter covers hunger and happiness together. It advances while either need is at 40 or below and pauses otherwise. Closing the period resets it.
- The first mistake is added when the counter reaches 6 hours. Another is added for every further 24 hours on the counter.
- Hunger and happiness both being low is still one period.
- Three active mistakes means death.
- One shared recovery timer clears one mistake per 72 hours of powered-on time. It only runs while the pet is not sick and has no open neglect period. It pauses and does not reset while the pet is sick or a neglect period is open. A new mistake resets it.
- Hunger decay, happiness decay, energy decay, critical time, recovery and sickness each have their own counter. They must not reset each other by accident.

### Sickness

- 12 continuous hours of hunger at 40 or below makes the pet sick. The timer resets if hunger rises above 40. It is separate from the mistake counter.
- While sick happiness decays twice as fast.
- Sickness clears after six continuous hours with hunger and happiness both at 100 or above. Any dip below 100 restarts the six hours.
- Sickness never adds a mistake. Recovering from it does not remove mistakes.
- Medicine is left for a later version.

### Test case: ignored pet starting at full stats and left awake

- Hour 25: hunger becomes critical.
- Hour 31: first mistake.
- Hour 37: sickness starts.
- Hour 55: second mistake.
- Hour 79: third mistake and death.

## Still to decide

- Display size: 128×64 or 128×32. Smaller sprites might be fine on the smaller display.
- Battery type. A CR2032 might struggle with the OLED's current peaks.
- EEPROM save interval and record format (a ring of records to spread the write cycles).
- Brown-out detection fuse settings.
- Pin allocation. See pinmap.md (draft, untested).
- Update interval for the pet logic, so tests are repeatable.
- How much flash to reserve for sprites and animation frames.
- Licences for the firmware and hardware designs.
