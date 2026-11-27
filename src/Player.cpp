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
    int total = playerAC + getModifier(dex);
    for (const auto& [slot, armourPiece] : equippedArmour) {
        if (armourPiece != nullptr) {
            total += armourPiece->acBonus;
        }
    }
    if (spec == Specialization::Frontend) {
        total += 1;
    }
    total += getEffectBonus(StatusType::firewalled);
    return total;
}

int xpNeededForLevel(int level) {
    constexpr int n = static_cast<int>(std::size(kXpToLevelUp));
    if (level < 1) level = 1;
    if (level <= n) return kXpToLevelUp[level - 1];
    return static_cast<int>(kXpToLevelUp[n - 1] * std::pow(1.5, level - n));
}

void player::chooseBackground() {
    std::cout << "\n[MIMIR]: 'Loggfører kompetanseprofil før sesjonsstart. Velg din bakgrunn:'\n";
    std::cout << "  [1] Selvlært koder     (INT 14, DEX 12, CON 10, STR 8)  - Kjapp i hodet, skarp på tastaturet.\n";
    std::cout << "  [2] Drift og support   (STR 14, CON 12, DEX 10, INT 8)  - Vant til tunge servere og sinte brukere.\n";
    std::cout << "  [3] Informatikkstudent (CON 12, INT 12, DEX 10, STR 10) - Herdet av lange netter på lesesalen.\n";

    while (true) {
        std::cout << "Velg bakgrunn (1-3): > ";
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            background = Background::SelfTaught;
            intel = 14; dex = 12; con = 10; str = 8;
            std::cout << "[MIMIR]: 'Profil satt: Selvlært koder.'\n";
            break;
        } else if (choice == "2") {
            background = Background::Support;
            str = 14; con = 12; dex = 10; intel = 8;
            std::cout << "[MIMIR]: 'Profil satt: Drift og support.'\n";
            break;
        } else if (choice == "3") {
            background = Background::Student;
            con = 12; intel = 12; dex = 10; str = 10;
            std::cout << "[MIMIR]: 'Profil satt: Informatikkstudent.'\n";
            break;
        }
        std::cout << "[MIMIR]: 'Ugyldig valg. Tast 1, 2 eller 3.'\n";
    }
    //Oppdaterer start-HP og start-Mana basert på stats
    playerMaxHP += getModifier(con) * 2;
    playerHP = playerMaxHP;
    playerMaxMana += getModifier(intel) * 2;
    playerMana = playerMaxMana;
}

void player::chooseSpecialization() {
    std::cout << "\n====================================================================\n";
    std::cout << "   DU HAR NÅDD LEVEL 3! TID FOR Å VELGE FAGRETNING\n";
    std::cout << "====================================================================\n";
    std::cout << "  [1] Backend-utvikler   (+2 INT, +1 CON) | Spells gjør +2 ekstra skade.\n";
    std::cout << "  [2] Frontend-utvikler  (+2 DEX, +1 INT) | +1 ekstra AC og +2 på kastevåpen.\n";
    std::cout << "  [3] Sysadmin / DevOps  (+2 STR, +1 CON) | +2 ekstra skade i nærkamp.\n";

    while (true) {
        std::cout << "Velg spesialisering (1-3): > ";
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            spec = Specialization::Backend;
            intel += 2; con += 1;
            std::cout << "[MIMIR]: 'Tilgang oppgradert: Backend-utvikler!'\n";
            break;
        } else if (choice == "2") {
            spec = Specialization::Frontend;
            dex += 2; intel += 1;
            std::cout << "[MIMIR]: 'Tilgang oppgradert: Frontend-utvikler!'\n";
            break;
        } else if (choice == "3") {
            spec = Specialization::Sysadmin;
            str += 2; con += 1;
            std::cout << "[MIMIR]: 'Tilgang oppgradert: Sysadmin / DevOps!'\n";
            break;
        }
        std::cout << "[MIMIR]: 'Ugyldig valg. Tast 1, 2 eller 3.'\n";
    }
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

    playerMaxHP += rollDice(10) + 1 + getModifier(con);
    playerHP = playerMaxHP;
    playerBaseDamage += 1;
    playerAC += 1;

    playerMaxMana += 3 + getModifier(intel);
    playerMana = playerMaxMana;

    if (playerLvl == 3) {
        chooseSpecialization();
    }
}