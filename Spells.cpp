#include "Spells.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>

constexpr int kKillNineMaxHpPercent = 35;

namespace {
    std::string trim(const std::string& s) {
        const char* ws = " \t\r\n";
        size_t start = s.find_first_not_of(ws);
        if (start == std::string::npos) return "";
        size_t end = s.find_last_not_of(ws);
        return s.substr(start, end - start + 1);
    }

    int rollDamage(int count, int sides) {
        int total = 0;
        for (int i = 0; i < count; ++i) total += rollDice(sides);
        return total;
    }

const std::vector<spell> spellDatabase = {
    //Rekkefølgen på feltene MÅ følge rekkefølgen i struct spell

    // ====== LEVEL 1 ======
    {
        .name = "ping",
        .description = "Sender et lite signal ut i mørket og venter på svar. Er det noen der? Det er det ofte!",
        .level = 1,
        .manaCost = 1,
        .special = spellSpecial::Ping,
        .castMessage = "Du sender ut en magisk pakke og venter...",
        .fizzleMessage = "Request timed out. (Du har ikke nok mana.)"
    },
    {
        .name = "grep",
        .description = "Søker gjennom rommet etter alt som er verdt å finne. Støtter ikke regex, men gjør sitt beste.",
        .level = 1,
        .manaCost = 1,
        .special = spellSpecial::Grep,
        .castMessage = "Du mumler 'grep -r .' og rommet gløder svakt av søketreff...",
        .fizzleMessage = "grep: Ingen mana igjen. Prøv med sudo? Nei, det hjelper ikke!"
    },
    {
        .name = "ls -la",
        .description = "Lister alt, også det skjulte. Avslører monsterets eierskap, rettigheter og dårlige vaner.",
        .level = 1,
        .manaCost = 1,
        .needsTarget = true,
        .special = spellSpecial::Identify,
        .castMessage = "Du skriver 'ls -la' i lufta. Monsterets stats flimrer forbi som en filliste:",
        .fizzleMessage = "ls: cannot access: Mana is out of range."
    },
    {
        .name = "git blame",
        .description = "Avslører hvem som egentlig er skyld i uføret. Monstre som er skyld i mye, mister fatning og forsvar.",
        .level = 1,
        .manaCost = 2,
        .acReduction = 2,
        .needsTarget = true,
        .castMessage = "Du peker på monsteret og roper: 'Det var DU som committet dette på en fredag!' Skammen svekker forsvaret.",
        .fizzleMessage = "Du husker ikke hvem som skrev det. Det var antagelig deg. (Ikke nok mana.)"
    },

    // ====== LEVEL 2 ======
    {
        .name = "git stash",
        .description = "Gjemmer unna det som gjr vondt og lover å komme tilbake til det senere. Det gjør du aldri.",
        .level = 2,
        .manaCost = 3,
        .healDiceCount = 2,
        .healDiceSides = 6,
        .castMessage = "Du stasher skadene dine. De ligger der, glemt, i stash@{0}.",
        .fizzleMessage = "No local changes to save. (Og ingen mana.)"
    },
    {
        .name = "sudo",
        .description = "Det mektigste av alle småord. Neste trylleformel koster ingenting og kan ikke gå galt, så lenge du vet hva du gjør.",
        .level = 2,
        .manaCost = 3,
        .special = spellSpecial::Sudo,
        .castMessage = "Du uttaler det magiske ordet. Virkeligheten nøler, og gir så etter: 'OK. Du er root. Ikke gjør noe dumt.'",
        .fizzleMessage = "Du er ikke i sudoers-filen. Denne hendelesen vil bli rapportert."
    },
    {
        .name = "iptables -j drop",
        .description = "Dropper all trafikk som ikke er eksplisitt tillatt. Virker også mot angrep, møteinnkallinger og invitasjoner til teambuilding.",
        .level = 2,
        .manaCost = 3,
        .statusType = StatusType::firewalled,
        .statusName = "Brannmur",
        .statusTurns = 4,
        .statusMagnitude = 3,
        .castMessage = "Du hvisker 'iptables -j DROP'. Luften rundt deg stivner til en usynlig brannmur av regler ingen forstår.",
        .fizzleMessage = "iptables: Permission denied (you must be root). (Ikke nok mana.)"
    },
    {
        .name = "nmap",
        .description = "Skanner nabolaget etter åpne porter, sovende prosesser og ting folk helst ikke ville at du skulle se. Helt lovlig, så lenge du spør pent.",
        .level = 2,
        .manaCost = 2,
        .special = spellSpecial::Nmap,
        .castMessage = "Du kjører 'nmap -sV' mot rommene rundt deg. Små lysprikker tegner et kart i lufta:",
        .fizzleMessage = "nmap: Du har ikke nok mana til å skanne. (Mana: 0)"
    },

    // ====== LEVEL 3 ======
    {
        .name = "chmod 777",
        .description = "Gir alle alt. Høres ut som en god idé helt til noen faktisk bruker tigangen.",
        .level = 3,
        .manaCost = 4,
        .damageDiceCount = 2,
        .damageDiceSides = 8,
        .targetDamageBoost = 2,
        .needsTarget = true,
        .castMessage = "Du gir alle rettigheter til alt. Det smeller i monsteret, men det ser seg rundt, smiler, og oppdager at det også har skrivetilgang.",
        .fizzleMessage = "chmod: changing persmission: Operation not permitted. (Ikke nok mana.)"
    },
    {
        .name = "ctrl+z",
        .description = "Angrer det siste som skjedde. Fungerer på sår, men dessverre aldri på ting du virkelig angrer.",
        .level = 3, 
        .manaCost = 4,
        .healDiceCount = 3,
        .healDiceSides = 6,
        .castMessage = "Du trykker Ctrl+Z. Tiden snurrer baklengs, og noe av skaden ble aldri til.",
        .fizzleMessage = "Nothing to undo. (Du har ikke mana nok til å angre.)"
    },
    {
        .name = "ssh",
        .description = "Logger deg trygt inn et annet sted, forutsatt at du har nøkkelen og ikke bryr deg om hvor.",
        .level = 3,
        .manaCost = 2,
        .special = spellSpecial::Escape,
        .castMessage = "Du skriver 'ssh trygt-sted' og forsvinner i en sky av pakker.",
        .fizzleMessage = "Permission denied (publickey). (Mana: 0)"
    },
    {
        .name = "kill -9",
        .description = "En brutal og endelig formel. Ber ikke om unnskyldning, venter ikke på opprydning, og sender ingen blomster.",
        .level = 3,
        .manaCost = 6,
        .needsTarget = true,
        .special = spellSpecial::KillNine,
        .castMessage = "Du hveser 'SIGKILL' mellom sammenbitte tenner.",
        .fizzleMessage = "kill: (PID): No such process. (Og ingen mana.)"
    },

    // ====== LEVEL 4 ======
    {
        .name = "rm -rf /",
        .description = "Den forbudte formelen. Hver lærling får høre advarselen. Hver lærling prøver likevel, én gang. --preserve-root beskytter nøkkelgjenstander og selve boken.",
        .level = 4,
        .manaCost = 10,
        .damageDiceCount = 6,
        .damageDiceSides = 10,
        .recoilDamage = 5,
        .needsTarget = true,
        .special = spellSpecial::RmRf,
        .castMessage = "Du skriver kommandoen og trykker Enter før samvittigheten rekker å gripe inn. Gulvet, veggene og sekken din begynner å forsvinne...",
        .fizzleMessage = "rm: it is dangerous to operate recursively on '/'. Heldigvis. Du har ikke nok mana.)"
    },
    {
        .name = "chroot",
        .description = "Sperrer monsteret inne i en katalog der det verken kan se, nå eller skade noe utenfor. Ingen har helt forstått hvordan man kommer ut igjen, heller ikke du.",
        .level = 4,
        .manaCost = 6,
        .statusType = StatusType::chrooted,
        .statusName = "Chroot-fengsel",
        .statusTurns = 2,
        .statusMagnitude = 1,
        .needsTarget = true,
        .castMessage = "Du skriver 'chroot /dev/null' og monsteret trekkes inn i en katalog uten utganger.",
        .fizzleMessage = "chroot: Operation not permitted. (Ikke nok mana.)"
    },
    {
        .name = "sudo make me a sandwich",
        .description = "Uten sudo svarer terminalen: 'Lag den selv.' Med sudo dukker det opp en sandwich fra intet. Ingen vet hvem som lager dem, eller hvor skinken kommer fra. Fungerer kun hvis du først har gjort deg til root.",
        .level = 4,
        .manaCost = 4,
        .healDiceCount = 4,
        .healDiceSides = 6,
        .special = spellSpecial::Sandwich,
        .castMessage = "Du hvisker: 'sudo make me a sandwich'.",
        .fizzleMessage = "make: *** No rule to make target 'sandwich'. (Ikke nok mana.)"
    },
    {
        .name = "locate",
        .description = "Slår opp i en søkeindeks som sist ble oppdatert i 2017. Finner stort sett alt som lå der for lenge siden.",
        .level = 4,
        .manaCost = 3,
        .special = spellSpecial::Locate,
        .castMessage = "Du skriver 'locate', og rommene rundt deg svarer fra en indeks som er sju år gammel:",
        .fizzleMessage = "locate: updatedb has not been run. (Ikke nok mana.)"
    },

    // ====== LEVEL 5 ======
    {
        .name = "git push --force",
        .description = "Overskriver historien og alle andres arbeid. Veldig effektivt. Veldig upopulært.",
        .level = 5,
        .manaCost = 12,
        .damageDiceCount = 8,
        .damageDiceSides = 6,
        .recoilDamage = 4,
        .needsTarget = true,
        .castMessage = "Du force-pusher rett til main. Monsteret får øyeblikkelig merge-konflikter i selve eksistensen. Teamit ditt hater deg.",
        .fizzleMessage = "! [rejected] main -> main (ikke nok mana)"
    },
    {
        .name = "git revert",
        .description = "Lager en ny commit som angrer en gammel. Fungerer på sår, men aldri på kollegaers kommentarer i code review.",
        .level = 5,
        .manaCost = 8,
        .healDiceCount = 5,
        .healDiceSides = 8,
        .castMessage = "Du kjører 'git revert HEAD'. Skadene blir en ny commit i historien, og en friskere versjon av deg blir sjekket ut.",
        .fizzleMessage = "error: could not revert. (Ikke nok mana.)"
    },
    {
        .name = "fork bomb",
        .description = ":(){ :|:& };: - Tretten tegn og et helt operativsystem på kne. Skaden kommer over tid, og stopper ikke bare fordi du ber pent.",
        .level = 5,
        .manaCost = 8,
        .needsTarget = true,
        .special = spellSpecial::ForkBomb,
        .castMessage = "Du mumler ':(){ :|:& };:' og prosessene begynner å formere seg i hjørnene av rommet.",
        .fizzleMessage = "bash: fork: retry: Resource temporarily unavailable. (Ikke nok mana.)"
    },
    {
        .name = "rubber duck debugging",
        .description = "Forklar problemet høyt for en gummiand, og noe skjer med alle som hører på. Krever, naturlig nok, en gummiand i sekken.",
        .level = 5,
        .manaCost = 5,
        .acReduction = 4,
        .targetDamageBoost = -3,
        .requiresItem = "liten gummiand",
        .needsTarget = true,
        .castMessage = "Du setter anda på bakken og forklarer monsteret koden linje for linje. Midt i en setning innser det at logikken ikke holder. Anda nikker anerkjennende.",
        .fizzleMessage = "Anda venter tålmodig. Du har ikke nok mana til å forklare det."
    },
};

const std::vector<std::string> kStarterSpells = { "ls -la" };

} //namespace

