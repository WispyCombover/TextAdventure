#include "RoomBlueprint.h"
#include "NPC.h"

const std::vector<DoorBlueprint>& getFloor1Doors() {
    static const std::vector<DoorBlueprint> doors = {
        { .id = "spawn" },
        { .id = "duck" },
        { .id = "sala" },
        { .id = "supply",   .locked = true, .keyName = "1357" },
        { .id = "copy",     .locked = true, .keyName = "adgangskort" },
        { .id = "bikeshed" },
        { .id = "restroom" },
        { .id = "hotfix" },
        { .id = "service" },
        { .id = "lift" },
        { .id = "stairs" }
    };
    return doors;
}

const std::vector<RoomBlueprint>& getFloor1Rooms() {
    static const std::vector<RoomBlueprint> rooms = {
        {
            .id = "spawn",
            .name = "Møterommet",
            .description = "Møterommet er som møterom flest. Du ser et bord med åtte stoler rundt. I taket henger en prosjektor, og på veggen henger et lerret.\n På bordet står det en tom og godt brukt kaffekopp som noen har glemt igjen, og det ligger en gammel bic penn ved siden av.\n  I et hjørne står en gammel whiteboard-tavle med nesten uleselige skriblerier, og det kommer et svakt lyst gjennom de frostede glassveggene på hver side av døren som leder ut.",
            .discoveryXP = 0,
            .startsVisited = true,
            .exits = {
                { "dør", "corridor", "spawn", "På døren har noen teipet fast et bilde av en flodhest og teksten: 'GOT MILK?'" }
            },
            .items = { "kaffekopp", "penn" },
            .scenery = {
                { {"prosjektor"}, "En gammel Epson-prosjektor med en kjølevifte som høres ut som et jetfly på tomgang. Lampen lyser svakt rødt." },
                { {"lerret"}, "Et nedtrekkbart lerret med en svak kaffeflekk nederst i hjørnet. Akkurat nå vises bare en tom, hvit flate." },
                { {"whiteboard", "tavle"}, "Tavlen er full av kaotiske piler, bokser merket 'MICROSERVICES?' og et utvisket forsøk på å tegne en katt." },
                { {"bord"}, "Et langt konferansebord i falsk eikefiner. Det er klissete på ett sted." }
            }
        },
        {
            .id = "corridor",
            .name = "Korridoren",
            .description = "En lang, smal korridor med falmede tapeter og knirkende gulvplanker som en herlig kontrast mot glassvinduer inn mot møterommene. Det er dører inn til to andre møterom langs veggen: 'RUBBER DUCK SANCTUM' og 'SALA SEGFAULT'.\nKorridoren fortsetter mot VEST og mot ØST, og du kan også gå tilbake til møterommet du nettopp forlot.",
            .discoveryXP = 5,
            .exits = {
                { "kaffekoppens krypt", "spawn", "spawn", "Fra denne siden kan du se dørskiltet som viser navnet på møterommet samt romnummeret. Dette rommet heter 'Kaffekoppens Krypt' og har nummer: ", true },
                { "rubber duck sanctum", "rubber_duck_sanctum", "duck", "Fra denne siden kan du se dørskiltet med navnet på møterommet: 'Rubber Duck Sanctum' samt romnummeret. Dette rommet har nummer: ", true },
                { "sala segfault", "sala_segfault", "sala", "Fra denne siden kan du se dørskiltet med navnet på møterommet: 'Sala Segfault' samt romnummeret. Dette rommet har nummer: ", true },
                { "vest", "corridor_west", "", "Når du ser mot vest kan du se døren inn til møterommet 'RUBBER DUCK SANCTUM', forbi døren fortsetter korridoren net til hjørnet med døren inn til REKVISITA. Veien fortsetter nordover derfra og inn i kontorlandskapet." },
                { "øst", "corridor_east", "", "Mot øst kan du se korridoren lede forbi møterommet 'SALA SEGFAULT' og ned til kaffekroken der også kopirommet ligger." }
            },
            .scenery = {
                { {"kaffekoppens krypt"}, "Døren til møterommet 'Kaffekoppens krypt' ({ADDR}). Det frostede glasset skjuler rommet der det hele startet, og ved siden av dørkarmen henger en gulnet lapp med teksten: 'Vennligst rydd ut kopper etter møtet - vi har ikke garbage collection her'.", "", "spawn" },
                { {"rubber duck sanctum"}, "Døren inn til 'Rubber Duck Sanctum' (romnummer: {ADDR}). På dørhåndtaket henger en liten lapp hvor det står: 'Ikke forstyrr - dyp feilsøking pågår'. Du syns du hører noen hviske forklaringer om pekere og minnelekkasjer innenfra.", "", "rubber_duck_sanctum" },
                { {"sala segfault"}, "På skiltet står det 'Sala Segfault' ({ADDR}). Det ser ut som om noen har prøvd å sparke opp døren fra innsiden, og under romnummeret har noen skrevet 'core dumped' med sprittusj.", "", "sala_segfault" }
            }
        },
        {
            .id = "rubber_duck_sanctum",
            .name = "Rubber Duck Sanctum",
            .description = "Du river opp døren og avbryter et intenst enmannsmøte. Lyset er dempet, og midt på konferansebordet står en diger, gul gummiand på en liten fløyelspute, belyst av en skrivebordslampe. Ved bordenden sitter en tydelig utslitt utvikler med bustete hår og stirrende blikk, som stopper brått midt i en setning og ser forfjamset på deg. Du kan gå ut igjen gjennom DØR.",
            .discoveryXP = 5,
            .exits = {
                { "dør", "corridor", "duck", "Døren tilbake til den knirkende korridoren. På døren henger et midtsidebilde av en gummiand. Nederst på bildet ser du teksten 'Frøken Februar 2016'." }
            },
            .items = { "liten gummiand" },
            .scenery = {
                { {"gummiand", "and"}, "En 30 centimeter høy, knall gul gummiand. Den har et dømmende, men tålmodig uttrykk i sine malte øyne, som om den allerede vet at feilen ligger på linje 42." },
                { {"utvikler"}, "En seniorutvikler i falmet hettegenser. Han holder et halvt oppspist knekkebrød med brunost i den ene hånden, og peker med den andre på en skjerm full av rød feilmeldingstekst. Han mumler irritert: 'Jeg var SÅ nær ved å forstå hvorfor pekeren var null...'." },
                { {"skjerm", "laptop"}, "Skjermen viser et terminalvindu med en endeløs strøm av stack traces og en åpen fane på StackOverflow fra 2014." }
            },
            .npcFactories = { createSlitenSenior }
        },
        {
            .id = "sala_segfault",
            .name = "Sala Segfault",
            .description = "I det du åpner døren, møtes du av et kaotisk syn. Møterommet ser ut til å ha krasjet midt i en kvartalspresentasjon. Stoler ligger veltet, dokumenter flyr rundt som konfetti, og prosjektoren på veggen viser en blendende blåskjerm. Midt i rommet svever en glitrende, ustabil anomali av rå minnedata og løse pekere som freser illevarslende. Utgangen er tilbake gjennom DØR.",
            .discoveryXP = 10,
            .exits = {
                { "dør", "corridor", "sala", "Døren ut igjen til korridoren. Det frister veldig å lukke den godt bak seg!" }
            },
            .items = { "adgangskort" },
            .scenery = {
                { {"blåskjerm", "prosjektor", "vegg"}, "Prosjektoren lyser opp hele rommet med en knallblå feilmelding: 'FATAL ERROR: Unhandled meeting exception at address {ADDR}. System halted.'", "", "sala_segfault" },
                { {"dokumenter", "papirer"}, "Utskrifter av arkitekturdiagrammer som gir absolutt ingen mening. Noen har skrevet 'HVEM SKREV DETTE?!' med rød tusj over hele forsiden." },
                { {"stoler"}, "Kontorstoler som er veltet i panikk. Det ser ut som møtedeltakerne bokstavelig talt fordampet da krasjet inntraff." }
            },
            .fixedMonsterLevel = 2,
            .fixedMonsterName = "Rogue Pointer"
        },
        {
            .id = "corridor_west",
            .name = "Korridoren mot vest",
            .description = "Du kommet til et hjørne som går videre mot NORD og inn i kontorlandskapet. Rett foran deg er der en dør in til REKVISITA. På veggen ved siden av døren sert du en et kodepanel.",
            .discoveryXP = 5,
            .exits = {
                { "øst", "corridor", "", "Du ser tilbake mot møterommet der det hele startet. Det virker så lenge siden nå." },
                { "rekvisita", "supply_room", "supply" },
                { "nord", "open_office_one" }
            },
            .scenery = {
                { {"kortleser"}, "En standard RFID-kortleser. Den blinker utålmodig med et lite rødt lys." },
                { {"kodepanel"}, "Et slitt talltastatur fra 0 til 9. Tastene 1, 3, 5 og 7 ser merkbart mer slitte ut enn de andre." },
                { {"dør"}, "På døren henger et skilt som sier 'REKVISITA'." }
            }
        },
        {
            .id = "supply_room",
            .name = "Rekvisitarommet",
            .description = "Et trangt, vindusløst rom stappet til randen med pappesker fulle av utdaterte VGA-kabler, post-it-blokker i neonfarger og tonerkassetter til en skriver som ble kastet i 2016. Det lukter svakt av støv og uåpnet plast. Utgangen er tilbake gjennom DØR.",
            .discoveryXP = 10,
            .exits = {
                { "dør", "corridor_west", "supply" }
            },
            .items = { "kopipapir" },
            .scenery = {
                { {"kabler"}, "En floke av PS/2-adaptere, SCART-kabler og mystiske strømforsyninger som ingen tør å kaste 'i tilfelle vi får bruk for dem'." },
                { {"post-it"}, "Tårn på tårn med gule og rosa lapper. Den ultimate formen for agil prosjektstyring." }
            }
        },
        {
            .id = "open_office_one",
            .name = "Kontorlandskapet - gruppe 1",
            .description = "Et åpent kontorlandskap med rekker av hev-senk-pulter og flimrende lysrør i taket. Luften er tung av konsentrasjon og lavmælt tasting. De fleste pultene er forlatt i hui og hast, med halvfulle kaffekopper og ulåste skjermer. Du kan gå tilbake mot SØR, eller videre mot NORD. Her ser du også Brannmur-Guruen sitte og stirre inn i skjermene sine.",
            .discoveryXP = 5,
            .exits = {
                { "sør", "corridor_west" },
                { "nord", "open_office_two" }
            },
            .scenery = {
                { {"brannmur-guruen", "guru"}, "En småtrøtt seniorutvikler omringet av tre buede skjermer, kabler og tomme bokser med hvit Monster™." },
                { {"skjermer"}, "Ulåste skjermer fulle av halvferdig kode, endeløse Slack-tråder og minst førti åpne nettleserfaner." },
                { {"pulter"}, "Rader med hev-senk-pulter i ulike høyder. På en av dem står en ensom kaktus og et krus med teksten 'World's Okayest Developer'." }
            },
            .npcFactories = { createFirewallGuru }
        },
        {
            .id = "open_office_two",
            .name = "Kontorlandskapet - gruppe 2",
            .description = "Du trår inn i den delen av kontorlandskapet der støyreduserende hodetelefoner er påbudt verneutstyr. Veggene er tapetsert med gamle sprint-tavler og falmede 'Keep Calm and git push --force'-plakater. En enslig vifte durer i hjørnet i et tappert, men forgjeves forsøk på å kjøle ned luften fra femten overarbeidede utvikler-laptoper. Du kan gå videre mot NORD, eller tilbake mot SØR.",
            .discoveryXP = 5,
            .exits = {
                { "sør", "open_office_one" },
                { "nord", "northern_corridor_one" }
            },
            .items = { "hvit monster", "lunken kaffe fra i går" },
            .scenery = {
                { {"pult", "kaffekopp"}, "På den innerste pulten står en halvfull pappkopp ved siden av en stabel med uåpnede Jira-tickets.", "lunken kaffe fra i går" },
                { {"bokser"}, "Under pulten ligger en liten pyramide av tomme energidrikk-bokser. Én av dem kjennes mistenkelig tung ut!", "hvit monster" }
            },
            .randomMonsterLevel = 1
        },
        {
            .id = "northern_corridor_one",
            .name = "Den nordre korridoren - vest",
            .description = "Du står i den nordvestre enden av hovedkorridoren. Gulvbelegget er slitt etter utallige sprint-retrospektiver og hastige turer til kaffemaskinen. Mot VEST er det en dør merket med et piktogram av en kjønnsnøytral strekfigur med en laptop. Du kan gå sørover inn i kontorlandskapet, eller følge korridoren mot ØST.",
            .discoveryXP = 5,
            .exits = {
                { "sør", "open_office_two" },
                { "øst", "northern_corridor_mid" },
                { "dør", "restrooms_it", "restroom" }
            },
            .scenery = {
                { {"restrooms"}, "Døren til IT-avdelingens toaletter. Under skiltet henger det en laminert lapp: 'Vennligst ikke flush ned uferdige branches her'." }
            },
            .randomMonsterLevel = 1
        },
        {
            .id = "restrooms_it",
            .name = "Unisex restrooms for IT-avdelingen",
            .description = "Overraskende rent, med dempet, behagelig belysning og en duft av billig håndsåpe og eksistensiell ro. På innsiden av båsdøren har noen risset inn '/* TODO: Vask hender */' med en nøkkel. Dette er det eneste stedet i bygget hvor ingen spør deg om et kjapt estimat. Utgangen er tilbake gjennom DØR.",
            .discoveryXP = 5,
            .exits = {
                { "dør", "northern_corridor_one", "restroom" }
            },
            .scenery = {
                { {"vask", "skap"}, "Et lite speilskap over vasken. Inni ligger en uåpnet pakke med oransje ørepropper – trolig lagt igjen av noen som trengte en pause fra åpent landskap.", "ørepropper i skum" }
            }
        },
        {
            .id = "northern_corridor_mid",
            .name = "Den nordre korridoren - senter",
            .description = "Midtpunktet i den nordre korridoren. Langs veggen ligger to møterom: 'THE HOTFIX HOTEL' og '503: SERVICE UNAVAILABLE'. Rett mot NORD finner du heisdørene, mens korridoren strekker seg videre mot VEST og ØST.",
            .discoveryXP = 5,
            .exits = {
                { "sør", "elevator_f1", "lift" },
                { "hotfixhotel", "the_hotfix_hotel", "hotfix", "", true },
                { "serviceunavailable", "service_unavailable", "service", "", true },
                { "vest", "northern_corridor_one" },
                { "øst", "northern_corridor_two" }
            },
            .scenery = {
                { {"the hotfix hotel"}, "Døren inn til møterommet 'The Hotfix Hotel' ({ADDR})", "", "the_hotfix_hotel" },
                { {"service unavailable"}, "Døren inn til møterommet '503: Service Unavailable' ({ADDR}). Dette møterommet virker å være permanent booket av Per fra drift.", "", "service_unavailable" },
                { {"elevator"}, "Heisen ser aldri ut til å være i riktig etasje når du trenger den. På heisdøren har noen hengt et ark hvor det står: 'UTE AV DRIFT – Går i uendelig løkke mellom 2. og 3. etasje. Ticket er opprettet (JIRA-404).'" }
            }
        },
        {
            .id = "the_hotfix_hotel",
            .name = "The Hotfix Hotel",
            .description = "Et kaotisk lite møterom som bærer preg av å ha blitt tatt i bruk på fem minutters varsel en fredag ettermiddag. Halvspiste energibarer ligger strødd rundt på bordet, og på veggen henger en lapp hvor det står: 'Midlertidig løsning implementert i 2021 – ikke rør!'. Utgangen er tilbake gjennom DØR.",
            .discoveryXP = 5,
            .exits = {
                { "dør", "northern_corridor_mid", "hotfix" }
            },
            .scenery = {
                { {"lapp"}, "Bordet er dekket av kafferinger og krøllete post-it-lapper med kryptiske stikkord som 'rollback?', 'hvem godkjente PR?' og 'skyld på DNS'." },
                { {"bord", "energibar"}, "Blant krøllete post-it-lapper og tomme kaffekopper ligger det en halvspist energibar som noen glemte midt i en krise.", "halvspist energibar" },
                { {"stol"}, "Over stolryggen henger en slitt hettegenser som lukter svakt av svette og nattarbeid.", "utslitt eventhettegenser" }
            }
        },
        {
            .id = "service_unavailable",
            .name = "503: Service Unavailable",
            .description = "Rommet er stappfullt av gamle serverrack, blinkende switcher og tykke bunter med patchekabler som henger som lianer fra taket. Det durer øredøvende fra kjøleviftene, og en rød varsellampe blinker taktfast i hjørnet. Et skilt på veggen sier: 'Per fra drift er på ferie. Prøv igjen senere.' Utgangen er tilbake gjennom DØR.",
            .discoveryXP = 15,
            .exits = {
                { "dør", "northern_corridor_mid", "service" }
            },
            .items = { "gammel cat5-kabel" },
            .scenery = {
                { {"servere"}, "Tårn på tårn med blinkende dioder. En av serverne har en gul post-it-lapp klistret over strømknappen med teksten: 'HVIS DENNE SKRUS AV, SLUTTER LØNNSUTBETALINGENE Å VIRKE'." },
                { {"kabelkveil"}, "En floke av gamle nettverkskabler slengt i et hjørne. Den øverste ser ut til å ha akkurat passe lengde og snert.", "gammel cat5-kabel" }
            }
        },
        {
            .id = "elevator_f1",
            .name = "Personheis",
            .description = "En trang stålheis som lukter svakt av våt ull og kald kaffe. Panelknappene er slitte, og displayet over døren viser bare et kryptisk 'NaN' i stedet for etasjenummer. Du kan gå ut igjen mot SØR.",
            .discoveryXP = 5,
            .exits = {
                { "dør", "northern_corridor_mid", "lift" }
            }
        },
        {
            .id = "northern_corridor_two",
            .name = "Den nordre korridoren - øst",
            .description = "Den østlige enden av den nordre korridoren. Her er det litt stillere, og gjennom et lite vindu ser du ut mot en grå bakgård. Korridoren går tilbake mot VEST, eller du kan bevege deg sørover inn mot resten av kontorlandskapet.",
            .discoveryXP = 5,
            .exits = {
                { "vest", "northern_corridor_mid" },
                { "sør", "open_office_three" }
            }
        },
        {
            .id = "open_office_three",
            .name = "Kontorlandskapet - gruppe 3",
            .description = "Her sitter frontend-gjengen. Pultene er mistenkelig ryddige, og belysningen er justert til en behagelig, estetisk korrekt fargetemperatur. På en tavle henger det utallige fargeprøver og en opphetet diskusjon om padding på 4px versus 6px. Korridoren ligger mot VEST, mens landskapet fortsetter mot SØR.",
            .discoveryXP = 5,
            .exits = {
                { "nord", "northern_corridor_two" },
                { "sør", "open_office_four" }
            },
            .scenery = {
                { {"skuff"}, "En minimalistisk hvit skuffeseksjon under en pult. Den står på gløtt.", "blålysbriller" }
            }
        },
        {
            .id = "open_office_four",
            .name = "Kontorlandskapet - gruppe 4",
            .description = "Det innerste hjørnet av kontorlandskapet, preget av tomme pizzabokser og mekaniske tastaturer med altfor høy klikkelyd. Luften vibrerer svakt av koffein og eksistensiell tvil rundt teknisk gjeld. Du kan gå mot NORD til gruppe 3, eller mot ØST mot trappegangen.",
            .discoveryXP = 5,
            .exits = {
                { "nord", "open_office_three" },
                { "sør", "stairwell_f1", "stairs" }
            },
            .scenery = {
                { {"tastatur"}, "Et støvete, mekanisk tastatur som ligger slengt oppå en tom pizzaboks. Det er tungt nok til å brukes som slagvåpen.", "mekanisk tastatur" }
            }
        },
        {
            .id = "stairwell_f1",
            .name = "Trappegangen",
            .description = "En betongtrapp med gjenklang og et ekkelt, kaldt trekk. På veggen henger et falmet branninstruks-skilt og en plakat som oppfordrer til å ta trappene for helsens skyld. Ingen tar trappene.",
            .discoveryXP = 5,
            .exits = {
            { "nord", "open_office_four", "stairs" }
            }
        },
        {
            .id = "corridor_east",
            .name = "Korridoren mot øst",
            .description = "Du kommer til et lite hjørne av korridoren. Du kan høre banking fra døren til kopirommet, og lyden av engasjert prat fra møterommet mot SØR. Møterommet heter: 'THE BIKESHED'. Her ser du også en liten sittegruppe med noen stoler og et bord, og på et lite bord i hjørnet står en gammel kaffemaskin. \nDu kan gå tilbake mot VEST, eller gjennom en dør mot NORD som har et skilt med 'KOPIROM'. Ved siden av døren til kopirommet henger en kortleser.",
            .discoveryXP = 5,
            .exits = {
                { "vest", "corridor", "", "Du ser tilbake ned korridoren mot vest. Der ligger møterommet 'Kaffekoppens Krypt', og veien videre mot rekvisitalageret" },
                { "nord", "copy_room_one", "copy" },
                { "kopirom", "copy_room_one", "copy" },
                { "the bikeshed", "the_bikeshed", "bikeshed", "Denne døren leder inn til møterommet 'The Bikeshed' med romnummer {ADDR} Det pågår en høylydt diskusjon der inne.", true }
            },
            .scenery = {
                { {"kaffemaskin"}, "En industriell trakter som har sett bedre dager. Den putrer illevarslende, og kannen er full av beksvart væske." },
                { {"dør", "kopirom"}, "På døren henger et skilt der det står 'KOPIROM'. Ved siden av døren ser du en kortleser." },
                { {"møterom", "the bikeshed"}, "Gjennom glassveggen ser du tre utviklere som gestikulerer vilt rundt en whiteboard. De ser ut til å være i ferd med å bruke sin tredje time på en helt triviel detalj." },
                { {"sittegruppe"}, "Et par slitte lenestoler og en tre-seters sofa i kunstskinn, og et lavt loungebord fullt av kafferinger og smuler." },
                { {"sofa"}, "Mellom to sofaputer finner du en inntørket vestlandslefse.", "inntørket vestlandslefse" },
                { {"lenestoler"}, "To nedsunkne lenestoler i imitert skinn. Den ene har en mistenkelig fjær som stikker opp av setet." },
                { {"kortleser"}, "En hvit RFID-leser ved siden av kopirommet. Den lyser rødt og venter på et godkjent adgangskort." },
                { {"fjær"}, "Du pirker borti den løse stålfjæren under puten og kjenner noe metallisk som har kilt seg fast!", "løs stålfjær" }
            },
            .randomMonsterLevel = 1,
            .npcFactories = { createLockedInSecopsVoice }
        },
        {
            .id = "copy_room_one",
            .name = "Kopirommet",
            .description = "En klaustrofobisk boks dominert av en gigantisk multifunksjonsskriver som blinker illevarslende med 'PC LOAD LETTER'. Hyllene langs veggen er fulle av A4-ark og makuleringsposer. Utgangen er tilbake mot SØR.",
            .discoveryXP = 10,
            .exits = {
                { "dør", "corridor_east", "copy", "Døren ut igjen til kaffekroken. På denne siden henger det en lapp der det står: 'Døren kan kun låses opp utenfra'." }
            },
            .scenery = {
                { {"skriver", "printer", "kopimaskin"}, "En gigant av en kontorprinter. Den lager en lav, truende brummelyd, og det sitter et fastkjørt ark dypt inne i innvollene dens." },
                { {"hyller", "a4-ark"}, "Stabler med kopipapir og en makuleringsmaskin som ser ut til å ha tygget i seg noe som definitivt ikke var papir." }
            },
            .npcFactories = { createFreedSecops }
        },
        {
            .id = "the_bikeshed",
            .name = "The Bikeshed",
            .description = "Et lite møterom der en opphetet diskusjon pågår. På whiteboarden står det 'VALG AV FARGE PÅ LOGG-KNAPP' understreket tre ganger, omringet av komplekse arkitekturdiagrammer. Døren tilbake til korridoren er mot NORD.",
            .discoveryXP = 5,
            .exits = {
                { "dør", "corridor_east", "bikeshed", "Døren ut igjen til korridoren og kaffekroken." }
            },
            .scenery = {
                { {"whiteboard", "tavle"}, "Tavlen er dekket av en 14-stegs prosess for å avgjøre om knappen skal være #FF0000 eller #EE0000. Nederst står det 'Må avklares i styringsgruppen'." },
                { {"utviklere", "folk"}, "Tre dypt konsentrerte utviklere som knapt enser at du kommer inn. En av dem mumler noe om 'kontrast-ratio og psykologisk effekt av hex-koder'." }
            }
        }
    };
    return rooms;
}
