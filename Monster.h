#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <random>

struct monster {
    std::string name;
    std::string description;
    int level;
    int hp;
    int maxHp;
    int attackDamage;
    int xpReward;
    int monsterAC;
    std::string attackMessage;
};

class monsterManual {
    private:
        std::map<int, std::vector<monster>> blueprintDatabase;
        std::mt19937 rng;
    
    public:
        monsterManual();
        std::unique_ptr<monster> getMonsterBlueprint(int level, const std::string& name);
        std::unique_ptr<monster> getRandomMonster(int level);
};