bool isSpellKnown(const player& p, const spell& s) {
    if (std::find(kStarterSpells.begin(), kStarterSpells.end(), s.name) != kStarterSpells.end()) return true;
    return p.knownSpells.count(s.name) > 0;
}

bool readSpellPage(player& p, std::vector<std::shared_ptr<item>>& inventory, const std::string& itemName) {
    const std::string wanted = toLowerNorwegian(trim(itemName));
    auto it = std::find_if(inventory.begin(), inventory.end(), [&](const std::shared_ptr<item>& i) {
        return toLowerNorwegian(i->name) == wanted;
    });
    if (it == inventory.end()) {
        std::cout << "Du har ingen '" << trim(itemName) << "' i sekken.\n";
        return false;
    }
    const item& spellbookPage = **it;
    if (spellbookPage.teachesSpell.empty()) {
        std::cout << "Du myser på " << spellbookPage.name << ", men finner ingenting du kan lære av den.\n";
        return false;
    }
    if (!hasSpellbook(inventory)) {
        std::cout << "Du har ingen bok å sette siden inn i.\n";
        return false;
    }
    const spell* s = findSpell(spellbookPage.teachesSpell);
    if (!s) return false;
    if (isSpellKnown(p, *s)) {
        std::cout << "Du kan allerede '" << s->name << "'. Siden er overflødig og tar bare opp plass i sekken.\n";
        return false;
    }

    p.knownSpells.insert(s->name);
    std::cout << "Du setter siden inn i boken. Skriften svømmer foran øynene dine, og blir sakte leselig: '" << s->name << "' (level " << s->level << ").\n";
    if (p.playerLvl < s->level) {
        std::cout << "Du skjønner ikke helt hvordan den virker ennå. Den krever level " << s->level << ".\n";
    }
    inventory.erase(it);    //Hmm, sletter dette siden dersom den ikke blir satt inn i boken også? Litt dumt, den bør jo være tradable.
    return true;
}

