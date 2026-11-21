#include "saveLoad.h"
#include "Monster.h"
#include "NPC.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <system_error>

namespace {

constexpr const char* kSaveFile  = "savefile.txt";
constexpr const char* kSaveMagic = "MIMIRSAVE";
constexpr int kSaveVersion = 2;     // Øk denne når formatet endres
constexpr int kArmourSlots = 5;     // slot 1..5 - se ArmourSlot i Item.h

// ===================== LAGRINGSFIL DATA  =====================

struct ItemSave {
    std::string name;
    std::string contents;           
};

struct NpcSave {
    std::string name;
    std::string given;              
    std::vector<ItemSave> shop;
};

struct DoorSave {
    std::string id;
    bool isOpen = false;
    bool isLocked = false;
};

struct RoomSave {
    std::string name;
    bool hasMonster = false;
    std::string monsterName;
    int monsterLevel = 0;
    int monsterHp = 0;
    std::vector<ItemSave> items;
    std::vector<std::string> hiddenKeys;    // skjulte gjenstander som ennå ikke er funnet
    std::vector<NpcSave> npcs;
};

struct SaveData {
    std::string name;
    int level = 1;
    int xp = 0;
    int xpToNext = 100;
    int hp = 20;
    int maxHp = 20;
    int mana = 10;
    int maxMana = 10;
    int baseDamage = 2;
    int ac = 10;
    long long dataKB = 0;
    std::string room;
    std::vector<ItemSave> inventory;
    int weaponIdx = -1;
    int armourIdx[kArmourSlots] = {-1, -1, -1, -1, -1};
    std::vector<DoorSave> doors;
    std::vector<RoomSave> rooms;
};

// ===================== LASTING =====================

class Reader {
public:
    explicit Reader(std::istream& in) : in_(in) {}

    std::string line() {
        std::string s;
        if (!std::getline(in_, s)) {
            ok_ = false;
            return "";
        }
        if (!s.empty() && s.back() == '\r') s.pop_back();
        return s;
    }

    long long num() {
        std::string s = line();
        try {
            size_t used = 0;
            long long v = std::stoll(s, &used);
            if (used != s.size()) ok_ = false;
            return v;
        } catch (...) {
            ok_ = false;
            return 0;
        }
    }

    size_t count() {
        long long n = num();
        if (n < 0 || n > 10000) {
            ok_ = false;
            return 0;
        }
        return static_cast<size_t>(n);
    }

