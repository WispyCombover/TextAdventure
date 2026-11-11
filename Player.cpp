#include "Player.h"
#include "Utils.h"
#include <iostream>

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

        std::cout << "\n🎉 LEVEL UP! Du er nå på level " << playerLvl << "!\n";
        std::cout << "HP økte til " << playerMaxHP << ", og din base damage er nå " << playerBaseDamage << ".\n";
    }
}