const std::vector<spell>& spellList() {
    return spellDatabase;
}

const spell* findSpell(const std::string& name) {
    std::string wanted = toLowerNorwegian(trim(name));
    for (const auto& s : spellDatabase) {
        if (s.name == wanted) return &s;
    }
    return nullptr;
}

bool hasSpellbook(const std::vector<std::shared_ptr<item>>& inventory) {
    return std::any_of(inventory.begin(), inventory.end(), [](const std::shared_ptr<item>& i) { return i->type == "spellbook"; });
}

void listSpells(const player& p, const std::vector<std::shared_ptr<item>>& inventory) {
    if (!hasSpellbook(inventory)) {
        std::cout << "Du har ingen trylleformelbok. Kanskje det finnes en et sted?\n";
        return;
    }
    std::cout << "\n--- Necronomicon Manualis --- (Mana: " << p.playerMana << "/" << p.playerMaxMana << ")\n";
    int known = 0, missing = 0;
    for (const auto& s : spellDatabase) {
        if (!isSpellKnown(p, s)) { ++missing; continue; }
        ++known;
        std::cout << "[LVL " << s.level << "] " << s.name << " (" << s.manaCost << " mana)";
        if (p.playerLvl < s.level) std::cout << "  -- LÅST: krever level " << s.level;
        std::cout << "\n    " << s.description << "\n";
    }
    if (known == 0) std::cout << "Alle sidene er tomme.\n";
    if (missing > 0) std::cout << "\n" << missing << " sider er tomme, eller limt sammen av kaffe. Løse sider må finnes og leses (les <side>).\n";
    std::cout << "\nBruk: cast <trylleformel>\n";
}

