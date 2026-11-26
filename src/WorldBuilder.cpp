#include "WorldBuilder.h"
#include "RoomBlueprint.h"
#include "Item.h"
#include "Monster.h"
#include "NPC.h"
#include "Player.h"
#include "Room.h"
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cstdint>

static std::string getMemoryAddress(const Room* roomPtr) {
    std::ostringstream oss;
    uintptr_t addr = reinterpret_cast<uintptr_t>(roomPtr);
    oss << "0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(sizeof(void*) * 2) << addr;
    return oss.str();
}

static std::string injectAddress(std::string text, const Room* targetRoom) {
    size_t pos = text.find("{ADDR}");
    if (pos != std::string::npos) {
        text.replace(pos, 6, getMemoryAddress(targetRoom));
    }
    return text;
}

template <typename T>
static void appendAll(std::vector<T>& dest, const std::vector<T>& src) {
    dest.insert(dest.end(), src.begin(), src.end());
}

Room* buildWorld(std::vector<std::unique_ptr<Room>>& allRooms, const player& p) {
    std::unordered_map<std::string, Room*> roomLookup;
    std::unordered_map<std::string, std::shared_ptr<Room::Door>> doorLookup;
    monsterManual manual;

    //Samler alle dører og rom fra alle etasjer
    std::vector<DoorBlueprint> allDoors;
    std::vector<RoomBlueprint> allBlueprints;

    //Etaseje U2 - papirarkivet
    appendAll(allBlueprints, getFloor_2Rooms());

    //Etasje 1 - IT
    appendAll(allDoors, getFloor1Doors());
    appendAll(allBlueprints, getFloor1Rooms());

    //Etasje 2
    //appendAll(allDoors, getFloor2Doors());
    //appendAll(allBlueprints, getFloor2Rooms());

    //Dører
    for (const auto& db : allDoors) {
        auto door = std::make_shared<Room::Door>();
        door->id = db.id;
        door->isLocked = db.locked;
        door->keyName = db.keyName;
        doorLookup[db.id] = door;
    }

    //Bygger alle rom og legger inn innhold
    for (const auto& bp : allBlueprints) {
        auto room = std::make_unique<Room>(bp.name, bp.description);
        room->discoveryXP = bp.discoveryXP;
        room->visited = bp.startsVisited;

        for (const auto& itemName : bp.items) {
            room->items.push_back(getItem(itemName));
        }

        for (const auto& sc : bp.scenery) {
            for (const auto& alias : sc.aliases) {
                room->scenery[alias] = sc.description;
                if (!sc.hiddenItem.empty()) {
                    room->hiddenItems[alias] = sc.hiddenItem;
                }
            }
        }

        if (bp.randomMonsterLevel > 0) {
            room->occupant = manual.getRandomMonster(bp.randomMonsterLevel);
        } else if (bp.fixedMonsterLevel > 0) {
            room->occupant = manual.getMonsterBlueprint(bp.fixedMonsterLevel, bp.fixedMonsterName);
        }

        for (const auto& makeNpc : bp.npcFactories) {
            room->npcs.push_back(makeNpc(p));
        }

        roomLookup[bp.id] = room.get();
        allRooms.push_back(std::move(room));
    }

    //Kobler rommene sammen og legger minneadresse
    for (const auto& bp : allBlueprints) {
        Room* current = roomLookup.at(bp.id);

        for (const auto& sc : bp.scenery) {
            if (!sc.addressTargetId.empty()) {
                Room* addrRoom = roomLookup.at(sc.addressTargetId);
                std::string resolvedText = injectAddress(sc.description, addrRoom);
                for (const auto& alias : sc.aliases) {
                    current->scenery[alias] = resolvedText;
                }
            }
        }

        for (const auto& ex : bp.exits) {
            Room* dest = roomLookup.at(ex.targetRoomId);
            std::shared_ptr<Room::Door> door = ex.doorId.empty() ? nullptr : doorLookup.at(ex.doorId);

            std::string desc = ex.inspectText;
            if (desc.find("{ADDR}") != std::string::npos) {
                desc = injectAddress(desc, dest);
            } else if (ex.appendTargetAddress) {
                desc += getMemoryAddress(dest);
            }

            current->exits[ex.direction] = { dest, door, desc };
        }
    }
    return roomLookup.at("spawn");    
}