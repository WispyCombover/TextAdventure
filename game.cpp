#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <windows.h>
#include <random>
#include <initializer_list>
#include <fstream>

//TO-DO AND WISHLIST:
// -Damage type piercing/slashing/bludgeoning/acid/fire/...WIP
// -Potions (Monster(TM) i forskjellige farger)
// -Dialogsystem for NPCer (keep it simple)
// -Når vi har NPCer ... kan vi også ha merchants?
// -Romnumre til møterommene som minneadresser (tilgjengelige rom skal ha den faktiske minneadressen som rommet ligger i)
// -Nøkkelkort og adgangskoder
// -Puzzles! Klart vi må ha puzzles!
// -Datamaskiner som står på, på skjermen vises en oversikt over de faktiske filene under /user/.../documents på spillerens PC (kommer sikkerhet til å få hetta av denne?)
//....

//Definerer terningkast - D&D regler
int rollDice(int sides) {
    if (sides <= 0) return 0;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, sides);
    return dis(gen);
}

//Definerer spillobjekt (gjenstander i verdenen)
struct item {
    std::string name;
    std::string description;
    int value;
    std::string currency;
    std::string type;         //"Weapon", "Container", "Armour", "Potion", etc.
    int damageDiceSides = 0;  //4 for D4, 6 for D6, 8 for D8...
    int damageDiceCount = 0;  // Antall terninger å kaste
    std::string damageType;   // "Piercing", "Bludgeoning", "Acid", "Fire", etc.
    int extraDiceSides = 0;   // Dersom gjenstanden er modifisert for ekstra dmg
    int extraDiceCount = 0;
    std::string extraDamageType = "";
    bool isFilled = false;
    bool throwable = false;
};

//Definerer monstre
struct monster {   // To-do: terningkast for monsterangrep, To hit chance, XP, og kanskje initiativ?
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

//Definerer NPCer
struct NPC {   // To-do: dialoglogikk
    std::string name;
    std::string description;
};

// Definererr rom 
class Room {
    public:
        std::string name;
        std::string description;
        std::map<std::string, Room*> exits; //Peker til tilstøtende rom
        std::vector<item>items;
        std::unique_ptr<monster> monster; //NULLPTR for rom uten monster

        Room(std::string n, std::string d) : name(n), description(d) {}
        
        void addExit(const std::string& direction, Room* neighbor) {
            exits[direction] = neighbor;
        }
        void addExit(std::initializer_list<std::string> directions, Room* neighbor)  {
            for (const auto& direction : directions) {
                exits[direction] = neighbor;
            }
        }
};

//Definerer spilleren
struct player {
    std::string name;
    int playerLvl = 1;              // Modifiseres av XP, så vi trenger en XP-teller
    int playerXP = 0;               // Man starter selvsagt på LVL 1 - må lage en leveling tabell, og level-up mekanisme
    int playerHP = 20;              // Modifiseres av 'databrus' (healing) og andre gjenstander
    int playerMaxHP = 20;           // Modifiseres av level. Level up trigger HP-up via terningkast ... d10...tror jeg
    int playerBaseDamage = 2;       // Modifiseres av våpen og gjenstander
    int xpToNextLevel = 100;        // Hvor mye XP kreves for level 2 og videre
    int playerAC = 0;               // Påvirker to-hit-chance. AC = Armour Class
    std::shared_ptr<item> equippedWeapon = nullptr;   // nullptr = knyttnever

    Room* lastRoom = nullptr;       // Holder på det siste rommet spilleren var i

