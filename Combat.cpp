#include "Combat.h"
#include "Utils.h"
#include "Monster.h"
#include "Spells.h"
#include <iostream>
#include <algorithm>

#include "CommandHandler.h"

bool handleCombat(player& p, Room*& currentRoom, std::vector<std::shared_ptr<item>>& inventory) {
    monster* m = currentRoom->monster.get();
    if (!m) return true; //Ingen monstre her, spillet fortsetter

    std::cout << "\n!!! SKREKK OG GRU !!!\n";
    std::cout << "Du møter på en " <<m->name <<"!\n";
    std::cout << m->description <<" (HP: " << m->hp << "/" << m->maxHp << ")\n";
    std::cout << "---------------------------------------\n";

    while (m->hp > 0 && p.playerHP > 0) {
        std::cout << "Hva gjør du? [angrip / flykt / kast <gjenstand>\n> ";
        if (hasSpellbook(inventory)) {
            std::cout << " / cast <formel>";
        }
        std::cout << "]\n> ";
        std::string combatInput;
        std::getline(std::cin, combatInput);

        //THROWABLES
        if (combatInput.rfind("kast ", 0) == 0) {
            std::string itemName = combatInput.substr(5);  //Virker lite robust. Bytt til å gå fra mellomrommet ved neste anledning

            auto it = std::find_if(inventory.begin(), inventory.end(), [&itemName](const std::shared_ptr<item>& i) {
                return i->name == itemName;
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

            int hitRoll = rollDice(20);
            int totalHit = hitRoll + p.playerLvl;

            if (totalHit >= m->monsterAC) {
                int damage = 0;

                // Base bludgeoning damage
                for (int i = 0; i < thrownItem->damageDiceCount; ++i) {
                    damage += rollDice(thrownItem->damageDiceSides);
                }
                if (thrownItem->isFilled) {
                    //Koppen er full: gjør ekstra acid damage  Hmmm, dette fungerer for nå, men ønsker å utvide med flere forskjellige throwables!
                    int acidDamage = rollDice(6);
                    damage += acidDamage;
                    std::cout << "PLASK! " << thrownItem->name << " var full av glovarm kontorkaffe!\n";
                    std::cout << "Det gjør " << acidDamage << " ekstra ACID-skade på " << m->name << ".\n";

                    //AOE - Glatt gulv - reduserer monsterets AC
                    int acReduction = 2;
                    m->monsterAC -= acReduction;
                    std::cout << "Gulvet blir såpeglatt av sølet!" << m->name << " mister " << acReduction << " AC. (Ny AC: " << m->monsterAC << ").\n";
                } else {
                    std::cout << thrownItem->name << " var tom og treffer med et tørt 'KLONK'!\n";
                }
                m->hp -= damage;
                std::cout << "Totalt gjorde kastet " << damage << " skade på " << m->name << ".\n";
            } else {
                std::cout << "Du bommet grovt! " << thrownItem->name << " seilte forbi monsteret.\n";
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
                if (p.equippedWeapon == thrownItem) {
                p.equippedWeapon = nullptr;
                }
                inventory.erase(it);  //Fjerner koppen fra inventaret
            } else {
                std::cout << "Koppen rullet mirakuløst nok hel avgårde etter at den landet.\n";
                thrownItem->isFilled = false;
                currentRoom->items.push_back(*thrownItem);
                inventory.erase(it);
            }
            if (m->hp <= 0) {
                std::cout << "Du beseiret " << m->name << " med et velrettet kast! Veien videre er klar.\n";
                p.gainXp(m->xpReward);
                currentRoom->monster = nullptr;
                return true;
            }
        }
        else if (combatInput == "cast" || combatInput.rfind("cast ", 0) == 0) {
            if (combatInput == "cast") {
                listSpells(p, inventory);
                continue;
            }
            castResult r = castSpell(p, combatInput.substr(5), m, currentRoom, inventory);

            if (!spellUsedTurn(r)) continue;   //Feil input gjør ikke at man mister runden

            if (r == castResult::PlayerDied) {
                std::cout << "\n=== GAME OVER ===\n";
                std::cout << "Du døde av din egen trylleformel. Mimir logger deg ut, og du tør sverge på at du så ham riste på hodet.\n";
                return false;
            }
            if (m->hp <= 0) {
                std::cout << "Du beseiret " << m->name << " med magi! Veien videre er klar.\n";
                p.gainXp(m->xpReward);
                currentRoom->monster = nullptr;
                return true;
            }
        }
        else if (combatInput == "angrip" || combatInput == "slå" || combatInput == "attack") {
            int caffeineMod = p.getEffectBonus(StatusType::caffeineRush) + p.getEffectBonus(StatusType::sugarCrash);
            int playerAttackRoll = rollDice(20);
            int attackBonus = p.playerLvl; //Usikker på om jeg vil gi såpass med bonus uten at spilleren har et magisk våpen
            int totalAttack = playerAttackRoll + attackBonus + caffeineMod;

            int targetAC = m->monsterAC;  

            std::cout << "Du triller en D20 for å treffe... du fikk " << playerAttackRoll << " (+ " << attackBonus << " bonus) = " << totalAttack << " mot AC " << targetAC << ".\n";

            if (totalAttack >= targetAC) {
                int bonusDamage = 0;
                // Treff! Beregner skade.
                if (playerAttackRoll == 20) {        //MÅ jo ha med crits!  .... som betyr at jeg også må ha med fumbles!
                    bonusDamage = rollDice(8);       //Er dette fair? Eller bør den revurderes?  D&D-regler sier man skal doble dmg ved crit...
                }

                int damageDealt = p.playerBaseDamage + bonusDamage + p.getEffectBonus(StatusType::caffeineRush);
                std::string usedWeaponName = "knyttnevene";
                std::string currentDamageType = "Bludgeoning";

                //Sjekker om spilleren har aktivert et våpen på forhånd
                if (p.equippedWeapon != nullptr) {
                    usedWeaponName = p.equippedWeapon->name;
                    currentDamageType = p.equippedWeapon->damageType;

                    int weaponDamage = 0;
                    for (int i = 0; i < p.equippedWeapon->damageDiceCount; ++i) {
                        weaponDamage += rollDice(p.equippedWeapon->damageDiceSides);
                    }

                    damageDealt += weaponDamage;
                    std::cout << "Du angriper med din " << usedWeaponName << "!\n";
                } else {
                    std::cout << "Du har ikke valgt et våpen, og slår løs med bare knyttnevene!\n";
                }
                m->hp -= damageDealt;
                if (playerAttackRoll == 20) {
                    std::cout << "CRITICAL HIT! Du gjorde " << damageDealt << " skade på " << m->name << ".\n";
                } else {
                    std::cout << "Du traff! Du gjorde " << damageDealt << " skade på " << m->name << ".\n";
                }
                if (m->hp <= 0) {
                    std::cout << "Du beseiret " << m->name << "! Veien videre er klar.\n";
                    p.gainXp(m->xpReward);  //Gir spilleren XP
                    currentRoom->monster = nullptr;
                    return true;
                }
            } else {
                //FUMBLES!
                if (playerAttackRoll == 1) {
                    std::cout << "FUMBLE! Du snubler i dine egne argumenter, mister balansen og skader deg selv!\n";
                    p.playerHP -= 2;  //Mister 2HP på en fumble - kan revideres
                    std::cout << "Du tok 2 skade. Din HP: " << p.playerHP << "/" << p.playerMaxHP << "\n";
                } else {
                    std::cout << "Bom! Angrepet ditt preller av eller blir blokkert!.\n";
                }
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

        //Monsterets tur
        if (m->hp > 0) {
            std::cout << m->name << " " << m->attackMessage << "\n";

            int monsterAttackRoll = rollDice(20);
            int monsterTotalAttack = monsterAttackRoll + m->level;  //Level bonus

            std::cout << m->name << " triller for å treffe deg... Det fikk " << monsterTotalAttack << " mot din AC " << p.playerAC << ".\n";

            if (monsterTotalAttack >= p.playerAC) {

                int monsterBonusDamage = 0;
                if (monsterAttackRoll == 20) {
                    monsterBonusDamage = rollDice(8);
                }
                int totalMonsterDamage = m->attackDamage + monsterBonusDamage;
                //Monsteret treffer
                p.playerHP -= totalMonsterDamage;
                if (monsterAttackRoll == 20) {
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