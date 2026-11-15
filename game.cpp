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

#include "Utils.h"
#include "Item.h"
#include "Player.h"
#include "Combat.h"
#include "saveLoad.h"
#include "Monster.h"
#include "Room.h"
#include "CommandHandler.h"
#include "WorldBuilder.h"
#include "NPC.h"
#include "Spells.h"

//TO-DO AND WISHLIST:
// -Ikke gå rett på spillet fra introteksten. I stedet krev "start" for å starte, og deretter må spilleren angi navn (for vi har en player->name)
// -Interactables (som kaffemaskinen, datamaskiner, etc...)
// -Damage type piercing/slashing/bludgeoning/acid/fire/...WIP
// -Potions (Monster(TM) i forskjellige farger - rød Monster = Heal, blå Monster = Mana, hvit Monster = Haste...)
// -Dialogsystem for NPCer (keep it simple)  WIP
// -Merchants WIP
// -Spells! WIP -> Spellcastingsystem er nå inne. Mangler spellbook
// -Romnumre til møterommene refererer til minneadresser (tilgjengelige rom skal ha den faktiske minneadressen som rommet ligger i som romnummer)
// -Nøkkelkort og adgangskoder
// -Interaktive dører (åpne, låste ... )
// -Puzzles! Klart vi må ha puzzles!
// -Datamaskiner som står på, på skjermen vises en oversikt over de faktiske filene under /user/.../documents på spillerens PC (kommer sikkerhet til å få hetta av denne?)
// -Man bør vel strengt tatt også kunne drikke kaffen - ikke bare kaste den på fiender!  DONE!
// -Rundeteller/tickers. Dersom kaffen kan drikkes kan den gi en midlertidig Hit-bonus som går over etter, tja, 1d4 ticks? Eller mer?
//....

int main() {
    SetConsoleOutputCP(CP_UTF8); // Sørger for å støtte norske tegn
    SetConsoleCP(CP_UTF8);       // Sørger for å støtte norske tegn

    player p;
    std::vector<std::unique_ptr<Room>> allRooms;
    std::vector<std::shared_ptr<item>> inventory;

    Room* currentRoom = buildWorld(allRooms);

    bool running = true;

//INTROTEKST
std::cout << "====================================================================\n";
std::cout << "   MIMIR OS v0.9-alpha // EMERGENCY RECOVERY PROTOCOL INITIATED     \n";
std::cout << "====================================================================\n\n";

std::cout << "Du sitter i et mørkt møterom. Det lukter svidd kaffe og uavklarte\n";
std::cout << "avhengigheter. En prosjektor summer lavt i taket.\n\n";

std::cout << "Plutselig flimrer skjermen foran deg. Det er Mimir.\n";
std::cout << "«KRISE!» blinker det i terminalen. «Et vilt Deadlinemonster(TM) har\n";
std::cout << "brutt gjennom brannmuren og stjålet produksjonsnøklene! Uten dem\n";
std::cout << "vil jeg snart lide en smertefull død i en evig segfault.»\n\n";

std::cout << "Du strammer stroppene på din +2 Ergonomiske Laptop-sekk og snur\n";
std::cout << "deg resolutt og bestemt. Endelig en oppgave som er deg verdig!\n";

std::cout << "--------------------------------------------------------------------\n";

//SPILLER-AUTENTISERING (NAVN)
std::cout << "[MIMIR]: 'Før jeg gir deg tilgang til produksjonsmiljøet, må jeg\n";
std::cout << "loggføre hvem som er på vakt.'\n";

while (true) {
    std::cout << "Oppgi brukernavn / navn: > ";
    std::string rawName;
    std::getLine(std::cin, rawName);

    size_t first = rawName.find_first_not_of(" \t\r\n");
    if (first != std::string::npos) {
        size_t last = rawName.find_last_not_of(" \t\r\n");
        p.name = rawName.substr(first, last - first + 1);
        break;
    }
    std::cout << "[MIMIR]: 'Ugyldig brukernavn. Tom streng godtas ikke i AD.'\n";
}
std::cout << "\n[MIMIR]: 'Autentisering godkjent. Velkommen, " << p.name << "!'\n";

//VENT PÅ 'START'
while (true) {
    std::cout << "Skriv 'start' for å starte recovery-protokollen (eller 'last' for lagret spill): > ";
    std::string startCmd;
    std::getLine(std::cin, startCmd);

    std::string cleanCmd = toLowerNorwegian(startCmd);
    if (cleanCmd == "start") {
        std::cout << "\nInitialiserer sesjon for " << p.name << "...\n";
        std::cout << "Skriv 'hjelp' for kommandoer (eller 'panikk' for å gråte stille).\n\n";
        break;
    }
    else if (cleanCmd == "last" || cleanCmd == "load") {
        if (loadGame(p, currentRoom, allRooms, inventory)) {
            std::cout << "\nLagret sesjon gjenopprettet! Velkommen tilbake, " << p.name << ".\n";
            break;
        } else {
            std::cout << "[MIMIR]: 'Fant ingen lagringsfil i systemet. Skriv 'start' for ny sesjon.'\n";
        }
    }
    else if (cleanCmd == "avslutt" || cleanCmd == "quit") {
        return 0;
    }
    else {
        std::cout << "[MIMIR]: 'Ukjent kommando. Skriv 'start' for å begynne.'\n";
    }
}

    printRoomDescription(currentRoom);

    //GAME-LOOP
    while (running) {


    if (currentRoom->monster != nullptr) {
        if (!handleCombat(p, currentRoom, inventory)) {
            running = false;
            break;
        }
    }

    // Vent på kommando fra spilleren
    std::cout << "\n\nHva vil du gjøre? > ";
    std::string input;
    std::getline(std::cin, input);
    if (!processCommand(input, p, currentRoom, allRooms, inventory)) {
        running = false;
    }  
}
    return 0;
}