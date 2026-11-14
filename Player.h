#pragma once
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#define NOMINMAX
#include <windows.h>
#include "Item.h"

class Room;

enum class StatusType {
    caffeineRush,           // +2 på hit-dice og dmg
    focused,                // +2 på spell-attack / redusert manakost
    sugarCrash              // -2 på hit-dice
};

struct StatusEffect {
    StatusType type;
    std::string name;
    int turnsRemaining;
    int magnitude;          // Styrke på effekt (+2 f.eks)
};

struct player {
    std::string name;  //Trenger en start game funksjon som ber om spillerens navn
    int playerLvl = 1;
    int playerXP = 0;
    int playerHP = 20;  //Man starter på level 1, men kanskje 20HP er lavt?
    int playerMaxHP = 20;
    long long dataBalanceKB = 1024;  //Spilleren starter med 1MB (om det er for lite/mye kan vi justere)
    int playerBaseDamage = 2;
    int xpToNextLevel = 100;
    int playerAC = 10;
    int playerMana = 10;
    int playerMaxMana = 10;
    int caffeineTurns = 0;
    bool sudoActive = false;   //Settes av sudo, forbrukes av spell
    std::shared_ptr<item> equippedWeapon = nullptr;

    std::vector<StatusEffect> activeEffects;

    void addEffect(StatusType type, const std::string& name, int turns, int magnitude) {
        for (auto& eff : activeEffects) {
            if (eff.type == type) {
                eff.turnsRemaining = (std::max)(eff.turnsRemaining, turns);
                return;
            }
        }
        activeEffects.push_back({type, name, turns, magnitude});
    }

    //Sjekk om spilleren har en aktiv effekt
    void tickEffects() {
        for (auto it = activeEffects.begin(); it != activeEffects.end(); ) {
            it->turnsRemaining--;

            if (it->turnsRemaining <= 0) {
                std::cout << "[Status] Effekten '" << it->name << "' har gått ut.\n";

                if (it->type == StatusType::caffeineRush) {
                    std::cout << "[Status] Hendene dine skjelver... Du får [Sukkerkrasj] (-2 angrep) i 1 runde!\n";
                    it = activeEffects.erase(it);
                    addEffect(StatusType::sugarCrash, "Sukkerkrasj", 1, -2);
                    continue;
                }

                it = activeEffects.erase(it);
            } else {
                ++it;
            }
        }
    }

    int getEffectBonus(StatusType type) const {
        for (const auto& eff : activeEffects) {
            if (eff.type == type) {
                return eff.magnitude;
            }
        }
        return 0;
    }

    Room* lastRoom = nullptr;

    //koden ligger i player.cpp - viser til funksjonen her
    void gainXp(int amount);
};