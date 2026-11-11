#pragma once
#include <string>
#include <memory>
#include "Item.h"

class Room;

struct player {
    std::string name;  //Trenger en start game funksjon som ber om spillerens navn
    int playerLvl = 1;
    int playerXP = 0;
    int playerHP = 20;
    int playerMaxHP = 20;
    int playerBaseDamage = 2;
    int xpToNextLevel = 100;
    int playerAC = 10;
    std::shared_ptr<item> equippedWeapon = nullptr;

    Room* lastRoom = nullptr;

    //koden ligger i player.cpp - viser til funksjonen her
    void gainXp(int amount);
};