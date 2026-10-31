#include "pet.h"

static const uint8_t kEggMinutes = 30;

void advanceMinute(Pet& pet) {
    if (pet.stage == LifeStage::Egg) {
        pet.eggMinutes++;
        if (pet.eggMinutes >= kEggMinutes) {
            pet.stage = LifeStage::Alive;
            pet.eggMinutes = 0;
            pet.hunger = 255;
            pet.happiness = 255;
            pet.energy = 255;
            pet.mistakes = 0;
        }
    }
}
