#pragma once
#include "Player.h"
#include "Monster.h"

struct AttackRoll {
    int roll = 0;
    int total = 0;
    bool crit = false;
    bool fumble = false;
    bool hit = false;
};

AttackRoll rollToHit(int attackBonus, int targetAC);

int playerMeleeAttackBonus(const player& p);
int playerThrowAttackBonus(const player& p);
int monsterAttackBonus(const monster& m);

int rollPlayerMeleeDamage(const player& p, const monster& m, bool crit);
int rollPlayerThrowDamage(const player& p, const item& thrownItem, const monster& m, bool crit);
int applyDamageModifiers(int baseDamage, const std::string& damageType, const monster& m);
int rollMonsterDamage(const monster& m, bool crit);