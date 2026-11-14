#pragma once
#include <string>
#include <vector>

enum class itemEffect {
    none,
    healHp,             //Gjenoppretter HP med rollDice
    restoreMana,        //Gjenoppretter Mana med rollDice
    caffeineRush,       //Midlertidig buff (haste? bonus på hit-dice, dmg og AC?)
    sugarCrash          //Skader spilleren litt (gammel smultring / utgått brus / etc)
};

struct item {
    std::string name;
    std::string description;
    int value = 0;
    std::string currency = "kB";
    std::string type;
    int damageDiceSides = 0;
    int damageDiceCount = 0;
    std::string damageType;
    int extraDiceSides = 0;
    int extraDiceCount = 0;
    std::string extraDamageType = "";

    // Consumable / Potion stats
    itemEffect effect = itemEffect::none;
    int effectDiceSides = 0;
    int effectDiceCount = 0;
    int effectFlatBonus = 0;        //F.eks +2 (D&D Health potion: 2d4 + 2)
    std::string consumeText = "";   //Custom flavour text for når den drikkes/spises

    bool isFilled = false;
    bool throwable = false;
    bool isQuestItem = false;  //Sett denne til true på kritiske items
};

const std::vector<item>& itemList();
item getItem(const std::string& name);
bool itemExists(const std::string& name);
void inspectItem(const item& it);