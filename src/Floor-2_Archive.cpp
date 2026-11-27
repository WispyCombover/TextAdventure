#include "RoomBlueprint.h"
#include "NPC.h"
#include <string>
#include <utility>
#include <vector>

namespace {
    const std::vector<std::string> kSteps = { "bak", "old", "tmp", "final", "final2", "final_endelig" };

    const char* kExitHints[] = {
        "Seks ganger går videre, hver merket på en slitt papp-etikett: BAK, OLD, TMP, FINAL, FINAL2 og FINAL_ENDELIG.",
        "Gangene fortsetter i seks retninger. Noen har skrevet navnene med tusj på hyllekanten: BAK, OLD, TMP, FINAL, FINAL2 og FINAL_ENDELIG.",
        "Det er de samme seks gangene her også, merket BAK, OLD, TMP, FINAL, FINAL2 og FINAL_ENDELIG. Navnene virker like usannsynlige hver gang.",
        "Etikettene er flekkete, men leselige: BAK, OLD, TMP, FINAL, FINAL2 og FINAL_ENDELIG.",
        "Du kjenner systemet igjen nå: BAK, OLD, TMP, FINAL, FINAL2 og FINAL_ENDELIG. Én av dem er riktig. Resten er arkivering."
    };

    struct JunctionScenery {
        std::vector<std::string> aliases;
        std::string text;
        std::string hidden;          // tom = ingen skjult gjenstand
    };

    struct JunctionDef {
        std::string name;
        std::string description;     
        std::vector<JunctionScenery> scenery;
        std::vector<std::string> items;
        int monsterChance = 0;       // 0-100
        std::vector<std::pair<int, std::string>> monsterPool;
    };

    const std::vector<JunctionDef>& junctions() {
        static const std::vector<JunctionDef> defs = {
            {
                .name = "Papirarkivet - Skannerhjørnet",
                .description = "Gangen åpner seg mot et hjørne der noen har satt opp en flatbedskanner fra tidlig 2000-tall. Den står på og viser det samme dokumentet i en evig loop. Rundt den ligger bunker med ferdig skannede mapper, alle merket «SKAL MAKULERES ETTER SKANNING». Ingen har makulert noe.",
                .scenery = {
                    { {"skanner", "flatbedskanner"}, "En Epson-skanner fra 2003. Skjermen viser «Side 1 av 4000. Estimert tid: 11 år.» Under skannerlokket finner du en diskett som noen har glemt der.", "diskett" },
                    { {"bunker", "mapper"}, "Du blar i en av bunkene. Nederst finner du en dokumentmappe merket «final_v2_NY_ENDELIG».", "dokumentmappe" }
                },
                .items = { "arkivperm" }
            },
            {
                .name = "Papirarkivet - Hullkortrekkene",
                .description = "Her er hyllene lave og dype, fulle av hullkort med gummistrikk rundt hver bunke. Et tykt støvlag ligger over alt som en dyne, og det rasler et sted bak deg. Det er sikkert bare trekk.",
                .scenery = {
                    { {"hullkort", "bunke"}, "Hullkort sortert etter en metode som sist ble forstått i 1987. På det øverste står det «IKKE STOKK». Noen har stokket. Du finner ett kort som har falt ned på gulvet.", "hullkort" },
                    { {"hylle", "reol"}, "Under en bunke finner du en rusten stiftemaskin. Den ser ut til å ha holdt sammen hele hylla.", "stiftemaskin" }
                },
                .monsterChance = 50,
                .monsterPool = { {2, "The Paper Jam"}, {2, "The Jira Ticket Overlord"} }
            },
            {
                .name = "Papirarkivet - Ansattarkivet",
                .description = "Personalmapper står i pene rader, sortert etter etternavn, så fornavn, så hvilken etasje de jobbet i, og til slutt hvem som likte hvem. Her ligger stillheten tett. Noen har tydeligvis tatt en lang lunsjpause her for veldig lenge siden.",
                .scenery = {
                    { {"personalmapper", "mapper"}, "Du blar litt. De fleste inneholder en arbeidskontrakt og en lapp: «Se vedlegg». Vedlegget mangler. Bak en av mappene sitter et utløpt ansattkort fast.", "ansattkort" },
                    { {"organisasjonskart", "vegg"}, "Et stort, kaffeflekket organisasjonskart. I toppen står det «DIREKTØR». Under står det «DIREKTØR». Så «DIREKTØR». Det henger løst, så du tar det ned.", "organisasjonskart" }
                },
                .items = { "telefonkatalog" }
            },
            {
                .name = "Papirarkivet - Bånd og kassetter",
                .description = "Hyllene her er fylt av bånd, kassetter og VHS-er med navn som «Q3-rapport (kopi)», «Julebord 1994» og «IKKE SPILL AV». Det summer svakt fra en bandstasjon i hjørnet som står på, selv om kabelen ikke er koblet til noe.",
                .scenery = {
                    { {"vhs", "kassetter"}, "Blant stablene ligger en VHS merket «Opplæring i brannsikkerhet (1992)». Du føler deg plutselig mistenkelig skyldig i noe.", "vhs-kassett" },
                    { {"båndstasjon"}, "Båndstasjonen spinner på tomgang uten strøm. Innvendig sitter det en båndkassett merket «BACKUP (IKKE SLETT)». Datoen er 2009.", "sikkerhetskopi" }
                },
                .monsterChance = 50,
                .monsterPool = { {3, "Stale Backup Tape"}, {3, "final_v3_ENDELIG(2).docx"} }
            },
            {
                .name = "Papirarkivet - Makuleringsrommet",
                .description = "Rommet lukter av papirstøv og noe som minner om brent kaffe. Midt på gulvet står en industriell makuleringsmaskin med tusenvis av remser strødd utover. Lenger inn kjenner du et svakt trekk, og du hører noe stort puste.",
                .scenery = {
                    { {"makuleringsmaskin", "makulator"}, "Maskinen er full til randen. En ferdig pakket pose med remser står ved siden av, klar til å bli glemt.", "makuleringspose" },
                    { {"stempel", "skrivebord"}, "På et skrivebord står en rekke gummistempler: «MOTTATT», «AVSLÅTT», «UNDER BEHANDLING». Du tar «GODKJENT».", "gummistempel" },
                    { {"lapp", "vegg"}, "En gul lapp på veggen: «Husk backup!». Noen har føyd til «Gjorde det ikke.» Du tar den med deg.", "post-it" }
                },
                .monsterChance = 40,
                .monsterPool = { {3, "final_v3_ENDELIG(2).docx"}, {3, "Stale Backup Tape"} }
            }
        };
        return defs;
    }

