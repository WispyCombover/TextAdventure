#include "Player.h"
#include "Utils.h"
#include <iostream>
#include <cmath>
#include <iterator>

static std::string getSlotName(int slot) {
    switch (slot) {
        case SlotHead: return "Hode";
        case SlotBody: return "Kropp";
        case SlotArms: return "Armer";
        case SlotLegs: return "Ben";
        case SlotOffHandShield: return "Shield";
        default:       return "Ukjent";
    }
}

int player::getTotalAC() const {
    int total = playerAC;
    for (const auto& [slot, armourPiece] : equippedArmour) {
        if (armourPiece != nullptr) {
            total += armourPiece->acBonus;
        }
    }
    return total;
}

int xpNeededForLevel(int level) {
    constexpr int n = static_cast<int>(std::size(kXpToLevelUp));
    if (level < 1) level = 1;
    if (level <= n) return kXpToLevelUp[level - 1];
    return static_cast<int>(kXpToLevelUp[n - 1] * std::pow(1.5, level - n));
}

void player::gainXp(int amount) {
    playerXP += amount;
    std::cout << "\n[XP] Du fikk " << amount << " XP!\n";

    while (playerXP >= xpToNextLevel) {
        playerXP -= xpToNextLevel;
        applyLevelUp();

        std::cout << "\n🎉 LEVEL UP! Du er nå på level " << playerLvl << "!\n";
        std::cout << "HP økte til " << playerMaxHP << ", og din base damage er nå " << playerBaseDamage << ".\n";
    }
}

void player::applyLevelUp() {
    playerLvl++;
    xpToNextLevel = xpNeededForLevel(playerLvl);

    playerMaxHP += rollDice(10) + 1;
    playerHP = playerMaxHP;
    playerBaseDamage += 1;
    playerAC += 1;

    playerMaxMana += 3;
    playerMana = playerMaxMana;
}