#include "CombatRules.h"
#include "Utils.h"

namespace {
    int rollDiceSum(int count, int sides, bool crit) {
        int rolls = crit ? count * 2 : count;
        int total = 0;
        for (int i = 0; i < count; ++i) total += rollDice(sides);
        return total;
    }
}

AttackRoll rollToHit(int attackBonus, int targetAC) {
    AttackRoll r;
    r.roll = rollDice(20);
    r.total = r.roll + attackBonus;
    r.crit = (r.roll == 20);
    r.fumble = (r.roll == 1);
    r.hit = r.crit || (!r.fumble && r.total >= targetAC);
    return r;
}

int playerAttackBonus(const player& p) {
    return p.playerLvl
        + p.getEffectBonus(StatusType::caffeineRush)
        + p.getEffectBonus(StatusType::sugarCrash);
}

int monsterAttackBonus(const monster& m) {
    return m.level;
}

int rollPlayerMeleeDamage(const player& p, bool crit) {
    int damage = p.playerBaseDamage + p.getEffectBonus(StatusType::caffeineRush);

    if (p.equippedWeapon) {
        const item& w = *p.equippedWeapon;
        damage += rollDiceSum(w.damageDiceCount, w.damageDiceSides, crit);
        damage += rollDiceSum(w.extraDiceCount, w.extraDiceSides, crit);
    }
    return damage;
}

int rollMonsterDamage(const monster& m, bool crit) {
    return rollDiceSum(m.damageDiceCount, m.damageDiceSides, crit) + m.attackDamage;
}