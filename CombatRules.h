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

int playerAttackBonus(const player& p);
int monsterAttackBonus(const monster& m);

int rollPlayerMeleeDamage(const player& p, bool crit);
int rollMonsterDamage(const monster& m, bool crit);