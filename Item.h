#pragma once
#include <string>

struct item {
    std::string name;
    std::string description;
    int value;
    std::string currency;
    std::string type;
    int damageDiceSides = 0;
    int damageDiceCount = 0;
    std::string damageType;
    int extraDiceSides = 0;
    int extraDiceCount = 0;
    std::string extraDamageType = "";
    bool isFilled = false;
    bool throwable = false;
};