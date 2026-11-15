#include "saveLoad.h"
#include <fstream>
#include <iostream>
#include <string>

static int findItemIndex(const std::vector<std::shared_ptr<item>>& inventory, const std::shared_ptr<item>& target) {
    if (!target) return -1;
    for (size_t i = 0; i < inventory.size(); ++i) {
        if (inventory[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void saveGame(const player& p, const Room* currentRoom, const std::vector<std::shared_ptr<item>>& inventory) {
        std::ofstream saveFile("savefile.txt");
    if (saveFile.is_open()) {
        saveFile << p.name << "\n";
        saveFile << p.playerLvl << "\n";
        saveFile << p.playerXP << "\n";
        saveFile << p.playerHP << "\n";
        saveFile << p.playerMaxHP << "\n";
        saveFile << p.playerMana << "\n";
        saveFile << p.playerMaxMana << "\n";
        saveFile << p.playerBaseDamage << "\n";
        saveFile << p.playerAC << "\n";
        saveFile << p.dataBalanceKB << "\n";
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
            saveFile << itemPtr->acBonus << "\n";
            saveFile << itemPtr->equipSlot << "\n";
        }

        saveFile << findItemIndex(inventory, p.equippedWeapon) << "\n";
        for (int slot = 1; slot <= 5; ++ slot) {
            auto it = p.equippedArmour.find(slot);
            std::shared_ptr<item> armourPtr = (it != p.equippedArmour.end()) ? it->second : nullptr;
            saveFile << findItemIndex(inventory, armourPtr) << "\n";
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
    loadFile >> p.name;
    loadFile >> p.playerLvl;
    loadFile >> p.playerXP;
    loadFile >> p.playerHP;
    loadFile >> p.playerMaxHP;
    loadFile >> p.playerMana;
    loadFile >> p.playerMaxMana;
    loadFile >> p.playerBaseDamage;
    loadFile >> p.playerAC;
    loadFile >> p.dataBalanceKB;
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
    p.equippedArmour.clear();

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

            loadFile >> loadedItem.acBonus >> loadedItem.equipSlot;
            loadFile.ignore();

            inventory.push_back(std::make_shared<item>(loadedItem));
        }

        int weaponIdx = -1;
        if (loadFile >> weaponIdx && weaponIdx >= 0 && static_cast<size_t>(weaponIdx) < inventory.size()) {
            p.equippedWeapon = inventory[weaponIdx];
        }

        for (int slot = 1; slot <= 5; ++slot) {
            int armourIdx = -1;
            if (loadFile >> armourIdx && armourIdx >= 0 && static_cast<size_t>(armourIdx) < inventory.size()) {
                p.equippedArmour[slot] = inventory[armourIdx];
            }
        }
    }

    loadFile.close();
    return true;
}