    //Funksjon for å håndtere level up
    void gainXp(int amount) {
        playerXP += amount;
        std::cout << "\n[XP] Du fikk " << amount << " XP!\n";

        while (playerXP >= xpToNextLevel) {
            playerXP -= xpToNextLevel;
            playerLvl++;
            xpToNextLevel = static_cast<int>(xpToNextLevel * 1.5); //Krever mer for neste level - må sikkert tweake balansen
            
            //Stats øker ved level up
            playerMaxHP += rollDice(10)+1;    //Dette bør vel strengt tatt følge D&D regler?
            playerHP = playerMaxHP;           //Full heal ved level up (eller er det for billig?)
            playerBaseDamage += 3;            //Virker greit med bare never, eller?
            playerAC += 1;                    //For lite?

            std::cout << "\n🎉 LEVEL UP! Du er nå på level " << playerLvl << "!\n";
            std::cout << "HP økte til " << playerMaxHP << ", og din base damage er nå " << playerBaseDamage << ".\n";
        }
    }
};

// KOmmenterer ut NPC-klassen under. Fikser denne senere!
//class NPC { //Database over NPCer - denne er i sin spede begynnelse, her trengs mer!
//    private:
//        std::map<int, std::vector<NPC>> blueprintDatabaseNPC;
//        
//    public:
//        NPC() {
//            blueprintDatabaseNPC[1].push_back({
//                "The Firewall Guru",
//                "En mystisk skikkelse med langt skjegg og mørke solbriller."});
//        }
//};

class monsterManual {  //Database over tilgjengelige monstre gruppert på level
    private:
        std::map<int, std::vector<monster>> blueprintDatabase;
        std::mt19937 rng; // D20?

    public:
        monsterManual() {
            //Starter RNG (Mersenne Twister)
            std::random_device rd;
            rng = std::mt19937(rd());

            //MONSTEROVERSIKT - "blueprints"
            //LEVEL 1
            blueprintDatabase[1].push_back({
                "Merge Conflict Wizard",
                "En skikkelse kledd i hettegenseren til servicedesk. Den mumler uforståelige git-kommandoer.",
                1, 10, 10, 4, 5, 1,         // Level, HP, Max HP, Max Damage, XP, AC  - Må ha logikk for terningkast når monstre angriper også.
                "Kaster en 'Git Push Force' som korrumperer filene dine!" });
            blueprintDatabase[1].push_back({
                "The Cookie Banner",
                "En vegg av uleselig juridisk tekst som blokkerer utsikten din. Den nekter å forsvinne.",
                1, 8, 8, 2, 7, 2,
                "Spammer deg med 'Godta alle' -knapper til du mister oversikten!"});
            blueprintDatabase[1].push_back({
                "The Missing Semicolon",
                "En mikroskopisk skapning som gjemmer seg i skyggene. Umulig å se med det blotte øyet.",
                1, 5, 5, 4, 10, 5,
                "Angriper i mørket, får deg til å snuble og kaster en 'Compilation Error' rett i ansiktet ditt!"
            });

            //LEVEL 2
            blueprintDatabase[2].push_back({
                "The Daily Standup Spectre",
                "Et gjenferd som tvinger deg til å stå oppreist mens det spør: 'Hva gjorde du i går?'.",
                2, 20, 20, 5, 15, 4,
                "Holder en endeløs monolog og påfører deg 'Feature Creep' -skade over tid!" //Poison damage. Enkelt å implementere?
            });
            blueprintDatabase[2].push_back({
                "The Jira Ticket Overlord",
                "Et monster laget av post-it-lapper og uleste varlser. Den flytter deg konstant mellom 'In Progress' og 'To Do'.",
                2, 25, 25, 4, 15, 6,
                "Oppretter tre nye sub-tasks som du MÅ løse med én gang!"
            });

            //LEVEL 3
            blueprintDatabase[3].push_back({
                "The DNS Daemon",
                "Ingen vet hva det er, eller hvor det kommer fra, men alt slutter å fungere når det våkner.",
                3, 40, 40, 8, 22, 6,
                "Endrer IP-adressen din i skjul, så angrepene dine treffer deg selv i stedet!" //Hva er beste mottiltak her? Physical attacks, eller "magic"?
            });
            blueprintDatabase[3].push_back({
                "The COBOL Leviathan",
                "Et eldgammelt monster dekket av hullkort og støv. Ingen nålevende personer forstår hva det er.",
                3, 50, 50, 6, 35, 7, 
                "Brøler et uforståelig feilsøkings-dump fra 1983 og lammer deg med nostalgi!" //Hmm, feature-creep. Saving-throw logikk? Rundebasert petrification?
            });

            //LEVEL 4
            blueprintDatabase[4].push_back({
                "Bobby Tables (The Dropper of Databases)",
                "En beryktet skikkelse som bærer på et skilt hvor det står: '; DROP TABLE Monsters;--'.",
                4, 75, 75, 12, 50, 8,
                "Kaster en SQL-injection som sletter vitale deler av dine essensielle funksjoner!" //Too much? Mottiltak? Beskyttelse?
            });

            //LEVEL 5
            blueprintDatabase[5].push_back({
                "The Deadline Monster",
                "En kolossal, tikkende skygge som blir større og rødere for hvert sekund som går. Den har en hær av uleste e-poster og kaffekopper rundt seg.",
                5, 200, 200, 25, 225, 10,
                "Kaster 'ASAP-panikk' over deg, spiser opp nattesøvnen din, og tvinger deg til å rulle ut uferdig kode i produksjon!" //Dette er mer potensiell skade enn spilleren har fra LVL1, må ha LEVEL-UP mekanikk.
            });
        }

