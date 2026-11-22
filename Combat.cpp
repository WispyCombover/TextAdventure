#include "Combat.h"
#include "CombatRules.h"
#include "Consumables.h"
#include "Utils.h"
#include "Monster.h"
#include "Spells.h"
#include <iostream>
#include <algorithm>

#include "CommandHandler.h"

static void onMonsterDefeated(monster* m, player& p, Room* currentRoom) {
    p.gainXp(m->xpReward);

    //MONEY MONEY MONEY (kB)
    if (m->maxDataReward > 0) {
        long long droppedKB = rollRange(m->minDataReward, m->maxDataReward);

        if (droppedKB > 0) {
            p.dataBalanceKB += droppedKB;
            std::cout << "[LOOT] Du høstet " << formatDataCurrency(droppedKB) << " fra restene av " << m->name << "!\n";
        }
    }

    //ITEM DROPS
    for (const auto& entry : m->lootTable) {
        if (rollDice(100) <= entry.dropChancePercent) {
            currentRoom->items.push_back(entry.dropItem);
            std::cout << "[LOOT] " << m->name << " mistet: " << entry.dropItem.name << "!\n";
        }
    }

    currentRoom->occupant = nullptr;
}

//Har spilleren noen consumables?
static bool hasDrinkable(const std::vector<std::shared_ptr<item>>& inventory) {
    return std::any_of(inventory.begin(), inventory.end(), [](const std::shared_ptr<item>& i) {
        return i->type == "consumable" || (i->name == "kaffekopp" && i->isFilled);
    });
}