    bool ok() const { return ok_; }

private:
    std::istream& in_;
    bool ok_ = true;
};

ItemSave readItem(Reader& r) {
    ItemSave s;
    s.name = r.line();
    s.contents = r.line();
    return s;
}

bool readSave(std::istream& in, SaveData& d) {
    Reader r(in);

    if (r.line() != kSaveMagic) return false;
    if (r.num() != kSaveVersion) return false;

    d.name       = r.line();
    d.level      = static_cast<int>(r.num());
    d.xp         = static_cast<int>(r.num());
    d.xpToNext   = static_cast<int>(r.num());
    d.hp         = static_cast<int>(r.num());
    d.maxHp      = static_cast<int>(r.num());
    d.mana       = static_cast<int>(r.num());
    d.maxMana    = static_cast<int>(r.num());
    d.baseDamage = static_cast<int>(r.num());
    d.ac         = static_cast<int>(r.num());
    d.dataKB     = r.num();
    d.room       = r.line();

    size_t invCount = r.count();
    for (size_t i = 0; i < invCount && r.ok(); ++i) d.inventory.push_back(readItem(r));

    d.weaponIdx = static_cast<int>(r.num());
    for (int i = 0; i < kArmourSlots; ++i) d.armourIdx[i] = static_cast<int>(r.num());

    size_t doorCount = r.count();
    for (size_t i = 0; i < doorCount && r.ok(); ++i) {
        DoorSave ds;
        ds.id = r.line();
        ds.isOpen = (r.num() != 0);
        ds.isLocked = (r.num() != 0);
        d.doors.push_back(ds);
    }

    size_t roomCount = r.count();
    for (size_t i = 0; i < roomCount && r.ok(); ++i) {
        RoomSave rs;
        rs.name = r.line();

        rs.hasMonster = (r.num() != 0);
        if (rs.hasMonster) {
            rs.monsterName  = r.line();
            rs.monsterLevel = static_cast<int>(r.num());
            rs.monsterHp    = static_cast<int>(r.num());
        }

        size_t n = r.count();
        for (size_t k = 0; k < n && r.ok(); ++k) rs.items.push_back(readItem(r));

        n = r.count();
        for (size_t k = 0; k < n && r.ok(); ++k) rs.hiddenKeys.push_back(r.line());

        n = r.count();
        for (size_t k = 0; k < n && r.ok(); ++k) {
            NpcSave ns;
            ns.name  = r.line();
            ns.given = r.line();
            size_t shopCount = r.count();
            for (size_t s = 0; s < shopCount && r.ok(); ++s) ns.shop.push_back(readItem(r));
            rs.npcs.push_back(ns);
        }

        d.rooms.push_back(rs);
    }

    if (r.line() != "END") return false;   
    return r.ok();
}


int findItemIndex(const std::vector<std::shared_ptr<item>>& inventory, const std::shared_ptr<item>& target) {
    if (!target) return -1;
    for (size_t i = 0; i < inventory.size(); ++i) {
        if (inventory[i] == target) return static_cast<int>(i);
    }
    return -1;
}

Room* findRoom(const std::vector<std::unique_ptr<Room>>& allRooms, const std::string& name) {
    for (const auto& room : allRooms) {
        if (room->name == name) return room.get();
    }
    return nullptr;
}

// Hver dør har en unik id (satt i WorldBuilder). Begge sider av en dør deler samme Door-objekt.
std::map<std::string, Room::Door*> collectDoors(const std::vector<std::unique_ptr<Room>>& allRooms) {
    std::map<std::string, Room::Door*> doors;
    for (const auto& room : allRooms) {
        for (const auto& [dir, ex] : room->exits) {
            if (ex.door && !ex.door->id.empty()) {
                doors[ex.door->id] = ex.door.get();
            }
        }
    }
    return doors;
}

item makeItem(const ItemSave& s) {
    item it = getItem(s.name);
    if (!s.contents.empty()) fillContainer(it, s.contents);
    return it;
}

void writeItem(std::ostream& out, const item& it) {
    out << it.name << "\n" << it.contents << "\n";
}

std::string givenFlags(const NPC& npc) {
    std::string s;
    for (const auto& node : npc.dialogueTree) {
        for (const auto& opt : node.options) s += opt.itemGiven ? '1' : '0';
    }
    return s;
}

void applyGivenFlags(NPC& npc, const std::string& flags) {
    size_t i = 0;
    for (auto& node : npc.dialogueTree) {
        for (auto& opt : node.options) {
            if (i < flags.size()) opt.itemGiven = (flags[i] == '1');
            ++i;
        }
    }
}

const NpcSave* findNpcSave(const RoomSave& rs, const std::string& name) {
    for (const auto& n : rs.npcs) {
        if (n.name == name) return &n;
    }
    return nullptr;
}

// ===================== WRITE =====================

void writeSave(std::ostream& out,
               const player& p,
               const Room* currentRoom,
               const std::vector<std::unique_ptr<Room>>& allRooms,
               const std::vector<std::shared_ptr<item>>& inventory)
{
    out << kSaveMagic << "\n" << kSaveVersion << "\n";

    out << p.name << "\n";
    out << p.playerLvl << "\n";
    out << p.playerXP << "\n";
    out << p.xpToNextLevel << "\n";
    out << p.playerHP << "\n";
    out << p.playerMaxHP << "\n";
    out << p.playerMana << "\n";
    out << p.playerMaxMana << "\n";
    out << p.playerBaseDamage << "\n";
    out << p.playerAC << "\n";
    out << p.dataBalanceKB << "\n";
    out << currentRoom->name << "\n";

    out << inventory.size() << "\n";
    for (const auto& itemPtr : inventory) writeItem(out, *itemPtr);

    out << findItemIndex(inventory, p.equippedWeapon) << "\n";
    for (int slot = 1; slot <= kArmourSlots; ++slot) {
        auto it = p.equippedArmour.find(slot);
        out << (it != p.equippedArmour.end() ? findItemIndex(inventory, it->second) : -1) << "\n";
    }

    auto doors = collectDoors(allRooms);
    out << doors.size() << "\n";
    for (const auto& [id, door] : doors) {
        out << id << "\n" << (door->isOpen ? 1 : 0) << "\n" << (door->isLocked ? 1 : 0) << "\n";
    }

    out << allRooms.size() << "\n";
    for (const auto& room : allRooms) {
        out << room->name << "\n";

        if (room->occupant) {
            out << 1 << "\n" << room->occupant->name << "\n"
                << room->occupant->level << "\n" << room->occupant->hp << "\n";
        } else {
            out << 0 << "\n";
        }

        out << room->items.size() << "\n";
        for (const auto& it : room->items) writeItem(out, it);

        out << room->hiddenItems.size() << "\n";
        for (const auto& [key, itemName] : room->hiddenItems) out << key << "\n";

        out << room->npcs.size() << "\n";
        for (const auto& npc : room->npcs) {
            out << npc->name << "\n" << givenFlags(*npc) << "\n";
            out << npc->shopInventory.size() << "\n";
            for (const auto& it : npc->shopInventory) writeItem(out, it);
        }
    }

    out << "END\n";
}

} // namespace