        //Henter spesifikt monster basert på navn og level (manuell plassering)
        std::unique_ptr<monster> getMonsterBlueprint(int level, const std::string& name) {
            if (blueprintDatabase.find(level) !=blueprintDatabase.end()) {
                for (const auto& m : blueprintDatabase[level]) {
                    if (m.name == name) {
                        return std::make_unique<monster>(m);
                    }
                }
            }
            return nullptr;
        }

        //Tilfeldig monster for random encounters
        std::unique_ptr<monster> getRandomMonster(int level) {
            if (blueprintDatabase.find(level) != blueprintDatabase.end() && !blueprintDatabase[level].empty()) {
                std::uniform_int_distribution<size_t> dist(0, blueprintDatabase[level].size() - 1);
                size_t randomIndex = dist(rng);
                // Spawner et monster i en unique_ptr
                return std::make_unique<monster>(blueprintDatabase[level][randomIndex]);
            }
            return nullptr;
        }
};

void handleCombat(Room* currentRoom, Room* spawnRoom, player& p, const std::vector<std::shared_ptr<item>>& inventory, bool& running) {
    monster* m = currentRoom->monster.get();
    std::cout << "\n!!! SKREKK OG GRU !!!\n";
    std::cout << "Du møter på en " <<m->name <<"!\n";
    std::cout << m->description <<" (HP: " << m->hp << "/" << m->maxHp << ")\n";
    std::cout << "---------------------------------------\n";

    while (m->hp > 0 && p.playerHP > 0) {
        std::cout << "Hva gjør du? [angrip / flykt]\n> ";
        std::string combatInput;
        std::getline(std::cin, combatInput);

        if (combatInput == "angrip" || combatInput == "slå" || combatInput == "attack") {
            int playerAttackRoll = rollDice(20);
            int attackBonus = p.playerLvl; //Usikker på om jeg vil gi såpass med bonus uten at spilleren har et magisk våpen
            int totalAttack = playerAttackRoll + attackBonus;

            int targetAC = m->monsterAC + m->level;  //Kanskje monstrene blir for tøffe med +lvl modifier?

            std::cout << "Du triller en D20 for å treffe... du fikk " << playerAttackRoll << " (+ " attackBonus << " bonus) = " << totalAttack << " mot AC " << targetAC << ".\n";

            if (totalAttack >= targetAC) {
                int bonusDamage = 0;
                // Treff! Beregner skade.
                if (playerAttackRoll == 20) {        //MÅ jo ha med crits!  .... som betyr at jeg også må ha med fumbles!
                    bonusDamage = rollDice(8);       //Er dette fair? Eller bør den revurderes?
                }

                int damageDealt = p.playerBaseDamage + bonusDamage;
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
                    return;
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
            std::cout << "Du får panikk, snur på hælen og løper tilbake til start!\n"; // Trenger lastRoom logikk
            currentRoom = p.lastRoom;
            return;
        }
        else {
            std::cout << "Du nøler, og det koster deg dyrt...\n";
        }

        //Monsterets tur
        if (m->hp > 0) {
            std::cout << "\n" << m->name << " " << m->attackMessage << "\n";

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
                    running = false;
                    return;
                }      
            } else {
                std::cout << " Flaks! Du dukker unna angrepet til " << m->name << ".\n";
            }
        }
    }
}

