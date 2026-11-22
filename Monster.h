#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <random>
#include "Item.h"

struct lootEntry {
    item dropItem;
    int dropChancePercent;  // 1 - 100
};

struct monster {
    std::string name;
    std::string description;
    int level;
    int hp;
    int maxHp;
    int attackDamage;           //Fast bonus som legges til terningkast. Kan være 0 eller negativ
    int damageDiceCount = 0;    //Damage dice = damageDiceCount d damageDiceSides + attackDamage
    int damageDiceSides = 0;
    int xpReward;
    int monsterAC;
    std::vector<std::string> weaknesses;        //Slik kan ekstra damagtypes faktisk ha noe å si, og valg av våpen blir et strategisk poeng
    std::vector<std::string> resistances;
    std::string attackMessage;

    //LOOT
    long long minDataReward = 0;  // I ant kB
    long long maxDataReward = 0;  // I ant kB
    std::vector<lootEntry> lootTable;
};

inline double averageDamage(const monster& m) {
    return m.attackDamage + m.damageDiceCount * (m.damageDiceSides +1) / 2.0;
}

inline std::string damageText(const monster& m) {
    std::string s;
    if (m.damageDiceCount > 0) s = std::to_string(m.damageDiceCount) + "d" + std::to_string(m.damageDiceSides);
    if (m.attackDamage != 0 || s.empty()) {
        if (!s.empty() && m.attackDamage > 0) s += "+";
        s += std::to_string(m.attackDamage);
    }
    return s;
}

class monsterManual {
    private:
        std::map<int, std::vector<monster>> blueprintDatabase;
        std::mt19937 rng;
    
    public:
        monsterManual();
        std::unique_ptr<monster> getMonsterBlueprint(int level, const std::string& name);
        std::unique_ptr<monster> getRandomMonster(int level);
        const std::map<int, std::vector<monster>>& blueprints() const { return blueprintDatabase; }
};