castResult castSpell(
    player& p,
    const std::string& spellName,
    monster* target,
    const Room* currentRoom,
    std::vector<std::shared_ptr<item>>& inventory)
{
    if (!hasSpellbook(inventory)) {
        std::cout << "Du står og gestikulerer, men uten trylleformelbok vet du ikke hva du skal si.\n";
        return castResult::NoSpellbook;
    }

    const spell* s = findSpell(spellName);
    if (!s || !isSpellKnown(p, *s)) {
        std::cout << "bash: " << trim(spellName) << ": command not found. (Skriv 'cast' for å se formlene dine.)\n";
        return castResult::NotFound;
    }

    if (p.playerLvl < s->level) {
        std::cout << "Boken åpner seg på feil side. '" << s->name << "' krever level " << s->level << ".\n";
        return castResult::LevelTooLow;
    }

    if (s->needsTarget && target == nullptr) {
        std::cout << "Det er ingenting å bruke '" << s->name << "' på her.\n";
        return castResult::NoTarget;
    }

    if (!s->requiresItem.empty()) {
        const std::string needed = toLowerNorwegian(s->requiresItem);
        bool has = std::any_of(inventory.begin(), inventory.end(), [&](const std::shared_ptr<item>& i) {
            return toLowerNorwegian(i->name) == needed;
        });
        if (!has) {
            std::cout << "Du trenger '" << s->requiresItem << "' for å bruke '" << s->name << "'.\n";
            return castResult::MissingItem;
        }
    }

    const bool sudoWasActive = p.sudoActive;
    const int cost = (sudoWasActive && s->special != spellSpecial::Sudo) ? 0 : s->manaCost;

    if (p.playerMana < cost) {
        std::cout << s->fizzleMessage << " (Mana: " << p.playerMana << "/" << p.playerMaxMana << ")\n";
        return castResult::NoMana;
    }
    p.playerMana -= cost;

    //Sudo aktiverer kun bufff
    if (s->special == spellSpecial::Sudo) {
        p.sudoActive = true;
        std::cout << s->castMessage << "\n";
        return castResult::Cast;
    }

    //Buffen brukes av denne spellen
    p.sudoActive = false;
    if (sudoWasActive) {
        std::cout << "[sudo] Ingen mana brukt, og ingenting kan gå galt.\n";
    }

    //Fumble - nat 1 på D20, men ikke med sudo
    if (!sudoWasActive && rollDice(20) == 1) {
        std::cout << "Segmentation faul (core dumped). Trylleformelen går av i hendene på deg!\n";
        p.playerHP -= 2;
        std::cout << "Du tok 2 skade. Din HP: " << p.playerHP << "/" << p.playerMaxHP << "\n";
        return (p.playerHP <= 0) ? castResult::PlayerDied : castResult::Fumbled;
    }

    std::cout << s->castMessage << "\n";

    // ------ Spesialeffekter ------
    switch (s->special) {
        case spellSpecial::Nmap: {
            bool foundAny = false;
            if (currentRoom) {
                for (const auto& [dir, ex] : currentRoom->exits) {
                    if (ex.destination && ex.destination->occupant) {
                        const monster& mon = *ex.destination->occupant;
                        std::cout << " [" << dir << "] " << mon.name << " | level " << mon.level << " | HP " << mon.hp << "/" << mon.maxHp << "\n";
                        foundAny = true;
                    }
                }
                for (const auto& [key, itemName] : currentRoom->hiddenItems) {
                    std::cout << " [skjult tjeneste] Noe lytter bak '" << key << "'.\n";
                    foundAny = true;
                }
            }
            if (!foundAny) std::cout << " Alle porter er lukket. Ingenting svarer.\n";
            return castResult::Cast;
        }

        case spellSpecial::Locate: {
            bool foundAny = false;
            if (currentRoom) {
                for (const auto& [dir, ex] : currentRoom->exits) {
                    if (!ex.destination || ex.destination->items.empty()) continue;
                    foundAny = true;
                    if (ex.destination->visited) {
                        std::cout << " [" << dir << "] ";
                        const auto& its = ex.destination->items;
                        for (size_t i = 0; i < its.size(); ++i) 
                            std::cout << its[i].name << (i + 1 < its.size() ? ", " : "");
                        std::cout << "\n";
                    } else {
                        std::cout << " [" << dir << "] " << ex.destination->items.size()
                            << " gjenstand(er) i et rom du ikke har besøkt.\n";
                    }
                }
            }
            if (!foundAny) std::cout << " locate: ingen treff i indeksen.\n";
            return castResult::Cast;
        }

        case spellSpecial::Sandwich:
            if (!sudoWasActive) {
                std::cout << "'Lag den selv,' svarer terminalen. Mana og tid er tapt.\n";
                return castResult::Cast;
            }
            std::cout << "'Okay.' Noe som ligner et ostesmørbrød materialiserer seg i hendene dine.\n";
            break;

        case spellSpecial::ForkBomb:
            if (currentRoom) currentRoom->addHazard("en forkbomb", 2, 6, "Lightning", 4);
            std::cout << "Prosessene formerer seg raskere enn noen rekker å telle. Rommet blir et stort prosess-tre.\n";
            return castResult::Cast;

        case spellSpecial::Escape:
            if (p.lastRoom == nullptr) {
                std::cout << "ssh: connect to host: No route to host. Det er ingen steder å logge inn.\n";
                return castResult::Cast;
            }
            return castResult::Escaped;
        
        case spellSpecial::Identify:
            std::cout << " " << target->name << " | level " << target->level << " | HP " << target->hp << "/" << target->maxHp << " | AC " << target->monsterAC << " | skade " << damageText(*target) << "\n";
            if (!target->weaknesses.empty()) {
                std::cout << " | Svakheter: ";
                for (size_t i = 0; i < target->weaknesses.size(); ++i) {
                    std::cout << target->weaknesses[i] << (i + 1 < target->weaknesses.size() ? ", " : "");
                }
                std::cout << "\n";
            }
            return castResult::Cast;
        
        case spellSpecial::Ping: {
            bool foundAny = false;
            if (currentRoom) {
                for (const auto& [dir, ex] : currentRoom->exits) {
                    if (ex.destination && ex.destination->occupant) {
                        std::cout << " 64 bytes fra noe stort og sint mot '" << dir << "'.\n";
                        foundAny = true;
                    }
                }
            }
            if (!foundAny) std::cout << " Request timed out. Ingen svar fra rommene rundt.\n";
                return castResult::Cast;
        }

        case spellSpecial::Grep: {
            bool foundAny = false;
            if (currentRoom) {
                for (const auto& it : currentRoom->items) {
                    std::cout << " [item] " << it.name << "\n";
                    foundAny = true;
                }
                for (const auto& [key, desc] : currentRoom->scenery) {
                    std::cout << " [scenery] " << key << "\n";
                    foundAny = true;
                }
            }
            if (!foundAny) std::cout << " Ingen treff.\n";
            return castResult::Cast;
        }

        case spellSpecial::KillNine:
            if (target->level >= 5) {
                std::cout << "Prosessen er allerede zombie. Den bryr seg ikke.\n";
            } else if (target->hp * 100 <= target->maxHp * kKillNineMaxHpPercent) {
                target->hp = 0;
                std::cout << target->name << " får ikke engang tid til å rope.\n";
            } else {
                std::cout << target->name << " er altfor sprek til å drepes med ett signal. SIGKILL ignorert. (Svekk den først.)\n";
            }
            return castResult::Cast;
        
        default:
            break;
    }

    // ------ Generelle effekter -------
    if (s->damageDiceCount > 0 && target) {
        int dmg = rollDamage(s->damageDiceCount, s->damageDiceSides);
        target->hp -= dmg;
        std::cout << "Trylleformelen gjør " << dmg << " skade på " << target->name << ".\n";
    }

    if (s->acReduction > 0 && target) {
        target->monsterAC -= s->acReduction;
        std::cout << target->name << " mister " << s->acReduction << " AC. (Ny AC: " << target->monsterAC << ")\n";
    }

    if (s->targetDamageBoost != 0 && target) {
        target->attackDamage += s->targetDamageBoost;
        if (s->targetDamageBoost > 0)
            std::cout << target->name << " slår nå " << s->targetDamageBoost << " hardere. Det der var kanskje ikke så lurt.\n";
        else
            std::cout << target->name << " slår nå " << -s->targetDamageBoost << " svakere. Anda nikker anerkjennende.\n";
    }

    if (s->statusTurns > 0) {
        p.addEffect(s->statusType, s->statusName, s->statusTurns, s->statusMagnitude);
        std::cout << "[Status] " << s->statusName << " aktiv i " << s->statusTurns << " runder.\n";
    }

    if (s->healDiceCount > 0) {
        int heal = rollDamage(s->healDiceCount, s->healDiceSides);
        p.playerHP = std::min(p.playerMaxHP, p.playerHP + heal);
        std::cout << "Du får tibake " << heal << " HP. Din HP: " << p.playerHP << "/" << p.playerMaxHP << "\n";
    }

    if (s->special == spellSpecial::RmRf) {
        // --preserve-root: quest-items og spellboken
        auto isDeletable = [](const std::shared_ptr<item>& i) {
            return !i->isQuestItem && i->type != "spellbook";
        };
        if (p.equippedWeapon && isDeletable(p.equippedWeapon)) {
            p.equippedWeapon = nullptr;
        }
        for (auto it = p.equippedArmour.begin(); it != p.equippedArmour.end(); ) {
            if (it->second && isDeletable(it->second)) it = p.equippedArmour.erase(it);
            else ++it;
        }
        std::erase_if(inventory, isDeletable);
        std::cout << "Sekken din er tom. Bare det viktigste overlevde (--preserve-root).\n";
    }

    if (s->recoilDamage > 0) {
        p.playerHP -= s->recoilDamage;
        std::cout << "Tilbakeslag! Du tar " << s->recoilDamage << " skade. Din HP: " << p.playerHP << "/" << p.playerMaxHP << "\n";
        if (p.playerHP <= 0) return castResult::PlayerDied;
    }

    return castResult::Cast;
}