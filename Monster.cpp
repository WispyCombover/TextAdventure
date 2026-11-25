#include "Monster.h"
#include "Item.h"
#include "Utils.h"
#include <map>

namespace {
    const std::map<int, std::vector<monster>>& getMonsterDatabase() {
        static const std::map<int, std::vector<monster>> db = {
            // ================= LEVEL 1 =================
            { 1, {
                {
                    .name = "Merge Conflict Wizard",
                    .description = "En skikkelse kledd i hettegenseren til servicedesk. Den mumler uforståelige git-kommandoer.",
                    .level = 1,
                    .hp = 10,
                    .maxHp = 10,
                    .attackDamage = 1,
                    .damageDiceCount = 1,
                    .damageDiceSides = 6,
                    .xpReward = 5,
                    .monsterAC = 10,
                    .weaknesses = {"slashing"},
                    .attackMessage = "Kaster en 'Git Push Force' som korrumperer filene dine!",
                    .minDataReward = 128,
                    .maxDataReward = 512,
                    .lootTable = {
                        { getItem("rød monster"), 60 },
                        { getItem("gammel smultring"), 40 },
                        { getItem("utslitt eventhettegenser"), 15 },
                        { getItem("løs side (git stash)"), 8 }
                    }
                },
                {
                    .name = "The Cookie Banner",
                    .description = "En vegg av uleselig juridisk tekst som blokkerer utsikten din. Den nekter å forsvinne.",
                    .level = 1,
                    .hp = 8,
                    .maxHp = 8,
                    .attackDamage = 0,
                    .damageDiceCount = 1,
                    .damageDiceSides = 4,
                    .xpReward = 7,
                    .monsterAC = 12,
                    .weaknesses = {"acid", "fire"},
                    .attackMessage = "Spammer deg med 'Godta alle'-knapper til du mister oversikten!",
                    .minDataReward = 150,
                    .maxDataReward = 450,
                    .lootTable = {
                        { getItem("mango loco"), 55 },
                        { getItem("kald filterkaffe"), 40 },
                        { getItem("blålysbriller"), 12 }
                    }
                },
                {
                    .name = "The Missing Semicolon",
                    .description = "En mikroskopisk skapning som gjemmer seg i skyggene. Umulig å se med det blotte øyet.",
                    .level = 1,
                    .hp = 5,
                    .maxHp = 5,
                    .attackDamage = 0,
                    .damageDiceCount = 1,
                    .damageDiceSides = 6,
                    .xpReward = 10,
                    .monsterAC = 14,
                    .weaknesses = {"sonic"},
                    .attackMessage = "Angriper i mørket, får deg til å snuble og kaster en 'Compilation Error' rett i ansiktet ditt!",
                    .minDataReward = 200,
                    .maxDataReward = 600,
                    .lootTable = {
                        { getItem("hvit monster"), 60 },
                        { getItem("boks med trykkluft"), 25 },
                        { getItem("løs side (grep)"), 10 }
                    }
                }
            }},

            // ================= LEVEL 2 =================
            { 2, {
                {
                    .name = "The Daily Standup Spectre",
                    .description = "Et gjenferd som tvinger deg til å stå oppreist mens det spør: 'Hva gjorde du i går?'.",
                    .level = 2,
                    .hp = 20,
                    .maxHp = 20,
                    .attackDamage = 0,
                    .damageDiceCount = 2,
                    .damageDiceSides = 4,
                    .xpReward = 15,
                    .monsterAC = 14,
                    .attackMessage = "Holder en endeløs monolog og påfører deg 'Feature Creep'-skade over tid!",
                    .minDataReward = 400,
                    .maxDataReward = 1000,
                    .lootTable = {
                        { getItem("hvit monster"), 65 },
                        { getItem("støydempende headsett"), 15 },
                        { getItem("løs side (ping)"), 12 }
                    }
                },
                {
                    .name = "The Jira Ticket Overlord",
                    .description = "Et monster laget av post-it-lapper og uleste varsler. Den flytter deg konstant mellom 'In Progress' og 'To Do'.",
                    .level = 2,
                    .hp = 25,
                    .maxHp = 25,
                    .attackDamage = 1,
                    .damageDiceCount = 1,
                    .damageDiceSides = 6,
                    .xpReward = 15,
                    .monsterAC = 15,
                    .weaknesses = {"fire"},
                    .attackMessage = "Oppretter tre nye sub-tasks som du MÅ løse med én gang!",
                    .minDataReward = 400,
                    .maxDataReward = 1000,
                    .lootTable = {
                        { getItem("mango loco"), 60 },
                        { getItem("deodorant og lighter"), 20 },
                        { getItem("løs side (git blame)"), 10 }
                    }
                },
                {
                    .name = "Rogue Pointer",
                    .description = "En glitrende, ustabil anomali av rå minnedata og løse pekere som peker rett ut i intet. Den flimrer inn og ut av eksistens med en skarp, statisk knitring.",
                    .level = 2,
                    .hp = 14,
                    .maxHp = 14,
                    .attackDamage = 1,
                    .damageDiceCount = 1,
                    .damageDiceSides = 6,
                    .xpReward = 20,
                    .monsterAC = 12,
                    .isUniqueBoss = true, // Kun i Sala Segfault
                    .attackMessage = "Hopper til en uallokert minneadresse og kaster en 'Segmentation Fault (core dumped)' rett i brystet ditt!",
                    .weaknesses = {"lightning"},
                    .minDataReward = 500,
                    .maxDataReward = 1300,
                    .lootTable = {
                        { getItem("anti-statisk armlenke"), 100 },
                        { getItem("hvit monster"), 75 }
                    }
                }
            }},

            // ================= LEVEL 3 =================
            { 3, {
                {
                    .name = "The Retention Policy",
                    .description = "En kolossal minotaur vevd sammen av gulnede A4-ark, stifter og uoppsigelige arkivforskrifter.",
                    .level = 3,
                    .hp = 55,
                    .maxHp = 55,
                    .attackDamage = 2,
                    .damageDiceCount = 2,
                    .damageDiceSides = 8,
                    .xpReward = 22,
                    .monsterAC = 15,
                    .weaknesses = {"fire", "acid"},
                    .attackMessage = "Begraver deg under ti års lovpålagt dokumentasjon som ingen har lov til å kaste!",
                    .isUniqueBoss = true, // Boss i papirarkivet
                    .minDataReward = 600,
                    .maxDataReward = 1400,
                    .lootTable = {
                        { getItem("the mythical man-month"), 100 },
                        { getItem("løs side (locate)"), 50 }
                    }
                },
                {
                    .name = "The DNS Daemon",
                    .description = "Ingen vet hva det er, eller hvor det kommer fra, men alt slutter å fungere når det våkner.",
                    .level = 3,
                    .hp = 37,
                    .maxHp = 37,
                    .attackDamage = 1,
                    .damageDiceCount = 2,
                    .damageDiceSides = 6,
                    .xpReward = 22,
                    .monsterAC = 16,
                    .attackMessage = "Endrer IP-adressen din i skjul, så angrepene dine treffer deg selv i stedet!",
                    .minDataReward = 700,
                    .maxDataReward = 1500,
                    .lootTable = {
                        { getItem("defekt strømforsyning"), 35 },
                        { getItem("løs side (nmap)"), 15 },
                        { getItem("løs side (ssh)"), 12 }
                    }
                },
                {
                    .name = "The COBOL Leviathan",
                    .description = "Et eldgammelt monster dekket av hullkort og støv. Ingen nålevende personer forstår hva det er.",
                    .level = 3,
                    .hp = 50,
                    .maxHp = 50,
                    .attackDamage = 1,
                    .damageDiceCount = 2,
                    .damageDiceSides = 4,
                    .xpReward = 35,
                    .monsterAC = 17,
                    .weaknesses = {"lightning", "acid"},
                    .attackMessage = "Brøler et uforståelig feilsøkings-dump fra 1983 og lammer deg med nostalgi!",
                    .minDataReward = 1000,
                    .maxDataReward = 2500,
                    .lootTable = {
                        { getItem("usb-minnepinne merket 'ikke rør'"), 30 },
                        { getItem("kevlar-forsterket laptopsekk"), 20 },
                        { getItem("løs side (git revert)"), 12 }
                    }
                }
            }},

            // ================= LEVEL 4 =================
            { 4, {
                {
                    .name = "Bobby Tables (The Dropper of Databases)",
                    .description = "En beryktet skikkelse som bærer på et skilt hvor det står: '; DROP TABLE Monsters;--'.",
                    .level = 4,
                    .hp = 50,
                    .maxHp = 50,
                    .attackDamage = 3,
                    .damageDiceCount = 2,
                    .damageDiceSides = 8,
                    .xpReward = 50,
                    .monsterAC = 18,
                    .weaknesses = {"sanitize"},
                    .attackMessage = "Kaster en SQL-injection som sletter vitale deler av dine essensielle funksjoner!",
                    .isUniqueBoss = true,
                    .minDataReward = 2000,
                    .maxDataReward = 4000,
                    .lootTable = {
                        { getItem("løs side (ctrl+z)"), 100 },
                        { getItem("brannmur-kappe (enterprise firewall)"), 35 }
                    }
                }
            }},
            // ================= LEVEL 5 =================
            { 5, {
                {
                    .name = "The Deadline Monster",
                    .description = "En kolossal, tikkende skygge som blir større og rødere for hvert sekund som går. Den har en hær av uleste e-poster og kaffekopper rundt seg.",
                    .level = 5,
                    .hp = 150,
                    .maxHp = 150,
                    .attackDamage = 4,
                    .damageDiceCount = 4,
                    .damageDiceSides = 6,
                    .xpReward = 225,
                    .monsterAC = 18,
                    .isUniqueBoss = true,
                    .attackMessage = "Kaster 'ASAP-panikk' over deg, spiser opp nattesøvnen din, og tvinger deg til å rulle ut uferdig kode i produksjon!",
                    .lootTable = {
                        { getItem("produksjonsnøklene"), 100 },
                        { getItem("gammel smultring"), 5 }
                    }
                }
            }}
        };
    return db;
    }
} // namespace

monsterManual::monsterManual()
{
    std::random_device rd;
    rng = std::mt19937(rd());
}

std::unique_ptr monsterManual::getMonsterBlueprint(int level, const std::string &name)
{
    const auto &db = getMonsterDatabase();
    auto it = db.find(level);
    if (it != db.end())
    {
        std::string wanted = toLowerNorwegian(name);
        for (const auto &m : it->second)
        {
            if (toLowerNorwegian(m.name) == wanted)
            {
                return std::make_unique(m);
            }
        }
    }
    return nullptr;
}

std::unique_ptr monsterManual::getRandomMonster(int level)
{
    const auto &db = getMonsterDatabase();
    auto it = db.find(level);
    if (it == db.end() || it->second.empty())
    {
        return nullptr;
    }
    // Filtrer ut unike bosser slik at de bare dukker opp der de skal
    std::vector<const monster *> pool;
    for (const auto &m : it->second)
    {
        if (!m.isUniqueBoss)
        {
            pool.push_back(&m);
        }
    }
    if (pool.empty())
    {
        return nullptr;
    }
    std::uniform_int_distribution<size_t> dist(0, pool.size() - 1);
    return std::make_unique(*pool[dist(rng)]);
}