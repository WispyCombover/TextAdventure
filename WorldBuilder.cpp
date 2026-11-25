#include "WorldBuilder.h"
#include "Item.h"
#include "Monster.h"
#include "NPC.h"
#include "Player.h"
#include "Room.h"
#include <sstream>
#include <iomanip>
#include <cstdint>

static std::string getMemoryAddress(const Room* roomPtr) {
    std::ostringstream oss;
    uintptr_t addr = reinterpret_cast<uintptr_t>(roomPtr);
    oss << "0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(sizeof(void*) * 2) << addr;
    return oss.str();
}

static std::shared_ptr<Room::Door> makeDoor(const std::string& id, bool locked = false, const std::string& key = "") {
    auto d = std::make_shared<Room::Door>();
    d->id = id;
    d->isLocked = locked;
    d->keyName = key;
    return d;
}

Room* buildWorld(std::vector<std::unique_ptr<Room>>& allRooms, const player& p) {

    //ROMMENE BYGGES HER

    auto spawnRoom = std::make_unique<Room>(
        "Møterommet",
        "Møterommet er som møterom flest. Du ser et bord med åtte stoler rundt. I taket henger en prosjektor, og på veggen henger et lerret.\n På bordet står det en tom og godt brukt kaffekopp som noen har glemt igjen, og det ligger en gammel bic penn ved siden av.\n  I et hjørne står en gammel whiteboard-tavle med nesten uleselige skriblerier, og det kommer et svakt lyst gjennom de frostede glassveggene på hver side av døren som leder ut."
    );
    auto corridor = std::make_unique<Room>(
        "Korridoren",
        "En lang, smal korridor med falmede tapeter og knirkende gulvplanker som en herlig kontrast mot glassvinduer inn mot møterommene. Det er dører inn til to andre møterom langs veggen: 'RUBBER DUCK SANCTUM' og 'SALA SEGFAULT'.\nKorridoren fortsetter mot VEST og mot ØST, og du kan også gå tilbake til møterommet du nettopp forlot."
    );
    auto rubberDuckSanctum = std::make_unique<Room>(
        "Rubber Duck Sanctum",
        "Du river opp døren og avbryter et intenst enmannsmøte. Lyset er dempet, og midt på konferansebordet står en diger, gul gummiand på en liten fløyelspute, belyst av en skrivebordslampe. Ved bordenden sitter en tydelig utslitt utvikler med bustete hår og stirrende blikk, som stopper brått midt i en setning og ser forfjamset på deg. Du kan gå ut igjen gjennom DØR."
    );
    auto salaSegfault = std::make_unique<Room>(
        "Sala Segfault",
        "I det du åpner døren, møtes du av et kaotisk syn. Møterommet ser ut til å ha krasjet midt i en kvartalspresentasjon. Stoler ligger veltet, dokumenter flyr rundt som konfetti, og prosjektoren på veggen viser en blendende blåskjerm. Midt i rommet svever en glitrende, ustabil anomali av rå minnedata og løse pekere som freser illevarslende. Utgangen er tilbake gjennom DØR."
    );
    auto corridorWest = std::make_unique<Room>(
        "Korridoren mot vest",
        "Du kommet til et hjørne som går videre mot NORD og inn i kontorlandskapet. Rett foran deg er der en dør in til REKVISITA. På veggen ved siden av døren sert du en et kodepanel." //Trenger logikk for å håndtere lesing av nøkkelkort samt inntasting av kode
    );
    auto supplyRoom = std::make_unique<Room>(
        "Rekvisitarommet",
        "Et trangt, vindusløst rom stappet til randen med pappesker fulle av utdaterte VGA-kabler, post-it-blokker i neonfarger og tonerkassetter til en skriver som ble kastet i 2016. Det lukter svakt av støv og uåpnet plast. Utgangen er tilbake gjennom DØR."
    );
    auto openOfficeOne = std::make_unique<Room>(
        "Kontorlandskapet - gruppe 1",
        "Et åpent kontorlandskap med rekker av hev-senk-pulter og flimrende lysrør i taket. Luften er tung av konsentrasjon og lavmælt tasting. De fleste pultene er forlatt i hui og hast, med halvfulle kaffekopper og ulåste skjermer. Du kan gå tilbake mot SØR, eller videre mot NORD. Her ser du også Brannmur-Guruen sitte og stirre inn i skjermene sine."
    );
    auto openOfficeTwo = std::make_unique<Room>(
        "Kontorlandskapet - gruppe 2",
        "Du trår inn i den delen av kontorlandskapet der støyreduserende hodetelefoner er påbudt verneutstyr. Veggene er tapetsert med gamle sprint-tavler og falmede 'Keep Calm and git push --force'-plakater. En enslig vifte durer i hjørnet i et tappert, men forgjeves forsøk på å kjøle ned luften fra femten overarbeidede utvikler-laptoper. Du kan gå videre mot NORD, eller tilbake mot SØR."
    );

    auto northernCorridorOne = std::make_unique<Room>(
        "Den nordre korridoren - vest",
        "Du står i den nordvestre enden av hovedkorridoren. Gulvbelegget er slitt etter utallige sprint-retrospektiver og hastige turer til kaffemaskinen. Mot VEST er det en dør merket med et piktogram av en kjønnsnøytral strekfigur med en laptop. Du kan gå sørover inn i kontorlandskapet, eller følge korridoren mot ØST."
    );
    auto restroomsIT = std::make_unique<Room>(
        "Unisex restrooms for IT-avdelingen",
        "Overraskende rent, med dempet, behagelig belysning og en duft av billig håndsåpe og eksistensiell ro. På innsiden av båsdøren har noen risset inn '/* TODO: Vask hender */' med en nøkkel. Dette er det eneste stedet i bygget hvor ingen spør deg om et kjapt estimat. Utgangen er tilbake gjennom DØR."
    );
    auto northernCorridorMid = std::make_unique<Room>(
        "Den nordre korridoren - senter",
        "Midtpunktet i den nordre korridoren. Langs veggen ligger to møterom: 'THE HOTFIX HOTEL' og '503: SERVICE UNAVAILABLE'. Rett mot NORD finner du heisdørene, mens korridoren strekker seg videre mot VEST og ØST."
    );
    auto theHotfixHotel = std::make_unique<Room>(
        "The Hotfix Hotel",
        "Et kaotisk lite møterom som bærer preg av å ha blitt tatt i bruk på fem minutters varsel en fredag ettermiddag. Halvspiste energibarer ligger strødd rundt på bordet, og på veggen henger en lapp hvor det står: 'Midlertidig løsning implementert i 2021 – ikke rør!'. Utgangen er tilbake gjennom DØR."
    );
    auto serviceUnavailable = std::make_unique<Room>(
        "503: Service Unavailable",
        "Rommet er stappfullt av gamle serverrack, blinkende switcher og tykke bunter med patchekabler som henger som lianer fra taket. Det durer øredøvende fra kjøleviftene, og en rød varsellampe blinker taktfast i hjørnet. Et skilt på veggen sier: 'Per fra drift er på ferie. Prøv igjen senere.' Utgangen er tilbake gjennom DØR."
    );
    auto elevatorFromIT = std::make_unique<Room>(
        "Personheis",
        "En trang stålheis som lukter svakt av våt ull og kald kaffe. Panelknappene er slitte, og displayet over døren viser bare et kryptisk 'NaN' i stedet for etasjenummer. Du kan gå ut igjen mot SØR."
    );
    auto northernCorridorTwo = std::make_unique<Room>(
        "Den nordre korridoren - øst",
        "Den østlige enden av den nordre korridoren. Her er det litt stillere, og gjennom et lite vindu ser du ut mot en grå bakgård. Korridoren går tilbake mot VEST, eller du kan bevege deg sørover inn mot resten av kontorlandskapet."
    );
    auto openOfficeThree = std::make_unique<Room>(
        "Kontorlandskapet - gruppe 3",
        "Her sitter frontend-gjengen. Pultene er mistenkelig ryddige, og belysningen er justert til en behagelig, estetisk korrekt fargetemperatur. På en tavle henger det utallige fargeprøver og en opphetet diskusjon om padding på 4px versus 6px. Korridoren ligger mot VEST, mens landskapet fortsetter mot SØR."
    );
    auto openOfficeFour = std::make_unique<Room>(
        "Kontorlandskapet - gruppe 4",
        "Det innerste hjørnet av kontorlandskapet, preget av tomme pizzabokser og mekaniske tastaturer med altfor høy klikkelyd. Luften vibrerer svakt av koffein og eksistensiell tvil rundt teknisk gjeld. Du kan gå mot NORD til gruppe 3, eller mot ØST mot trappegangen."
    );
    auto stairwellFromIT = std::make_unique<Room>(
        "Trappegangen",
        "En betongtrapp med gjenklang og et ekkelt, kaldt trekk. På veggen henger et falmet branninstruks-skilt og en plakat som oppfordrer til å ta trappene for helsens skyld. Ingen tar trappene."
    );

    auto corridorEast = std::make_unique<Room>(
        "Korridoren mot øst",
        "Du kommer til et lite hjørne av korridoren. Du kan høre banking fra døren til kopirommet, og lyden av engasjert prat fra møterommet mot SØR. Møterommet heter: 'THE BIKESHED'. Her ser du også en liten sittegruppe med noen stoler og et bord, og på et lite bord i hjørnet står en gammel kaffemaskin. \nDu kan gå tilbake mot VEST, eller gjennom en dør mot NORD som har et skilt med 'KOPIROM'. Ved siden av døren til kopirommet henger en kortleser." //Man skal kunne snakke med personen som er innelåst på kopirommet. "snakk dør" eller noe slikt...
    );
    auto copyRoomOne = std::make_unique<Room>(
        "Kopirommet",
        "En klaustrofobisk boks dominert av en gigantisk multifunksjonsskriver som blinker illevarslende med 'PC LOAD LETTER'. Hyllene langs veggen er fulle av A4-ark og makuleringsposer. Utgangen er tilbake mot SØR."  //Her er også en innelåst NPC
    );
    auto theBikeshed = std::make_unique<Room>(
        "The Bikeshed",
        "Et lite møterom der en opphetet diskusjon pågår. På whiteboarden står det 'VALG AV FARGE PÅ LOGG-KNAPP' understreket tre ganger, omringet av komplekse arkitekturdiagrammer. Døren tilbake til korridoren er mot NORD."
    );


    //HENT MINNEADRESSENE TIL ROMMENE DER DET ER AKTUELT
    std::string spawnAddr = getMemoryAddress(spawnRoom.get());
    std::string rubberDuckSanctumAddr = getMemoryAddress(rubberDuckSanctum.get());
    std::string salaSegfaultAddr = getMemoryAddress(salaSegfault.get());
    std::string theBikeshedAddr = getMemoryAddress(theBikeshed.get());
    std::string hotfixHotelAddr = getMemoryAddress(theHotfixHotel.get());
    std::string serviceUnavailableAddr = getMemoryAddress(serviceUnavailable.get());

    //UTGANGER HER
    //DØRER (én per dør, delt mellom begge sider)
    auto doorSpawn   = makeDoor("spawn");
    auto doorDuck    = makeDoor("duck");
    auto doorSala    = makeDoor("sala");
    auto doorSupply  = makeDoor("supply", true, "1357");
    auto doorCopy    = makeDoor("copy", true, "adgangskort");
    auto doorBikeshed = makeDoor("bikeshed");
    auto doorRestroomIT = makeDoor("restroom", false, "");
    auto doorHotfix = makeDoor("hotfix");
    auto doorServiceUnavailable = makeDoor("service");
    auto doorElevator = makeDoor("lift");
    auto doorStairwell = makeDoor("stairs");

    //Spawn <-> Korridor
    spawnRoom->exits["dør"] = {
        corridor.get(), 
        doorSpawn,
        "På døren har noen teipet fast et bilde av en flodhest og teksten: 'GOT MILK?'"
    };
    corridor->exits["kaffekoppens krypt"] = {
        spawnRoom.get(), 
        doorSpawn,
        "Fra denne siden kan du se dørskiltet som viser navnet på møterommet samt romnummeret. Dette rommet heter 'Kaffekoppens Krypt' og har nummer: " + spawnAddr
    };

    //Korridor <-> Rubber Duck Sanctum
    corridor->exits["rubber duck sanctum"] = {
        rubberDuckSanctum.get(), 
        doorDuck,
        "Fra denne siden kan du se dørskiltet med navnet på møterommet: 'Rubber Duck Sanctum' samt romnummeret. Dette rommet har nummer: " + rubberDuckSanctumAddr
    };
    rubberDuckSanctum->exits["dør"] = {
        corridor.get(), 
        doorDuck,
        "Døren tilbake til den knirkende korridoren. På døren henger et midtsidebilde av en gummiand. Nederst på bildet ser du teksten 'Frøken Februar 2016'."
    };

    //Korridor <-> Sala Segfault
    corridor->exits["sala segfault"] = {
        salaSegfault.get(), 
        doorSala,
        "Fra denne siden kan du se dørskiltet med navnet på møterommet: 'Sala Segfault' samt romnummeret. Dette rommet har nummer: " + salaSegfaultAddr
    };
    salaSegfault->exits["dør"] = {
        corridor.get(), 
        doorSala,
        "Døren ut igjen til korridoren. Det frister veldig å lukke den godt bak seg!"
    };

    //Korridor <-> Korridor vest/øst (åpne passasjer, ingen dør)
    corridor->exits["vest"] = {
        corridorWest.get(), 
        nullptr,
        "Når du ser mot vest kan du se døren inn til møterommet 'RUBBER DUCK SANCTUM', forbi døren fortsetter korridoren net til hjørnet med døren inn til REKVISITA. Veien fortsetter nordover derfra og inn i kontorlandskapet."
    };
    corridor->exits["øst"] = {
        corridorEast.get(), 
        nullptr,
        "Mot øst kan du se korridoren lede forbi møterommet 'SALA SEGFAULT' og ned til kaffekroken der også kopirommet ligger."
    };
    corridorWest->exits["øst"] = {
        corridor.get(), 
        nullptr,
        "Du ser tilbake mot møterommet der det hele startet. Det virker så lenge siden nå."
    };
    corridorEast->exits["vest"] = {
        corridor.get(), 
        nullptr,
        "Du ser tilbake ned korridoren mot vest. Der ligger møterommet 'Kaffekoppens Krypt', og veien videre mot rekvisitalageret"
    };

    //Korridor vest <-> Rekvisita (kodepanel)
    corridorWest->exits["rekvisita"] = { 
        supplyRoom.get(), 
        doorSupply, 
        "" 
    };
    supplyRoom->exits["dør"] = { 
        corridorWest.get(), 
        doorSupply, 
        "" 
    };

    //Korridor vest <-> Kontorlandskap
    corridorWest->exits["nord"] = { 
        openOfficeOne.get(), 
        nullptr, 
        "" 
    };
    openOfficeOne->exits["sør"] = { 
        corridorWest.get(), 
        nullptr, 
        "" 
    };

    //Kontorlandskap 1 <-> kontorlandskap 2
    openOfficeOne->exits["nord"] = {
        openOfficeTwo.get(),
        nullptr,
        ""
    };
    openOfficeTwo->exits["sør"] = {
        openOfficeOne.get(),
        nullptr,
        ""
    };
    openOfficeTwo->exits["nord"] = {
        northernCorridorOne.get(),
        nullptr,
        ""
    };

    //Korridor nord <-> kontorlandskap gruppe to, unisex toalett mot vest og nordre korridor videre mot øst.
    northernCorridorOne->exits["sør"] = {
        openOfficeTwo.get(),
        nullptr,
        ""
    };
    northernCorridorOne->exits["øst"] = {
        northernCorridorMid.get(),
        nullptr,
        ""
    };
    northernCorridorOne->exits["dør"] = {
        restroomsIT.get(),
        doorRestroomIT,
        ""
    };

    restroomsIT->exits["dør"] = {
        northernCorridorOne.get(),
        doorRestroomIT,
        ""
    };

    //Korridor nord midtparti <-> heis, Hotfix Hotel, Service Unavailable, korridor nord øst
    northernCorridorMid->exits["sør"] = {
        elevatorFromIT.get(),
        doorElevator,
        ""
    };
    northernCorridorMid->exits["hotfixhotel"] = {
        theHotfixHotel.get(),
        doorHotfix,
        "" + hotfixHotelAddr
    };
    theHotfixHotel->exits["dør"] = {
        northernCorridorMid.get(),
        doorHotfix,
        ""
    };
    serviceUnavailable->exits["dør"] = {
        northernCorridorMid.get(),
        doorServiceUnavailable,
        "" 
    };
    elevatorFromIT->exits["dør"] = {
        northernCorridorMid.get(),
        doorElevator,
        ""
    };
    northernCorridorMid->exits["serviceunavailable"] = {
        serviceUnavailable.get(),
        doorServiceUnavailable,
        "" + serviceUnavailableAddr
    };
    northernCorridorMid->exits["vest"] = {
        northernCorridorOne.get(),
        nullptr,
        ""
    };
    northernCorridorMid->exits["øst"] = {
        northernCorridorTwo.get(),
        nullptr,
        ""
    };

    //Nordre korridor øst <-> nordre korridor midt
    northernCorridorTwo->exits["vest"] = {
        northernCorridorMid.get(),
        nullptr,
        ""
    };
    northernCorridorTwo->exits["sør"] = {
        openOfficeThree.get(),
        nullptr,
        ""
    };


    openOfficeThree->exits["nord"] = {
        northernCorridorTwo.get(),
        nullptr,
        ""
    };
    openOfficeThree->exits["sør"] = {
        openOfficeFour.get(),
        nullptr,
        ""
    };
    openOfficeFour->exits["nord"] = {
        openOfficeThree.get(),
        nullptr,
        ""
    };
    openOfficeFour->exits["sør"] = {
        stairwellFromIT.get(),
        doorStairwell,
        ""
    };
    stairwellFromIT->exits["nord"] = {
        openOfficeFour.get(),
        doorStairwell,
        ""
    };


    //Korridor øst <-> Kopirom (to navn, SAMME dør)
    corridorEast->exits["nord"] = { 
        copyRoomOne.get(), 
        doorCopy, 
        "" 
    };
    corridorEast->exits["kopirom"] = { 
        copyRoomOne.get(), 
        doorCopy, 
        "" 
    };
    copyRoomOne->exits["dør"] = {
        corridorEast.get(), 
        doorCopy,
        "Døren ut igjen til kaffekroken. På denne siden henger det en lapp der det står: 'Døren kan kun låses opp utenfra'."
    };

    //Korridor øst <-> The Bikeshed
    corridorEast->exits["the bikeshed"] = {
        theBikeshed.get(), 
        doorBikeshed,
        "Denne døren leder inn til møterommet 'The Bikeshed' med romnummer " + theBikeshedAddr + " Det pågår en høylydt diskusjon der inne."
    };
    theBikeshed->exits["dør"] = {
        corridorEast.get(), 
        doorBikeshed,
        "Døren ut igjen til korridoren og kaffekroken."
    };

    // LEGG TIL ITEMS
    spawnRoom->items.push_back(getItem("kaffekopp"));
    spawnRoom->items.push_back(getItem("penn"));

    rubberDuckSanctum->items.push_back(getItem("liten gummiand"));

    salaSegfault->items.push_back(getItem("adgangskort"));  //Tror adgangskortene må gjøres spesifikke, men enn så lenge har vi ett!

    supplyRoom->items.push_back(getItem("kopipapir"));

    openOfficeTwo->items.push_back(getItem("hvit monster"));
    openOfficeTwo->items.push_back(getItem("lunken kaffe fra i går"));

    serviceUnavailable->items.push_back(getItem("gammel cat5-kabel"));

    

    // RANDOM ENCOUNTERS
    monsterManual manual;
    corridorEast->occupant = manual.getRandomMonster(1);  // Spawner et tilfeldig level 1 monster i østkorridoren
    northernCorridorOne->occupant = manual.getRandomMonster(1);
    openOfficeTwo->occupant = manual.getRandomMonster(1);


    // FAST ENCOUNTER
    salaSegfault->occupant = manual.getMonsterBlueprint(2, "Rogue Pointer");



    // NPCer
    openOfficeOne->npcs.push_back(createFirewallGuru(p));
    rubberDuckSanctum->npcs.push_back(createSlitenSenior(p));
    copyRoomOne->npcs.push_back(createFreedSecops(p));
    corridorEast->npcs.push_back(createLockedInSecopsVoice(p));

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
    rubberDuckSanctum->scenery["and"] = rubberDuckSanctum->scenery["gummiand"];
    rubberDuckSanctum->scenery["utvikler"] = "En seniorutvikler i falmet hettegenser. Han holder et halvt oppspist knekkebrød med brunost i den ene hånden, og peker med den andre på en skjerm full av rød feilmeldingstekst. Han mumler irritert: 'Jeg var SÅ nær ved å forstå hvorfor pekeren var null...'.";
    rubberDuckSanctum->scenery["skjerm"] = "Skjermen viser et terminalvindu med en endeløs strøm av stack traces og en åpen fane på StackOverflow fra 2014.";
    rubberDuckSanctum->scenery["laptop"] = rubberDuckSanctum->scenery["skjerm"];

    //Sala Segfault
    salaSegfault->scenery["blåskjerm"] = "Prosjektoren lyser opp hele rommet med en knallblå feilmelding: 'FATAL ERROR: Unhandled meeting exception at address " + salaSegfaultAddr + ". System halted.'";
    salaSegfault->scenery["prosjektor"] = salaSegfault->scenery["blåskjerm"];
    salaSegfault->scenery["vegg"] = salaSegfault->scenery["blåskjerm"];
    salaSegfault->scenery["dokumenter"] = "Utskrifter av arkitekturdiagrammer som gir absolutt ingen mening. Noen har skrevet 'HVEM SKREV DETTE?!' med rød tusj over hele forsiden.";
    salaSegfault->scenery["papirer"] = salaSegfault->scenery["dokumenter"];
    salaSegfault->scenery["stoler"] = "Kontorstoler som er veltet i panikk. Det ser ut som møtedeltakerne bokstavelig talt fordampet da krasjet inntraff.";

    //Korridoren mot vest
    corridorWest->scenery["kortleser"] = "En standard RFID-kortleser. Den blinker utålmodig med et lite rødt lys.";
    corridorWest->scenery["kodepanel"] = "Et slitt talltastatur fra 0 til 9. Tastene 1, 3, 5 og 7 ser merkbart mer slitte ut enn de andre.";
    corridorWest->scenery["dør"] = "På døren henger et skilt som sier 'REKVISITA'.";

    //Rekvisitarommet
    supplyRoom->scenery["kabler"] = "En floke av PS/2-adaptere, SCART-kabler og mystiske strømforsyninger som ingen tør å kaste 'i tilfelle vi får bruk for dem'.";
    supplyRoom->scenery["post-it"] = "Tårn på tårn med gule og rosa lapper. Den ultimate formen for agil prosjektstyring.";



    //Kontorlandskapet - gruppe 1
    openOfficeOne->scenery["brannmur-guruen"] = "En småtrøtt seniorutvikler omringet av tre buede skjermer, kabler og tomme bokser med hvit Monster™.";
    openOfficeOne->scenery["guru"] = openOfficeOne->scenery["brannmur-guruen"];
    openOfficeOne->scenery["skjermer"] = "Ulåste skjermer fulle av halvferdig kode, endeløse Slack-tråder og minst førti åpne nettleserfaner.";
    openOfficeOne->scenery["pulter"] = "Rader med hev-senk-pulter i ulike høyder. På en av dem står en ensom kaktus og et krus med teksten 'World's Okayest Developer'.";


    //Kontorlandskapet - gruppe 2
    openOfficeTwo->scenery["pult"] = "På den innerste pulten står en halvfull pappkopp ved siden av en stabel med uåpnede Jira-tickets.";
    openOfficeTwo->scenery["kaffekopp"] = openOfficeTwo->scenery["pult"];
    openOfficeTwo->hiddenItems["pult"] = "lunken kaffe fra i går";
    openOfficeTwo->scenery["bokser"] = "Under pulten ligger en liten pyramide av tomme energidrikk-bokser. Én av dem kjennes mistenkelig tung ut!";
    openOfficeTwo->hiddenItems["bokser"] = "hvit monster";

    //Nordre korridor - vest
    northernCorridorOne->scenery["restrooms"] = "Døren til IT-avdelingens toaletter. Under skiltet henger det en laminert lapp: 'Vennligst ikke flush ned uferdige branches her'.";

    //Restrooms IT
    restroomsIT->scenery["vask"] = "Et lite speilskap over vasken. Inni ligger en uåpnet pakke med oransje ørepropper – trolig lagt igjen av noen som trengte en pause fra åpent landskap.";
    restroomsIT->scenery["skap"] = restroomsIT->scenery["vask"];
    restroomsIT->hiddenItems["skap"] = "ørepropper i skum";

    //Nordre korridor - midt
    northernCorridorMid->scenery["the hotfix hotel"] = "Døren inn til møterommet 'The Hotfix Hotel' (" + hotfixHotelAddr + ")";
    northernCorridorMid->scenery["service unavailable"] = "Døren inn til møterommet '503: Service Unavailable' (" + serviceUnavailableAddr + "). Dette møterommet virker å være permanent booket av Per fra drift.";
    northernCorridorMid->scenery["elevator"] = "Heisen ser aldri ut til å være i riktig etasje når du trenger den. På heisdøren har noen hengt et ark hvor det står: 'UTE AV DRIFT – Går i uendelig løkke mellom 2. og 3. etasje. Ticket er opprettet (JIRA-404).'";


    //Nordre korridor - øst


    //The Hotfix Hotel
    theHotfixHotel->scenery["lapp"] = "Bordet er dekket av kafferinger og krøllete post-it-lapper med kryptiske stikkord som 'rollback?', 'hvem godkjente PR?' og 'skyld på DNS'.";
    theHotfixHotel->scenery["bord"] = "Blant krøllete post-it-lapper og tomme kaffekopper ligger det en halvspist energibar som noen glemte midt i en krise.";
    theHotfixHotel->scenery["energibar"] = theHotfixHotel->scenery["bord"];
    theHotfixHotel->hiddenItems["bord"] = "halvspist energibar";
    theHotfixHotel->scenery["stol"] = "Over stolryggen henger en slitt hettegenser som lukter svakt av svette og nattarbeid.";
    theHotfixHotel->hiddenItems["stol"] = "utslitt eventhettegenser";

    //503: Service Unavailable
    serviceUnavailable->scenery["servere"] = "Tårn på tårn med blinkende dioder. En av serverne har en gul post-it-lapp klistret over strømknappen med teksten: 'HVIS DENNE SKRUS AV, SLUTTER LØNNSUTBETALINGENE Å VIRKE'.";
    serviceUnavailable->scenery["kabelkveil"] = "En floke av gamle nettverkskabler slengt i et hjørne. Den øverste ser ut til å ha akkurat passe lengde og snert.";
    serviceUnavailable->hiddenItems["kabelkveil"] = "gammel cat5-kabel";

    //Kontorlandskapet - gruppe 3
    openOfficeThree->scenery["skuff"] = "En minimalistisk hvit skuffeseksjon under en pult. Den står på gløtt.";
    openOfficeThree->hiddenItems["skuff"] = "blålysbriller";

    //Kontorlandskapet - gruppe 4
    openOfficeFour->scenery["tastatur"] = "Et støvete, mekanisk tastatur som ligger slengt oppå en tom pizzaboks. Det er tungt nok til å brukes som slagvåpen.";
    openOfficeFour->hiddenItems["tastatur"] = "mekanisk tastatur";

    //Korridoren mot øst
    corridorEast->scenery["kaffemaskin"] = "En industriell trakter som har sett bedre dager. Den putrer illevarslende, og kannen er full av beksvart væske.";
    corridorEast->scenery["dør"] = "På døren henger et skilt der det står 'KOPIROM'. Ved siden av døren ser du en kortleser.";
    corridorEast->scenery["kopirom"] = corridorEast->scenery["dør"];
    corridorEast->scenery["møterom"] = "Gjennom glassveggen ser du tre utviklere som gestikulerer vilt rundt en whiteboard. De ser ut til å være i ferd med å bruke sin tredje time på en helt triviel detalj.";
    corridorEast->scenery["the bikeshed"] = corridorEast->scenery["møterom"];
    corridorEast->scenery["sittegruppe"] = "Et par slitte lenestoler og en tre-seters sofa i kunstskinn, og et lavt loungebord fullt av kafferinger og smuler.";
    corridorEast->scenery["sofa"] = "Mellom to sofaputer finner du en inntørket vestlandslefse.";  // "Undersøk sofa?" må trigge item.get("inntørket vestlandslefse") - vi trenger en handleExamine!
    corridorEast->scenery["lenestoler"] = "To nedsunkne lenestoler i imitert skinn. Den ene har en mistenkelig fjær som stikker opp av setet.";  //Så her må vi ha en "inspect/examine/undersøk fjær"?
    corridorEast->scenery["kortleser"] = "En hvit RFID-leser ved siden av kopirommet. Den lyser rødt og venter på et godkjent adgangskort.";
    corridorEast->hiddenItems["sofa"] = "inntørket vestlandslefse";
    corridorEast->scenery["fjær"] = "Du pirker borti den løse stålfjæren under puten og kjenner noe metallisk som har kilt seg fast!";
    corridorEast->hiddenItems["fjær"] = "løs stålfjær"; 

    //Kopirommet
    copyRoomOne->scenery["skriver"] = "En gigant av en kontorprinter. Den lager en lav, truende brummelyd, og det sitter et fastkjørt ark dypt inne i innvollene dens.";
    copyRoomOne->scenery["printer"] = copyRoomOne->scenery["skriver"];
    copyRoomOne->scenery["kopimaskin"] = copyRoomOne->scenery["skriver"];
    copyRoomOne->scenery["hyller"] = "Stabler med kopipapir og en makuleringsmaskin som ser ut til å ha tygget i seg noe som definitivt ikke var papir.";
    copyRoomOne->scenery["a4-ark"] = copyRoomOne->scenery["hyller"];

    //The Bikeshed
    theBikeshed->scenery["whiteboard"] = "Tavlen er dekket av en 14-stegs prosess for å avgjøre om knappen skal være #FF0000 eller #EE0000. Nederst står det 'Må avklares i styringsgruppen'.";
    theBikeshed->scenery["tavle"] = theBikeshed->scenery["whiteboard"];
    theBikeshed->scenery["utviklere"] = "Tre dypt konsentrerte utviklere som knapt enser at du kommer inn. En av dem mumler noe om 'kontrast-ratio og psykologisk effekt av hex-koder'.";
    theBikeshed->scenery["folk"] = theBikeshed->scenery["utviklere"];

    //PEKER TIL SPAWN
    Room* startingRoomPtr = spawnRoom.get();

    //DISCOVERY XP
    corridor->discoveryXP = 5;
    rubberDuckSanctum->discoveryXP = 5;
    salaSegfault->discoveryXP = 10;
    corridorWest->discoveryXP = 5;
    corridorEast->discoveryXP = 5;
    supplyRoom->discoveryXP = 10;
    openOfficeOne->discoveryXP = 5;
    openOfficeTwo->discoveryXP = 5;
    copyRoomOne->discoveryXP = 10;
    theBikeshed->discoveryXP = 5;
    northernCorridorOne->discoveryXP = 5;
    northernCorridorMid->discoveryXP = 5;
    northernCorridorTwo->discoveryXP = 5;
    serviceUnavailable->discoveryXP = 15;
    restroomsIT->discoveryXP = 5;
    elevatorFromIT->discoveryXP = 5;
    openOfficeThree->discoveryXP = 5;
    openOfficeFour->discoveryXP = 5;
    stairwellFromIT->discoveryXP = 5;


    spawnRoom->visited = true;      //Man får ikke XP bare for å starte spillet


    //LEGG ROMMENE I allRooms-vektoren
    allRooms.push_back(std::move(spawnRoom));
    allRooms.push_back(std::move(corridor));
    allRooms.push_back(std::move(rubberDuckSanctum));
    allRooms.push_back(std::move(salaSegfault));
    allRooms.push_back(std::move(corridorWest));
    allRooms.push_back(std::move(supplyRoom));
    allRooms.push_back(std::move(openOfficeOne));
    allRooms.push_back(std::move(openOfficeTwo));
    allRooms.push_back(std::move(northernCorridorOne));
    allRooms.push_back(std::move(northernCorridorMid));
    allRooms.push_back(std::move(northernCorridorTwo));
    allRooms.push_back(std::move(openOfficeThree));
    allRooms.push_back(std::move(openOfficeFour));
    allRooms.push_back(std::move(stairwellFromIT));
    allRooms.push_back(std::move(theHotfixHotel));
    allRooms.push_back(std::move(serviceUnavailable));
    allRooms.push_back(std::move(restroomsIT));
    allRooms.push_back(std::move(elevatorFromIT));
    allRooms.push_back(std::move(corridorEast));
    allRooms.push_back(std::move(copyRoomOne));
    allRooms.push_back(std::move(theBikeshed));



    //SEND SPAWN TIL game.cpp
    return startingRoomPtr;
    
}