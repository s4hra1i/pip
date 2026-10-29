# Pet states (DRAFT)

Built from the rules in `decisions.md`. All timers count minutes of powered on time. This assumes a one minute logic tick. The tick interval is still to be decided.

## Life stages

| Stage | Meaning | What runs |
|---|---|---|
| Egg | Waiting to hatch | Egg timer only |
| Alive | Normal life with asleep and sick flags | Everything |
| Dead | Shows final age and waits for restart | Nothing |

## Stage changes

| From | Event | To | What changes |
|---|---|---|---|
| Egg | Egg timer reaches 30 minutes | Alive | Hunger, happiness and energy set to 255. Mistakes 0. All counters cleared. Age 0. Both flags off. Save now. |
| Alive | Third mistake added | Dead | Age at death is stored as the previous pet's age. Asleep and sick flags cleared. All timers stopped. Save now. |
| Dead | Feed button held for three seconds | Egg | Stats and timers reset. Mistakes 0. Previous pet's age kept. Save now. |

There are no other changes. An egg cannot die and a dead pet cannot be revived.

## Button presses

| Situation | Feed | Play | Sleep |
|---|---|---|---|
| Screen off | Wakes the screen. If the pet is asleep, it wakes too. Nothing else happens. | Same | Same |
| Egg | Ignored | Ignored | Ignored |
| Alive and awake | Hunger +60 | Needs 30 energy or more. Happiness +50 and energy −30. Otherwise refused. | Falls asleep. Refused at full energy. |
| Alive and asleep | Wakes only | Wakes only | Wakes only |
| Dead | Hold for three seconds to restart | Ignored | Ignored |


## Update order for each tick

- Egg: the egg timer goes up by one minute. At 30 minutes the egg hatches (see Stage changes). Nothing else runs.
- Dead: nothing runs.
- Alive: the eight steps below in this order.

1. Age goes up by one minute.
2. Hunger and happiness decay using the accumulators below.
3. Energy drops 1 point per 15 minutes while awake. While asleep it rises 1 point per 3 minutes and sleep ends at 255.
4. If hunger or happiness is at 40 or below the critical counter goes up by one and a neglect period opens if none is open. If a period is open and both needs are at 100 or above the period close timer goes up by one and otherwise it resets to 0. At 60 minutes the period closes and the critical counter and close timer reset to 0 -- mistakes stay.
5. A mistake is added when the critical counter reaches 360 or 1800 or 3240 minutes (6 hours and 30 hours and 54 hours). A new mistake resets the recovery timer.
6. Sickness. If not sick the sickness timer goes up while hunger is at 40 or below and resets to 0 when hunger is above 40. At 720 minutes the pet becomes sick and the timer resets to 0. If sick the same timer counts recovery. It goes up while hunger and happiness are both at 100 or above and resets to 0 otherwise. At 360 minutes the pet is well again and the timer resets to 0.
7. Mistake recovery. The recovery timer goes up only while the pet is not sick and has no open neglect period and has at least one mistake. It pauses and does not reset while sick or in an open period. At 4320 minutes one mistake is cleared and the timer resets to 0. With no mistakes the timer stays at 0.
8. If mistakes reach 3 the pet dies.

Doing the steps in this order every time keeps the tests repeatable.

## Decay accumulators

Each stat has an integer accumulator. Every minute the weight is added. When the accumulator reaches the threshold, one point is lost and the threshold is subtracted. The accumulator is never reset so the remainder carries over when the pet falls asleep, wakes or gets sick.

| Stat and condition | Added each minute | Threshold | Points lost every |
|---|---:|---:|---|
| Hunger awake | 4 | 28 | 7 minutes |
| Hunger asleep | 1 | 28 | 28 minutes |
| Happiness awake | 4 | 36 | 9 minutes |
| Happiness asleep | 1 | 36 | 36 minutes |
| Happiness sick and awake | 8 | 36 | 4.5 minutes |
| Happiness sick and asleep | 2 | 36 | 18 minutes |
| Energy awake (decay) | 1 | 15 | 15 minutes |
| Energy asleep (recovery) | 1 | 3 | 3 minutes gained |

Energy has two accumulators because it falls when awake and rises when asleep.

## Age

Age counts powered on time from hatching. It is stored in hours and shown in days. A day means 24 hours of powered on time.

## Stored data (about 22 bytes)

- Stage and asleep and sick and period-open flags (1 byte)
- Hunger, happiness and energy (3 bytes)
- Mistakes (1 byte)
- Decay accumulators for hunger and happiness and two for energy (4 bytes)
- Critical counter (2 bytes)
- Period-close timer (1 byte)
- Sickness timer (2 bytes). It is the onset timer when well and the recovery timer when sick.
- Mistake recovery timer (2 bytes)
- Egg timer (1 byte)
- Age in hours and a minute counter (3 bytes)
- Previous pet's age in hours (2 bytes)
