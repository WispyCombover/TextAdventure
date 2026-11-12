#pragma once
#include <string>
#include <memory>
#include "Item.h"

class Room;

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
    std::shared_ptr<item> equippedWeapon = nullptr;

    Room* lastRoom = nullptr;

    //koden ligger i player.cpp - viser til funksjonen her
    void gainXp(int amount);
};