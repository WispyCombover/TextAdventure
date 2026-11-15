#include "WorldBuilder.h"
#include "Item.h"
#include "Monster.h"
#include "NPC.h"
#include "Player.h"
#include <sstream>
#include <iomanip>
#include <cstdint>

static std::string getMemoryAddress(const Room* roomPtr) {
    std::ostringstream oss;
    uintptr_t addr = reinterpret_cast<uintptr_t>(roomPtr);
    oss << "0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(sizeof(void*) * 2) << addr;
    return oss.str();
}

Room* buildWorld(std::vector<std::unique_ptr<Room>>& allRooms, const player& p) {
    //BYGG ROMMENE HER
    auto spawnRoom = std::make_unique<Room>(
        "Møterommet",
        "Møterommet er som møterom flest. Du ser et bord med åtte stoler rundt. I taket henger en prosjektor, og på veggen henger et lerret.\n På bordet står det en tom og godt brukt kaffekopp som noen har glemt igjen, og det ligger en gammel bic penn ved siden av.\n  I et hjørne står en gammel whiteboard-tavle med nesten uleselige skriblerier, og det kommer et svakt lyst gjennom de frostede glassveggene på hver side av døren som leder ut."
    );
    auto corridor = std::make_unique<Room>(
        "Korridoren",
        "En lang, smal korridor med falmede tapeter og knirkende gulvplanker. Det er dører inn til to andre møterom langs veggen: 'RUBBER DUCK SANCTUM' og 'SALA SEGFAULT'.\nKorridoren fortsetter mot VEST og mot ØST, og du kan også gå tilbake til møterommet du nettopp forlot."
    );
    auto rubberDuckSanctum = std::make_unique<Room>(
        "Rubber Duck Sanctum",
        "Du river opp døren og avbryter et intenst enmannsmøte. Lyset er dempet, og midt på konferanseborde står en diger, gul gummiand på en liten fløyelspute, belyst av en skrivebordslampe. Ved bordenden sitter en tydelig utslitt utvikler med bustete hår og stirrende blikk, som stopper brått midt i en setning og ser forfjamset på deg. Du kan gå ut igjen gjennom DØR."
    );
    auto salaSegfault = std::make_unique<Room>(
        "Sala Segfault",
        "I det du åpner døren, møtes du av et kaotisk syn. Møterommet ser ut til å ha krasjet midt i en kvartalspresentasjon. Stoler ligger veltet, dokumenter flyr rundt som konfetti, og prosjektoren på veggen viser en blendende blåskjerm. Midt i rommet svever en glitrende, ustabil anomali av rå minnedata og løse pekere som freser illevarslende. Utgangen er tilbake gjennom DØR."
    );
    auto corridorWest = std::make_unique<Room>(
        "Korridoren mot vest",
        "Du kommet til et hjørne som går videre mot NORD og inn i kontorlandskapet. Rett foran deg er der en dør. På veggen ved siden av døren sert du en kortleser med et kodepanel." //Trenger logikk for å håndtere lesing av nøkkelkort samt inntasting av kode
    );
    auto corridorEast = std::make_unique<Room>(
        "Korridoren mot øst",
        "Du kommer til et lite hjørne av korridoren. Det er stille her, men du kan høre lyden av dempet prat fra at av de nærmeste møterommene. \n Her ser du også en liten sittegruppe med noen stoler og et bord, og på et lite bord i hjørnet står en gammel kaffemaskin. \nDu kan gå tilbake mot VEST, eller gjennom en dør mot NORD." //Kaffemaskinen må kunne brukes til å fylle koppen med varm kaffe (Acid attack, AOE 1d4)
    );


    //HENT MINNEADRESSENE TIL ROMMENE DER DET ER AKTUELT
    std::string spawnAddr = getMemoryAddress(spawnRoom.get());
    std::string rubberDuckSanctumAddr = getMemoryAddress(rubberDuckSanctum.get());
    std::string salaSegfaultAddr = getMemoryAddress(salaSegfault.get());

    //UTGANGER HER
    //Spawn (møterommet)
    spawnRoom->exits["dør"] = {
        corridor.get(),       //destination
        true,                 //hasDoor
        false,                //isOpen
        false,                //isLocked
        "",                   //keyName
        "På døren har noen teipet fast et bilde av en flodhest og teksten: 'GOT MILK?'"
    };

    //Corridor
    corridor->exits["kaffekoppens krypt"] = {
        spawnRoom.get(),
        true,
        false,
        false,
        "",
        "Fra denne siden kan du se dørskiltet som viser navnet på møterommet samt romnummeret. Dette rommet heter 'Kaffekoppens Krypt' og har nummer: " + spawnAddr
    };
    corridor->exits["vest"] = {
        corridorWest.get(),
        false,
        true,
        false,
        "",
        "Når du ser mot vest kan du se døren inn til møterommet 'RUBBER DUCK SANCTUM', forbi døren fortsetter korridoren net til hjørnet med døren inn til REKVISITA. Veien fortsetter nordover derfra og inn i kontorlandskapet."
    };
    corridor->exits["øst"] = {
        corridorEast.get(),
        false,
        true,
        false,
        "",
        "Mot øst kan du se korridoren lede forbi møterommet 'SALA SEGFAULT' og ned til kaffekroken der også kopirommet ligger."
    };
    corridor->exits["rubber duck sanctum"] = {
        rubberDuckSanctum.get(),
        true,
        false,
        false,
        "",
        "Fra denne siden kan du se dørskiltet med navnet på møterommet: 'Rubber Duck Sanctum' samt romnummeret. Dette rommet har nummer: " + rubberDuckSanctumAddr
    };
    corridor->exits["sala segfault"] = {
        salaSegfault.get(),
        true,
        false,
        false,
        "",
        "Fra denne siden kan du se dørskiltet med navnet på møterommet: 'Sala Segfault' samt romnummeret. Dette rommet har nummer: " + salaSegfaultAddr
    };

    //Corridor-West
    corridorWest->exits["øst"] = {
        corridor.get(),
        false,
        true,
        false,
        "",
        "Du ser tilbake mot møterommet der det hele startet. Det virker så lenge siden nå."
    };

    //Corridor-East
    corridorEast->exits["vest"] = {
        corridor.get(),
        false,
        true,
        false,
        "",
        "Du ser tilbake ned korridoren mot vest. Der ligger møterommet 'Kaffekoppens Krypt', og veien videre mot rekvisitalageret"
    };

    rubberDuckSanctum->exits["dør"] = {
        corridor.get(),
        true,
        false,
        false,
        "",
        "Døren tilbake til den knirkende korridoren."
    };

    salaSegfault->exits["dør"] = {
        corridor.get(),
        true,
        false,
        false,
        "",
        "Døren ut igjen til korridoren. Det frister veldig å lukke den godt bak seg!"
    };

    // LEGG TIL ITEMS
    spawnRoom->items.push_back(getItem("kaffekopp"));
    spawnRoom->items.push_back(getItem("penn"));

    rubberDuckSanctum->items.push_back(getItem("liten gummiand"));

    salaSegfault->items.push_back(getItem("adgangskort"));  //Tror adgangskortene må gjøres spesifikke, men enn så lenge har vi ett!

    // RANDOM ENCOUNTERS
    monsterManual manual;
    corridorEast->monster = manual.getRandomMonster(1);  // Spawner et tilfeldig level 1 monster i østkorridoren

    // FAST ENCOUNTER
    salaSegfault->monster = manual.getMonsterBlueprint(2, "Rogue Pointer");



    // NPCer
    //openOfficeOne->npcs.push_back(createFirewallGuru());
    rubberDuckSanctum->npcs.push_back(createSlitenSenior(p));

    //SCENERY
    //Spawn
    spawnRoom->scenery["prosjektor"] = "En gammel Epson-prosjektor med en kjølevifte som høres ut som et jetfly på tomgang. Lampen lyser svakt rødt.";
    spawnRoom->scenery["lerret"] = "Et nedtrekkbart lerret med en svak kaffeflekk nederst i hjørnet. Akkurat nå vises bare en tom, hvit flate.";
    spawnRoom->scenery["whiteboard"] = "Tavlen er full av kaotiske piler, bokser merket 'MICROSERVICES?' og et utvisket forsøk på å tegne en katt.";
    spawnRoom->scenery["tavle"] = spawnRoom->scenery["whiteboard"];
    spawnRoom->scenery["bord"] = "Et langt konferansebord i falsk eikefiner. Det er klissete på ett sted.";
    
    //Korridoren ut fra møterommet (spawn)
    corridor->scenery["kaffekoppens krypt"] = "Døren til møterommet 'Kaffekoppens krypt' (" + spawnAddr + "). Det frostede glasset skjuler rommet der det hele startet, og ved siden av dørkarmen henger en gulnet lapp med teksten: 'Vennligst rydd ut kopper etter møtet - vi har ikke garbage collection her'.";
    corridor->scenery["rubber duck sanctum"] = "Døren inn til 'Rubber Duck Sanctum' (romnummer: " + rubberDuckSanctumAddr + "). På dørhåndtaket henger en liten lapp hvor det står: 'Ikke forstyrr - dyp feilsøking pågår'. Du syns du hører noen hviske forklaringer om pekere og minnelekkasjer innenfra.";
    corridor->scenery["sala segfault"] = "På skiltet står det 'Sala Segfault' (" + salaSegfaultAddr + "). Det ser ut som om noen har prøvd å sparke opp døren fra innsiden, og under romnummeret har noen skrevet 'core dumped' med sprittusj.";

    //Rubber Duck Sanctum
    rubberDuckSanctum->scenery["gummiand"] = "En 30 centimeter høy, knall gul gummiand. Den har et dømmende, men tålmodig uttrykk i sine malte øyne, som om den allerede vet at feilen ligger på linje 42.";
    rubberDuckSanctum->scenery["and"] = "En 30 centimeter høy, knall gul gummiand. Den har et dømmende, men tålmodig uttrykk i sine malte øyne, som om den allerede vet at feilen ligger på linje 42.";
    rubberDuckSanctum->scenery["utvikler"] = "En seniorutvikler i falmet hettegenser. Han holder et halvt oppspist knekkebrød med brunost i den ene hånden, og peker med den andre på en skjerm full av rød feilmeldingstekst. Han mumler irritert: 'Jeg var SÅ nær ved å forstå hvorfor pekeren var null...'.";
    rubberDuckSanctum->scenery["skjerm"] = "Skjermen viser et terminalvindu med en endeløs strøm av stack traces og en åpen fane på StackOverflow fra 2014.";
    rubberDuckSanctum->scenery["laptop"] = "Skjermen viser et terminalvindu med en endeløs strøm av stack traces og en åpen fane på StackOverflow fra 2014.";

    //Sala Segfault
    salaSegfault->scenery["blåskjerm"] = "Prosjektoren lyser opp hele rommet med en knallblå feilmelding: 'FATAL ERROR: Unhandled meeting exception at address " + salaSegfaultAddr + ". System halted.'";
    salaSegfault->scenery["prosjektor"] = "Prosjektoren lyser opp hele rommet med en knallblå feilmelding: 'FATAL ERROR: Unhandled meeting exception at address " + salaSegfaultAddr + ". System halted.'";
    salaSegfault->scenery["vegg"] = "Prosjektoren lyser opp hele rommet med en knallblå feilmelding: 'FATAL ERROR: Unhandled meeting exception at address " + salaSegfaultAddr + ". System halted.'";
    salaSegfault->scenery["dokumenter"] = "Utskrifter av arkitekturdiagrammer som gir absolutt ingen mening. Noen har skrevet 'HVEM SKREV DETTE?!' med rød tusj over hele forsiden.";
    salaSegfault->scenery["papirer"] = "Utskrifter av arkitekturdiagrammer som gir absolutt ingen mening. Noen har skrevet 'HVEM SKREV DETTE?!' med rød tusj over hele forsiden.";
    salaSegfault->scenery["stoler"] = "Kontorstoler som er veltet i panikk. Det ser ut som møtedeltakerne bokstavelig talt fordampet da krasjet inntraff.";

    //Korridoren mot vest
    corridorWest->scenery["kortleser"] = "En standard RFID-kortleser. Den blinker utålmodig med et lite rødt lys.";
    corridorWest->scenery["kodepanel"] = "Et slitt talltastatur fra 0 til 9. Tastene 1, 3, 5 og 7 ser merkbart mer slitte ut enn de andre.";
    corridorWest->scenery["dør"] = "På døren henger et skilt som sier 'REKVISITA'.";

    //Korridoren mot øst
    corridorEast->scenery["kaffemaskin"] = "En industriell trakter som har sett bedre dager. Den putrer illevarslende, og kannen er full av beksvart væske.";
    corridorEast->scenery["dør"] = "På døren henger et skilt der det står 'KOPIROM'.";


    //PEKER TIL SPAWN
    Room* startingRoomPtr = spawnRoom.get();

    //LEGG ROMMENE I allRooms-vektoren
    allRooms.push_back(std::move(spawnRoom));
    allRooms.push_back(std::move(corridor));
    allRooms.push_back(std::move(rubberDuckSanctum));
    allRooms.push_back(std::move(salaSegfault));
    allRooms.push_back(std::move(corridorWest));
    allRooms.push_back(std::move(corridorEast));

    //SEND SPAWN TIL game.cpp
    return startingRoomPtr;
    
}