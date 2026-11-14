#include "WorldBuilder.h"
#include "Item.h"
#include "Monster.h"
#include "NPC.h"

Room* buildWorld(std::vector<std::unique_ptr<Room>>& allRooms) {
    //BYGG ROMMENE HER
    auto spawnRoom = std::make_unique<Room>(
        "Møterommet",
        "Møterommet er som møterom flest. Du ser et bord med åtte stoler rundt. I taket henger en prosjektor, og på veggen henger et lerret.\n På bordet står det en tom og godt brukt kaffekopp som noen har glemt igjen, og det ligger en gammel bic penn ved siden av.\n  I et hjørne står en gammel whiteboard-tavle med nesten uleselige skriblerier, og det kommer et svakt lyst gjennom de frostede glassveggene på hver side av døren som leder ut."
    );
    auto corridor = std::make_unique<Room>(
        "Korridoren",
        "En lang, smal korridor med falmede tapeter og knirkende gulvplanker. Det er flere dører til tilstøtende møterom langs veggene. På dørene til møterommene står der romnumre som.\nKorridoren fortsetter mot VEST og mot ØST, og du kan også gå tilbake gjennom DØR til møterommet du nettopp forlot."
    );
    auto corridorWest = std::make_unique<Room>(
        "Korridoren mot vest",
        "Du kommet til et hjørne som går videre mot NORD og inn i kontorlandskapet. Rett foran deg er der en dør. På veggen ved siden av døren sert du en kortleser med et kodepanel." //Trenger logikk for å håndtere lesing av nøkkelkort samt inntasting av kode
    );
    auto corridorEast = std::make_unique<Room>(
        "Korridoren mot øst",
        "Du kommer til et lite hjørne av korridoren. Det er stille her, men du kan høre lyden av dempet prat fra at av de nærmeste møterommene. \n Her ser du også en liten sittegruppe med noen stoler og et bord, og på et lite bord i hjørnet står en gammel kaffemaskin. \nDu kan gå tilbake mot VEST, eller gjennom en dør mot NORD." //Kaffemaskinen må kunne brukes til å fylle koppen med varm kaffe (Acid attack, AOE 1d4)
    );

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
    corridor->exits["door"] = {
        spawnRoom.get(),
        true,
        false,
        false,
        "",
        "Fra denne siden kan du se dørskiltet som viser navnet på møterommet samt romnummeret. Dette rommet heter 'Kaffekoppens Krypt' og har nummer: 0xC0FFEE00"  //Må få inn minneadressen til rommet - tror det blir kult
    };
    corridor->exits["vest"] = {
        corridorWest.get(),
        false,
        true,
        false,
        "",
        "Når du ser mot vest kan du se hjørnet med døren inn til REKVISITA. Veien fortsetter nordover derfra og inn i kontorlandskapet."
    };
    corridor->exits["øst"] = {
        corridorEast.get(),
        false,
        true,
        false,
        "",
        "Mot øst kan du se korridoren lede forbi mange lukkede og opptatte møterom og ned til kaffekroken der også kopirommet ligger."
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

    // LEGG TIL ITEMS
    spawnRoom->items.push_back(getItem("kaffekopp"));
    spawnRoom->items.push_back(getItem("penn"));

    // RANDOM ENCOUNTERS
    monsterManual manual;
    corridorEast->monster = manual.getRandomMonster(1);  // Spawner et tilfeldig level 1 monster i østkorridoren

    // NPCer
    //openOfficeOne->npcs.push_back(createFirewallGuru());

    //SCENERY
    //Spawn
    spawnRoom->scenery["prosjektor"] = "En gammel Epson-prosjektor med en kjølevifte som høres ut som et jetfly på tomgang. Lampen lyser svakt rødt.";
    spawnRoom->scenery["lerret"] = "Et nedtrekkbart lerret med en svak kaffeflekk nederst i hjørnet. Akkurat nå vises bare en tom, hvit flate.";
    spawnRoom->scenery["whiteboard"] = "Tavlen er full av kaotiske piler, bokser merket 'MICROSERVICES?' og et utvisket forsøk på å tegne en katt.";
    spawnRoom->scenery["tavle"] = spawnRoom->scenery["whiteboard"];
    spawnRoom->scenery["bord"] = "Et langt konferansebord i falsk eikefiner. Det er klissete på ett sted.";
    
    //Korridoren ut fra møterommet (spawn)
    corridor->scenery["dør"] = "Du lukket som vanlig døren bak deg. På døren henger et skilt som viser navnet på møterommet samt romnummeret. Dette møterommet heter 'Kaffekoppens krypt' og har romnummer: 0xC0FFEE00";  //Her må inn minneadressen til rommet

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
    allRooms.push_back(std::move(corridorWest));
    allRooms.push_back(std::move(corridorEast));

    //SEND SPAWN TIL game.cpp
    return startingRoomPtr;
    
}