    // Seks utganger i hvert rom: bare steps[correct] går videre, resten sender deg tilbake til porten.
    void addAisles(RoomBlueprint& r, size_t correct, const std::string& nextId) {
        for (size_t i = 0; i < kSteps.size(); ++i) {
            const std::string target = (i == correct) ? nextId : std::string("archive_gate");
            r.exits.push_back({ kSteps[i], target, "", "En smal gang mellom to hyller. Etiketten er håndskrevet og delvis utvisket." });
        }
    }
}

const std::vector<RoomBlueprint>& getFloor_2Rooms() {
    static const std::vector<RoomBlueprint> rooms = [] {
        std::vector<RoomBlueprint> v;

        // Inngangen = steg 0 (riktig utgang: "bak")
        RoomBlueprint gate;
        gate.id = "archive_gate";
        gate.name = "Papirarkivet - inngangen";
        gate.description = "Trappen ender i et betongrom med lav himling og en dør som står permanent åpen. Foran deg strekker et arkiv seg inn i mørket: rekke på rekke av grå stålhyller, fylt med mapper ingen har åpnet siden forrige revisjon. På veggen henger et skilt: 'ARKIVET – kun for autorisert personell. Alle henvendelser må leveres skriftlig i tre eksemplarer.' Bak en liten pult ved inngangen sitter det en ung mann. Seks ganger fører videre: BAK, OLD, TMP, FINAL, FINAL2 og FINAL_ENDELIG. Trappen går tilbake OPP.";
        gate.discoveryXP = 10;
        addAisles(gate, 0, "archive_1");
        gate.exits.push_back({ "opp", "stairwell_f1", "", "Trappen tilbake opp til første etasje." });
        gate.scenery.push_back({ {"skilt"}, "Skiltet er laminert og har en uleselig underskrift. Under er det klistret en gul lapp: 'Takk for din tålmodighet.'" });
        gate.scenery.push_back({ {"pult"}, "En liten pult med en kalender som har stått på mars 2009. På skjermen blinker en e-post fra HR: «Vi kommer tilbake til deg.»" });
        gate.npcFactories.push_back(createPraktikant);
        v.push_back(gate);

        // Gang 1-5
        const auto& defs = junctions();
        for (size_t k = 1; k <= defs.size(); ++k) {
            const JunctionDef& def = defs[k - 1];
            RoomBlueprint r;
            r.id = "archive_" + std::to_string(k);
            r.name = def.name;
            r.description = def.description + " " + kExitHints[k - 1];
            r.discoveryXP = 3;
            r.items = def.items;
            for (const auto& s : def.scenery) r.scenery.push_back({ s.aliases, s.text, s.hidden });
            r.monsterChancePercent = def.monsterChance;
            r.monsterPool = def.monsterPool;
            const std::string next = (k < defs.size()) ? "archive_" + std::to_string(k + 1) : std::string("archive_heart");
            addAisles(r, k, next);
            v.push_back(r);
        }

        // Arkivets sentrum (bossfight)
        RoomBlueprint heart;
        heart.id = "archive_heart";
        heart.name = "Papirarkivet - hjertet";
        heart.description = "Gangene åpner seg til en stor sirkulær hall der tusenvis av ark flyter i luften som støv i sollys. I midten ligger en tom reol der det tydeligvis har stått én enkelt bok. Ut-døren leder tilbake til inngangen.";
        heart.discoveryXP = 15;
        heart.fixedMonsterLevel = 4;                       
        heart.fixedMonsterName = "The Retention Policy";
        heart.exits.push_back({ "ut", "archive_gate", "", "Gangen leder tilbake til arkivets inngang." });
        heart.scenery.push_back({ {"reol", "hylle"},
            "En tom hylle midt i rommet. Et merke i støvet viser at en bok på størrelse med The Mythical Man-Month har stått her. Bak hylla har noe falt ned: en zip-disk merket «Click of Death».",
            "zip-disk" });
        v.push_back(heart);

        return v;
    }();
    return rooms;
}