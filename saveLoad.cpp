#include "saveLoad.h"
#include <fstream>
#include <iostream>
#include <string>

void saveGame(const player& p, const Room* currentRoom, const std::vector<std::shared_ptr<item>>& inventory) {
        std::ofstream saveFile("savefile.txt");
    if (saveFile.is_open()) {
        saveFile << p.playerLvl << "\n";
        saveFile << p.playerXP << "\n";
        saveFile << p.playerHP << "\n";
        saveFile << p.playerMaxHP << "\n";
        saveFile << p.playerBaseDamage << "\n";
        saveFile << p.playerAC << "\n";
        saveFile << currentRoom->name << "\n";

        saveFile << inventory.size() << "\n";

        for (const auto& itemPtr : inventory) {
            saveFile << itemPtr->name << "\n";
            saveFile << itemPtr->description << "\n";
            saveFile << itemPtr->value << "\n";
            saveFile << itemPtr->currency << "\n";
            saveFile << itemPtr->type << "\n";
            saveFile << itemPtr->damageDiceSides << "\n";
            saveFile << itemPtr->damageDiceCount << "\n";
            saveFile << itemPtr->damageType << "\n";
            saveFile << itemPtr->extraDiceSides << "\n";
            saveFile << itemPtr->extraDiceCount << "\n";
            saveFile << itemPtr->extraDamageType << "\n";
            saveFile << (itemPtr->isFilled ? 1 : 0) << "\n";
            saveFile << (itemPtr->throwable ? 1 : 0) << "\n";
        }

        saveFile.close();
        std::cout << " Spillet og inventaret ditt ble lagret!\n";
    } else {
        std::cout << "Feil: Kunne ikke opprette lagringsfil.\n";
    }
}

bool loadGame(player& p, Room*& currentRoom, const std::vector<std::unique_ptr<Room>>& allRooms, std::vector<std::shared_ptr<item>>& inventory) {
    std::ifstream loadFile("savefile.txt");
    if (!loadFile.is_open()) {
        return false;  //Finner ingen lagringsfil
    }

    std::string roomName;
    loadFile >> p.playerLvl;
    loadFile >> p.playerXP;
    loadFile >> p.playerHP;
    loadFile >> p.playerMaxHP;
    loadFile >> p.playerBaseDamage;
    loadFile >> p.playerAC;
    loadFile.ignore();
    std::getline(loadFile, roomName);
    //Finn riktig rom
    for (const auto& room : allRooms) {
        if (room->name == roomName) {
            currentRoom = room.get();
            break;
        }
    }
    //Tøm gammelt inventar før vi laster inn lagret
    inventory.clear();
    p.equippedWeapon = nullptr;

    size_t inventorySize = 0;
    if (loadFile >> inventorySize) {
        loadFile.ignore();

        for (size_t i = 0; i < inventorySize; ++i) {
            item loadedItem;
            std::getline(loadFile, loadedItem.name);
            std::getline(loadFile, loadedItem.description);
            loadFile >> loadedItem.value;
            loadFile.ignore();
            std::getline(loadFile, loadedItem.currency);
            std::getline(loadFile, loadedItem.type);
            loadFile >> loadedItem.damageDiceSides;
            loadFile >> loadedItem.damageDiceCount;
            loadFile.ignore();
            std::getline(loadFile, loadedItem.damageType);
            loadFile >> loadedItem.extraDiceSides;
            loadFile >> loadedItem.extraDiceCount;
            loadFile.ignore();
            std::getline(loadFile, loadedItem.extraDamageType);
            int isFilledInt, throwableInt;
            loadFile >> isFilledInt >> throwableInt;
            loadFile.ignore();
            loadedItem.isFilled = (isFilledInt == 1);
            loadedItem.throwable = (throwableInt == 1);

            inventory.push_back(std::make_shared<item>(loadedItem));
        }
    }
    loadFile.close();
    return true;
}