bool handleCombat(player& p, Room*& currentRoom, std::vector<std::shared_ptr<item>>& inventory) {
    monster* m = currentRoom->occupant.get();
    if (!m) return true; //Ingen monstre her, spillet fortsetter

    std::cout << "\n!!! SKREKK OG GRU !!!\n";
    std::cout << "Du møter på en " <<m->name <<"!\n";
    std::cout << m->description <<" (HP: " << m->hp << "/" << m->maxHp << ")\n";
    std::cout << "---------------------------------------\n";

    while (m->hp > 0 && p.playerHP > 0) {
        std::string prompt = "Hva gjør du? [angrip / flykt / kast <gjenstand> ";
        if (hasDrinkable(inventory)) prompt += " / drikk <gjenstand>";
        if (hasSpellbook(inventory)) prompt += " / cast <formel>";
        prompt += "]\n> ";
        std::cout << prompt;

        std::string combatInput;
        std::getline(std::cin, combatInput);
        combatInput = toLowerNorwegian(combatInput);

        //THROWABLES
        if (combatInput.rfind("kast ", 0) == 0) {
            std::string itemName = combatInput.substr(5);  //Virker lite robust. Bytt til å gå fra mellomrommet ved neste anledning

            auto it = std::find_if(inventory.begin(), inventory.end(), [&itemName](const std::shared_ptr<item>& i) {
                return toLowerNorwegian(i->name) == itemName;
            });

            if (it == inventory.end()) {
                std::cout << "Du har ikke en '" << itemName << "' i sekken din.\n";
                continue;  //Mister ikke runden sin, men bør kanskje vurdere å implementere en AP-mekanikk?
            }

            std::shared_ptr<item> thrownItem = *it;
            if (!thrownItem->throwable) {
                std::cout << "Du kan ikke kaste en " << thrownItem->name << ". Det gir ingen mening!\n";
                continue;
            }

            std::cout << "Du sikter og kaster din " << thrownItem->name << " hardt mot " << m->name << "!\n";

            AttackRoll toss = rollToHit(playerAttackBonus(p), m->monsterAC);

            if (toss.hit) {
                int damage = 0;
                const int diceMultiplier = toss.crit ? 2 : 1;

                if (toss.crit) std::cout << "CRITICAL HIT! Et perfekt kast!\n";

                // Base bludgeoning damage
                for (int i = 0; i < thrownItem->damageDiceCount * diceMultiplier; ++i) {
                    damage += rollDice(thrownItem->damageDiceSides);
                }
                damage = applyDamageModifiers(damage, thrownItem->damageType, *m);
                if (thrownItem->isFilled) {
                    //Koppen er full: gjør ekstra acid damage  Hmmm, dette fungerer for nå, men ønsker å utvide med flere forskjellige throwables!
                    int acidDamage = 0;
                    for (int i = 0; i < diceMultiplier; ++i) acidDamage += rollDice(6);

                    acidDamage = applyDamageModifiers(acidDamage, "acid", *m);
                    damage += acidDamage;
                    std::cout << "PLASK! " << thrownItem->name << " var full av glovarm kontorkaffe!\n";
                    std::cout << "Det gjør " << acidDamage << " ekstra ACID-skade på " << m->name << ".\n";

                    //AOE - Glatt gulv - reduserer monsterets AC
                    int acReduction = 2;
                    m->monsterAC -= acReduction;
                    std::cout << "Gulvet blir såpeglatt av sølet! " << m->name << " mister " << acReduction << " AC. (Ny AC: " << m->monsterAC << ").\n";
                } else if (thrownItem->isContainer) {
                    std::cout << thrownItem->name << " var tom og treffer med et tørt 'KLONK'!\n";
                }
                m->hp -= damage;
                std::cout << "Totalt gjorde kastet " << damage << " skade på " << m->name << ".\n";
            } else {
                std::cout << "Du bommet grovt! " << thrownItem->name << " seilte forbi monsteret.\n";
            }
            if (p.equippedWeapon == thrownItem) {
                p.equippedWeapon = nullptr;
            }
            //Sjanse for at koppen knuser 60%?  Hmmm, dette fungerer for nå, men ønsker å utvide med flere forskjellige throwables!
            if (rollDice(10) <= 6) {
                std::cout << "KRAASJ! Din " << thrownItem->name << " knuste i tusen knas når den traff!\n";

                if (!thrownItem->isFilled) {
                //Tom kopp genererer 1d8 caltrops på bakken - kan ikke en full kopp også knuse?
                    int caltropsCount = rollDice(8);
                    std::cout << "Skårene etter porselenet sprer seg og danner " << caltropsCount << " spisse caltrops på gulvet!\n";
                    //Logikk for AOE-skade dersom monsteret beveger på seg (tråkker på caltropsene)
                }
                inventory.erase(it);  //Fjerner koppen fra inventaret
            } else {
                std::cout << "Koppen rullet mirakuløst nok hel avgårde etter at den landet.\n";
                emptyContainer(*thrownItem);
                currentRoom->items.push_back(*thrownItem);
                inventory.erase(it);
            }
            if (m->hp <= 0) {
                std::cout << "Du beseiret " << m->name << " med et velrettet kast! Veien videre er klar.\n";
                onMonsterDefeated(m, p, currentRoom);
                return true;
            }
        }

        //DRIKKE - koster en runde, men kun dersom noe blir drukket
        else if (combatInput == "drikk" || combatInput.rfind("drikk", 0) == 0) {
            std::string what = (combatInput == "drikk") ? "": combatInput.substr(6);
            if (!drinkItem(p, inventory, what)) continue;
        }

        //SPELLCASTING
        else if (combatInput == "cast" || combatInput.rfind("cast ", 0) == 0) {
            if (combatInput == "cast") {
                listSpells(p, inventory);
                continue;
            }
            castResult r = castSpell(p, combatInput.substr(5), m, currentRoom, inventory);

            if (!spellUsedTurn(r)) continue;   //Feil input gjør ikke at man mister runden
            
            if (r == castResult::Escaped){
                currentRoom = p.lastRoom;
                printRoomDescription(currentRoom);
                return true;
            }

            if (r == castResult::PlayerDied) {
                std::cout << "\n=== GAME OVER ===\n";
                std::cout << "Du døde av din egen trylleformel. Mimir logger deg ut, og du tør sverge på at du så ham riste på hodet.\n";
                return false;
            }
            if (m->hp <= 0) {
                std::cout << "Du beseiret " << m->name << " med magi! Veien videre er klar.\n";
                onMonsterDefeated(m, p, currentRoom);
                return true;
            }
        }
        else if (combatInput == "angrip" || combatInput == "slå" || combatInput == "attack") {
            AttackRoll a = rollToHit(playerAttackBonus(p), m->monsterAC);

            std::cout << "Du triller en D20 for å treffe... du fikk " << a.roll << " (+ " << a.total - a.roll << " bonus) = " << a.total << " mot AC " << m->monsterAC << ".\n";

            if (a.hit) {
                if (p.equippedWeapon != nullptr) {
                    std::cout << "Du angriper med din " << p.equippedWeapon->name << "!\n";
                } else {
                    std::cout << "Du har ikke valgt et våpen, og slår løs med bare knyttnevene!\n";
                }

                int damageDealt = rollPlayerMeleeDamage(p, *m, a.crit);
                m->hp -= damageDealt;
                if (a.crit) {
                    std::cout << "CRITICAL HIT! Du gjorde " << damageDealt << " skade på " << m->name << ".\n";
                } else {
                    std::cout << "Du traff! Du gjorde " << damageDealt << " skade på " << m->name << ".\n";
                }
                if (m->hp <= 0) {
                    std::cout << "Du beseiret " << m->name << "! Veien videre er klar.\n";
                    onMonsterDefeated(m, p, currentRoom);
                    return true;
                }
            } else if (a.fumble) {
                std::cout << "FUMBLE! Du snubler i dine egne argumenter, mister balansen og skader deg selv!\n";
                p.playerHP -= 2;  //Mister 2HP på en fumble - kan revideres
                std::cout << "Du tok 2 skade. Din HP: " << p.playerHP << "/" << p.playerMaxHP << "\n";
            } else {
                std::cout << "Bom! Angrepet ditt preller av eller blir blokkert!.\n";
            }
        }
        else if (combatInput == "flykt" || combatInput == "løp" || combatInput == "røm") {
            if (p.lastRoom != nullptr) {
                std::cout << "Du får panikk, snur på hælen og løper tilbake til start!\n"; // Trenger lastRoom logikk. Lasroom-logikk er inne!
                currentRoom = p.lastRoom;
                printRoomDescription(currentRoom);
                return true;
            } else {
                std::cout << "Det er ingen steder å flykte! Du må kjempe!\n";
            }   
        }
        else {
            std::cout << "Du nøler, og det koster deg dyrt...\n";
        }
        if (p.playerHP <= 0) {
            std::cout << "\n=== GAME OVER ===\n";
            std::cout << "Du snublet i dine egne føtter. Mimir logger deg ut uten kommentar.\n";
            return false;
        }

        //Monsterets tur
        if (m->hp > 0) {
            std::cout << m->name << " " << m->attackMessage << "\n";

            AttackRoll b = rollToHit(monsterAttackBonus(*m), p.getTotalAC());

            std::cout << m->name << " triller for å treffe deg... Det fikk " << b.total << " mot din AC " << p.getTotalAC() << ".\n";

            if (b.hit) {
                int totalMonsterDamage = rollMonsterDamage(*m, b.crit);
                //Monsteret treffer
                p.playerHP -= totalMonsterDamage;
                if (b.crit) {
                    std::cout << "CRITICAL HIT! Du tok " << totalMonsterDamage << " skade! Din HP: " << p.playerHP << "/" << p.playerMaxHP << "\n";
                } else {
                    std::cout << "AU! Du tok " << totalMonsterDamage << " skade! Din HP: " << p.playerHP << "/" << p.playerMaxHP << "\n";
                }
                if (p.playerHP <= 0) {
                    std::cout << "\n=== GAME OVER ===\n";
                    std::cout << "Deadlinemonsteret vant denne gangen. Mimir er skuffet og logger deg ut!\n";
                    return false;
                }      
            } else {
                std::cout << "Flaks! Du dukker unna angrepet til " << m->name << ".\n";
            }
        }
        if (!p.activeEffects.empty()) {
            std::cout << "Aktive effekter: ";
            for (const auto& eff : p.activeEffects) {
                std::cout << "[" << eff.name << " (" << eff.turnsRemaining << " runder)] ";
            }
            std::cout << "\n";
        }
        p.tickEffects();
    }
    return true;
}