void saveGame(const player& p, const Room* currentRoom) {
    std::ofstream saveFile("savefile.txt");
    if (saveFile.is_open()) {
        saveFile << p.playerLvl << "\n";
        saveFile << p.playerXP << "\n";
        saveFile << p.playerHP << "\n";
        saveFile << p.playerMaxHP << "\n";
        saveFile << p.playerBaseDamage << "\n";
        saveFile << p.playerAC << "\n";
        saveFile << currentRoom->name << "\n";

        saveFile.close();
        std::cout << " Spillet ble lagret!\n";
    } else {
        std::cout << "Feil: Kunne ikke opprette lagringsfil.\n";
    }
}

bool loadGame(player& p, Room*& currentRoom, const std::vector<std::unique_ptr<Room>>& allRooms) {
    std::ifstream loadFile("savefile.txt");
    if (!loadFile.is_open()) {
        return false;  //Finner ingen lagringsfil
    }

    std::string roomName;
    loadFile >> p.playerLvl;
    loadFile >> p.playerXP;
    loadFile >> p.playerHP;
    loadFile >> p.playerMaxHP;
    loadFile >> p.playerBaseDamage;
    loadFile >> p.playerAC;
    loadFile.ignore();
    std::getline(loadFile, roomName);
    //Finn riktig rom
    for (const auto& room : allRooms) {
        if (room->name == roomName) {
            currentRoom = room.get();
            break;
        }
    }
    loadFile.close();
    return true;
}

