
#ifndef POCKETBIT_PET_H
#define POCKETBIT_PET_H

#include <stdint.h>

enum class LifeStage : uint8_t {
    Egg,
    Alive,
    Dead
};

struct Pet {
    LifeStage stage = LifeStage::Egg;
    uint8_t eggMinutes = 0;

    uint8_t hunger = 0;
    uint8_t happiness = 0;
    uint8_t energy = 0;

    uint8_t mistakes = 0;
};

void advanceMinute(Pet& pet);

#endif
