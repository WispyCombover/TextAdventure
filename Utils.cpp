#include "Utils.h"
#include <random>

int rollDice(int sides) {
    if (sides <= 0) return 0;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, sides);
    return dis(gen);
}