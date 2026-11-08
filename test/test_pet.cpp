
#include "pet.h"
#include <cstdio>

static int failures = 0;

#define CHECK(cond)                                              \
    do {                                                         \
        if (!(cond)) {                                           \
            std::printf("FAIL line %d: %s\n", __LINE__, #cond);  \
            ++failures;                                          \
        }                                                        \
    } while (0)

static void testNewPetIsAnEgg() {
    Pet pet;
    CHECK(pet.stage == LifeStage::Egg);
    CHECK(pet.eggMinutes == 0);
}

static void testEggHatchesAtExactly30Minutes() {
    Pet pet;
    for (int i = 0; i < 29; ++i) advanceMinute(pet);
    CHECK(pet.stage == LifeStage::Egg);
    advanceMinute(pet);
    CHECK(pet.stage == LifeStage::Alive);
    CHECK(pet.hunger == 255);
    CHECK(pet.happiness == 255);
    CHECK(pet.energy == 255);
    CHECK(pet.mistakes == 0);
}

static void testHatchedPetStaysAlive() {
    Pet pet;
    for (int i = 0; i < 100; ++i) advanceMinute(pet);
    CHECK(pet.stage == LifeStage::Alive);
}

int main() {
    testNewPetIsAnEgg();
    testEggHatchesAtExactly30Minutes();
    testHatchedPetStaysAlive();

    if (failures == 0) {
        std::printf("All tests passed\n");
        return 0;
    }
    std::printf("%d check(s) failed\n", failures);
    return 1;
}
