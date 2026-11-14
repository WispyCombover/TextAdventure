#include "Player.h"
#include "Utils.h"
#include <iostream>

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
        total += armourPiece->acBonus;
    }
    return total;
}

void player::gainXp(int amount) {
    playerXP += amount;
    std::cout << "\n[XP] Du fikk " << amount << " XP!\n";

    while (playerXP >= xpToNextLevel) {
        playerXP -= xpToNextLevel;
        playerLvl++;
        xpToNextLevel = static_cast<int>(xpToNextLevel * 1.5);

        playerMaxHP += rollDice(10) + 1;
        playerHP = playerMaxHP;
        playerBaseDamage += 3;
        playerAC += 1;

        playerMaxMana += 3;
        playerMana = playerMaxMana;

        std::cout << "\n🎉 LEVEL UP! Du er nå på level " << playerLvl << "!\n";
        std::cout << "HP økte til " << playerMaxHP << ", og din base damage er nå " << playerBaseDamage << ".\n";
    }
}