int main() {
    SetConsoleOutputCP(CP_UTF8); // Sørger for å støtte norske tegn
    SetConsoleCP(CP_UTF8);       // Sørger for å støtte norske tegn
    std::vector<std::unique_ptr<Room>> allRooms;
    player p;

    //1. VERDENSBYGGING (DM-ing)
    //Prøver å bruke unique_ptr for å håndtere minne automatisk - manuell minnehåndtering er fremdeles innmari skummelt!
    auto spawnRoom = std::make_unique<Room>(
        "Møterommet",
        "Du står alene på møterommet. De andre gikk for noen minutter siden, men du ble stående igjen. Det lukter stramt av gammel svette og kald kaffe.\nEt svakt lyst kommer gjennom de frostede vinduene ut i korridoren utenfor."     
    );

    auto corridor = std::make_unique<Room>(
        "Korridoren",
        "En lang, smal korridor med falmede tapeter og knirkende gulvplanker. Det er flere dører til tilstøtende møterom langs veggene, men lyser rødt for å indikere at de er opptatt.\nKorridoren fortsetter mot VEST og mot ØST, og du kan også gå tilbake gjennom DØR til møterommet du nettopp forlot."
    );

    auto corridorWest = std::make_unique<Room>(
        "Korridoren mot vest",
        "Du kommet til et hjørne som går videre mot NORD og inn i kontorlandskapet. Rett foran deg er der en dør. På veggen ved siden av døren sert du en kortleser med et kodepanel." //Trenger logikk for å håndtere lesing av nøkkelkort samt inntasting av kode
    );

    auto corridorEast = std::make_unique<Room>(
        "Korridoren mot øst",
        "Du kommer til et lite hjørne av korridoren. Det er stille her, men du kan høre lyden av dempet prat fra at av de nærmeste møterommene. \n Her ser du også en liten sittegruppe med noen stoler og et bord, og på et lite bord i hjørnet står en gammel kaffemaskin. \nDu kan gå tilbake mot VEST, eller gjennom en dør mot NORD." //Kaffemaskinen må kunne brukes til å fylle koppen med varm kaffe (Acid attack, AOE 1d4)
    );

    //Koble rommene sammen (retning, peker)
    spawnRoom->addExit({"DØR", "dør", "Dør"}, corridor.get());
    corridor->addExit({"DØR", "dør", "Dør"}, spawnRoom.get());
    corridor->addExit({"ØST", "øst", "EAST", "east", "E", "e", "Ø", "ø", "East", "Øst"}, corridorEast.get());
    corridor->addExit({"WEST", "VEST", "West", "Vest", "vest", "west", "W", "w", "V", "v"}, corridorWest.get());
    corridorEast->addExit({"VEST", "West", "Vest", "vest", "west", "W", "w", "V", "v"}, corridor.get());

    // Legg til en item
    spawnRoom->items.push_back(item{"kaffekopp", "En mye brukt kaffekopp som noen har glemt igjen. Innsiden er misfarget av gammel kaffe. Verdi: ", 2, "CP", "Weapon", 2, 1, "Bludgeoning", 0, 0, "", false});
    spawnRoom->items.push_back(item{"penn", "En vanlig BIC-penn som har tørket inn. Improvisert stikkvåpen (1d4 skade). Verdi: ", 1, "SP", "Weapon", 4, 1, "Piercing"});

    // Random encounters
    monsterManual manual;
    corridorEast->monster = manual.getRandomMonster(1);  // Spawner et tilfeldig level 1 monster i østkorridoren

    //Legger rommene inn i allRooms vektoren for lagring og minnehåndtering
    allRooms.push_back(std::move(spawnRoom));
    allRooms.push_back(std::move(corridor));
    allRooms.push_back(std::move(corridorWest));
    allRooms.push_back(std::move(corridorEast));

    Room* currentRoom = allRooms[0].get();  //Setter spawnRoom som ... vel ... spawn.
    std::vector<std:shared_ptr> inventory;

    //2. SPILLER-TILSTAND
    bool running = true;

    std::cout << "=== VELKOMMEN til MIMIRS HEMMELIGE DUNGEON CRAWLER ===\n\n";
    std::cout << "------------------------------------------------------\n\n";
    std::cout << "Det siste møtet kom med fryktelige avsløringer og overraskelser! \n";
    std::cout << "Deadlinemonsteret™ har stukket av med krypteringsnøklene, og Mimir har sendt deg ut for å hente dem tilbake. \n";
    std::cout << "Du må navigere gjennom kontorlandskapet, samle gjenstander og bekjempe monstre for å finne nøklene og redde Mimir fra en sikker død og evig segfault.\n";
    std::cout << "Du slenger din trofaste laptop-sekk på ryggen, og er klar for å ta til på eventyret! \n";
    std::cout << "Lykke til, vår alles fremtid ahenger av deg og ditt heltemot!\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "Skriv 'hjelp' for å se tilgjengelige kommandoer.\n\n";  //Hjelpemenyen må lages

    //3. GAME-LOOP
    while (running) {
        std::cout << "\n---------------------------------------\n";
        std::cout << "[" << currentRoom->name << "]\n";
        std::cout << currentRoom->description << "\n";
    
    if (!currentRoom->items.empty()) {
        std::cout << "Du ser følgende gjenstander her:\n";
        for (const auto& item : currentRoom->items) {
            std::cout << "- " << item.name << "\n";
        }
    }

    // Vent på kommando fra spilleren
    std::cout << "\nHva vil du gjøre? > ";
    std::string input;
    std::getline(std::cin, input);

    //Kommandotolk (parser)
    if (input == "avslutt" || input == "exit" || input == "quit") {
        running = false;
        std::cout << "Spillet avsluttes... Mimir returnerer til normal drift.\n";
    }
    else if (input.rfind("gå ", 0) == 0) { //Sjekker om kommandoen starter med "gå"
        //Finner første mellomrom etter "gå " for å hente retningen
        size_t spacePos = input.find(' ');
        std::string direction = input.substr(spacePos + 1); //Henter alt etter mellomrommet

        if (currentRoom->exits.find(direction) != currentRoom->exits.end()) {
            p.lastRoom = currentRoom;
            currentRoom = currentRoom->exits[direction];
        } else {
            std::cout << "Du kan ikke gå den veien.\nTilgjengelige utveier er: ";
            for (const auto& exit : currentRoom->exits) {
                std::cout << "[" << exit.first << "] ";
            }
            std::cout << "\n";
        }
    }
    else if (input.rfind("ta ", 0) == 0) {
        size_t spacePos = input.find(' ');
        std::string itemName = input.substr(spacePos +1); //Henter alt etter mellomrommet

        auto it = std::find_if(currentRoom->items.begin(), currentRoom->items.end(), [&itemName](const item& item) {return item.name == itemName;});
    
        if (it != currentRoom->items.end()) {
            inventory.push_back(std::make_shared<item>(*it));
            std::cout << "Du plukket opp: " << it->name << "\n";
            currentRoom->items.erase(it); //Fjerner gjenstanden fra rommet
        } else {
            std::cout << "Den gjenstanden finnes ikke her.\n";
        }
    }
    else if (input == "inventory" || input == "inv" || input == "i" || input == "inventar" || input == "se sekk") {
        if (inventory.empty()) {
            std::cout << "Sekken din er tom.\n";
        } else {
            if (p.equippedWeapon != nullptr) {
                std::cout << "[ Aktivt våpen: " << p.equippedWeapon->name << " ]\n";
            }
            std::cout << "I sekken din har du:\n";
            for (const auto& itemPtr : inventory) {
                std::cout << " - " << itemPtr->name << " (" << itemPtr->description << ")\n";
            }
        }
    }
    else if (input == "lagre" || input == "save") {
        saveGame(p, currentRoom);
    }
    else if (input == "load" || input == "last") {
        if (loadGame(p, currentRoom, allRooms)) {
            std::cout << "Lagringsfil lastet inn! Du er i " << currentRoom->name << ".\n";            
        } else {
            std::cout << "Ingen lagringsfil ble funnet.\n";
        }
    }
    else if (input == "fyll kopp" || input == "bruk kaffemaskin") {
        if (currentRoom->name != "Korridoren mot øst") {
            std::cout << "Det er ingen kaffemaskin her.\n";
        } else {
            //Sjekker om spilleren har en kaffekopp i inventaret
            auto it = std::find_if(inventory.begin(), inventory.end(), [](const std::shared_ptr<item>& i) {
                return i->name == "kaffekopp";
            });

            if (it != inventory.end()) {
                if ((*it)->isFilled) {
                    std::cout << "Koppen er allerede full av rykende fersk kaffe!\n";
                } else {
                    (*it)->isFilled = true;
                    (*it)->description = "En kaffekopp fylt med rykende fersk, varm og etsende trakterkaffe! (1d2 bludgeoning + 1d4 acid).\n";
                    (*it)->extraDiceSides = 4;
                    (*it)->extraDiceCount = 1;
                    (*it)->extraDamageType = "Acid";

                    std::cout << "*BRRrrrrrzzzzzt* Du fyller kaffekoppen med beksvart, rykende varm  kontorkaffe! Den lukter etsende, men også oppfriskende.\n";
                }
            } else {
                std::cout << "Du har ingen kaffekopp å fylle. Kanskje du finner en på et møterom eller noe?\n";
            }
        }
    }
    //BRUK VÅPEN
    else if (input.rfind("bruk", 0) == 0) {
        size_t spacePos = input.find(' ');
        std::string weaponName = input.substr(spacePos +1);

        //Se om spilleren har våpenet i inventaret
        auto it = std::find_if(inventory.begin(), inventory.end(), [&weaponName](const std::shared_ptr<item>& i) {
            return i.name == weaponName && i.type == "weapon";
        });

        if (it != inventory.end()) {
            p.equippedWeapon = *it;
            std::cout << "Du bruker nå: " << p.equippedWeapon->name << "!\n";
            std::cout << "Beskrivelse: " << p.equippedWeapon->description << "\n";
        } else {
            std::cout << "Du har ingen '" << weaponName << "' som kan brukes som våpen i inventaret ditt.\n";
        }
        }
    }
    else {
        std::cout << "Ukjent kommando. Prøv 'gå [retning]', 'ta [gjenstand]', 'inventory', eller 'avslutt'.\n";
    }
}
    return 0;
}