#include "CombatRules.h"
#include "Utils.h"
#include <algorithm>
#include <iostream>

namespace {
    int rollDiceSum(int count, int sides, bool crit) {
        int rolls = crit ? count * 2 : count;
        int total = 0;
        for (int i = 0; i < rolls; ++i) total += rollDice(sides);
        return total;
    }

    bool hasType(const std::vector<std::string>& list, const std::string& type) {
        std::string target = toLowerNorwegian(type);
        return std::any_of(list.begin(), list.end(), [&target](const std::string& entry) {
            return toLowerNorwegian(entry) == target;
        });
    }
}

int applyDamageModifiers(int baseDamage, const std::string& damageType, const monster& m) {
    if (damageType.empty() || baseDamage <= 0) return baseDamage;

    if (hasType(m.weaknesses, damageType)) {
        std::cout << "  -> SVAKHET! " << m.name << " tåler " << damageType << " dårlig! (Dobbel skade)\n";
        return baseDamage * 2;
    }
    if (hasType(m.resistances, damageType)) {
        std::cout << "  -> RESISTANS! " << m.name << " motstår " << damageType << "! (Halv skade)\n";
        return std::max(1, baseDamage / 2);
    }
    return baseDamage;
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

int rollPlayerMeleeDamage(const player& p, const monster& m, bool crit) {
    int flatBonus = p.playerBaseDamage + p.getEffectBonus(StatusType::caffeineRush);

    if (!p.equippedWeapon) {
        return applyDamageModifiers(flatBonus, "bludgeoning", m);
    }

    const item& w = *p.equippedWeapon;
    int primaryDmg = rollDiceSum(w.damageDiceCount, w.damageDiceSides, crit) + flatBonus;
    int totalDamage = applyDamageModifiers(primaryDmg, w.damageType, m);

    if (w.extraDiceCount > 0) {
        if (toLowerNorwegian(w.name) != "kaffekopp" || w.isFilled) {
            int extraDmg = rollDiceSum(w.extraDiceCount, w.extraDiceSides, crit);
            totalDamage += applyDamageModifiers(extraDmg, w.extraDamageType, m);
        }
    }
    return totalDamage;
}

int rollMonsterDamage(const monster& m, bool crit) {
    return rollDiceSum(m.damageDiceCount, m.damageDiceSides, crit) + m.attackDamage;
}