void saveGame(const player& p,
              const Room* currentRoom,
              const std::vector<std::unique_ptr<Room>>& allRooms,
              const std::vector<std::shared_ptr<item>>& inventory)
{
    // Skriv til en midlertidig fil først. Krasjer noe underveis, er forrige lagring intakt.
    const std::string tmpFile = std::string(kSaveFile) + ".tmp";
    {
        std::ofstream out(tmpFile);
        if (!out) {
            std::cout << "Feil: Kunne ikke opprette lagringsfil.\n";
            return;
        }
        writeSave(out, p, currentRoom, allRooms, inventory);
        out.flush();
        if (!out) {
            std::cout << "Feil: Klarte ikke å skrive hele lagringsfilen.\n";
            return;
        }
    }   

    std::error_code ec;
    std::filesystem::rename(tmpFile, kSaveFile, ec);
    if (ec) {   
        std::filesystem::remove(kSaveFile, ec);
        std::filesystem::rename(tmpFile, kSaveFile, ec);
    }
    if (ec) {
        std::cout << "Feil: Kunne ikke fullføre lagringen (" << ec.message() << ").\n";
        return;
    }

    std::cout << " Spillet og inventaret ditt ble lagret!\n";
}

bool loadGame(player& p,
              Room*& currentRoom,
              const std::vector<std::unique_ptr<Room>>& allRooms,
              std::vector<std::shared_ptr<item>>& inventory)
{
    std::ifstream in(kSaveFile);
    if (!in.is_open()) {
        std::cout << "Fant ingen lagringsfil.\n";
        return false;
    }

    // ---- 1. LES ----
    SaveData d;
    if (!readSave(in, d)) {
        std::cout << "Lagringsfilen er skadet, eller lagret med en annen versjon av spillet.\n";
        return false;
    }

    // ---- 2. KONTROLLER ----
    Room* savedRoom = findRoom(allRooms, d.room);
    if (!savedRoom) {
        std::cout << "Lagringsfilen peker på et rom som ikke finnes ('" << d.room << "').\n";
        return false;
    }


    // Spiller
    p.name           = d.name;
    p.playerLvl      = d.level;
    p.playerXP       = d.xp;
    p.xpToNextLevel  = d.xpToNext;
    p.playerHP       = d.hp;
    p.playerMaxHP    = d.maxHp;
    p.playerMana     = d.mana;
    p.playerMaxMana  = d.maxMana;
    p.playerBaseDamage = d.baseDamage;
    p.playerAC       = d.ac;
    p.dataBalanceKB  = d.dataKB;

    // Midlertidig tilstand lagres ikke
    p.activeEffects.clear();
    p.sudoActive = false;
    p.lastRoom = nullptr;

    // Inventar og utstyr
    inventory.clear();
    for (const auto& is : d.inventory) {
        inventory.push_back(std::make_shared<item>(makeItem(is)));
    }

    auto itemAt = [&inventory](int idx) -> std::shared_ptr<item> {
        if (idx >= 0 && static_cast<size_t>(idx) < inventory.size()) return inventory[idx];
        return nullptr;
    };
    p.equippedWeapon = itemAt(d.weaponIdx);
    p.equippedArmour.clear();
    for (int slot = 1; slot <= kArmourSlots; ++slot) {
        if (auto piece = itemAt(d.armourIdx[slot - 1])) p.equippedArmour[slot] = piece;
    }

    // Dører
    auto doors = collectDoors(allRooms);
    for (const auto& ds : d.doors) {
        auto it = doors.find(ds.id);
        if (it == doors.end()) continue;        
        it->second->isOpen = ds.isOpen;
        it->second->isLocked = ds.isLocked;
    }

    // Rom: monstre, gjenstander, skjulte ting, NPCer
    monsterManual manual;
    for (const auto& rs : d.rooms) {
        Room* room = findRoom(allRooms, rs.name);
        if (!room) continue;

        if (!rs.hasMonster) {
            room->occupant = nullptr;
        } else if (room->occupant) {
            auto saved = manual.getMonsterBlueprint(rs.monsterLevel, rs.monsterName);
            if (saved) {
                saved->hp = std::clamp(rs.monsterHp, 1, saved->maxHp);
                room->occupant = std::move(saved);
            }
        }

        room->items.clear();
        for (const auto& is : rs.items) room->items.push_back(makeItem(is));

        for (auto it = room->hiddenItems.begin(); it != room->hiddenItems.end(); ) {
            bool stillHidden = std::find(rs.hiddenKeys.begin(), rs.hiddenKeys.end(), it->first) != rs.hiddenKeys.end();
            it = stillHidden ? std::next(it) : room->hiddenItems.erase(it);
        }

        auto& npcs = room->npcs;
        npcs.erase(
            std::remove_if(npcs.begin(), npcs.end(), [&rs](const std::unique_ptr<NPC>& npc) {
                return findNpcSave(rs, npc->name) == nullptr;   // NPC-en var borte da du lagret
            }),
            npcs.end()
        );
        for (auto& npc : npcs) {
            const NpcSave* ns = findNpcSave(rs, npc->name);
            applyGivenFlags(*npc, ns->given);
            if (npc->isMerchant) {
                npc->shopInventory.clear();
                for (const auto& is : ns->shop) npc->shopInventory.push_back(makeItem(is));
            }
        }
    }

    currentRoom = savedRoom;
    return true;
}
