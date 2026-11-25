#include "Monster.h"
#include "Item.h"

monsterManual::monsterManual() {
    // Starter RNG (Mersenne Twister)
    std::random_device rd;
    rng = std::mt19937(rd());

    // MONSTEROVERSIKT - "blueprints"
    // LEVEL 1
    blueprintDatabase[1].push_back({
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
            { getItem("rød monster"), 75 },
            { getItem("gammel smultring"), 50 }
        }
    });
    blueprintDatabase[1].push_back({
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
        .attackMessage = "Spammer deg med 'Godta alle' -knapper til du mister oversikten!",
        .minDataReward = 150,
        .maxDataReward = 450,
        .lootTable = {
            { getItem("mango loco"), 75 },
            { getItem("gammel smultring"), 50 }
        }
    });
    blueprintDatabase[1].push_back({
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
            { getItem("rød monster"), 75 },
            { getItem("gammel smultring"), 50 }
        }
    });

    // LEVEL 2
    blueprintDatabase[2].push_back({
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
        .attackMessage = "Holder en endeløs monolog og påfører deg 'Feature Creep' -skade over tid!",
        .minDataReward = 400,
        .maxDataReward = 1000,
        .lootTable = {
            { getItem("rød monster"), 75 },
            { getItem("gammel smultring"), 50 }
        }
    });
    blueprintDatabase[2].push_back({
        .name = "The Jira Ticket Overlord",
        .description = "Et monster laget av post-it-lapper og uleste varlser. Den flytter deg konstant mellom 'In Progress' og 'To Do'.",
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
            { getItem("rød monster"), 75 },
            { getItem("gammel smultring"), 50 }
        }
    });
    blueprintDatabase[2].push_back({
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
        .weaknesses = {"lightning"},
        .attackMessage = "Hopper til en uallokert minneadresse og kaster en 'Segmentation Fault (core dumped)' rett i brystet ditt!",
        .minDataReward = 500,
        .maxDataReward = 1300,
        .lootTable = {
            { getItem("rød monster"), 75 },
            { getItem("gammel smultring"), 50 }
        }
    });

    // LEVEL 3
    blueprintDatabase[3].push_back({
        .name = "The Retention Policy",
        .description = "A paper minotaur...",
        .level = 3,
        .hp = 55,
        .maxHp = 55,
        .attackDamage = 2,
        .damageDiceCount = 2,
        .damageDiceSides = 8,
        .xpReward = 22,
        .monsterAC = 15,
        .weaknesses = {"fire", "acid"},
        .attackMessage = "",
        .minDataReward = 600,
        .maxDataReward = 1400,
        .lootTable = {
            { getItem("the mythical man-month"), 100}
        }
    });
    blueprintDatabase[3].push_back({
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
            { getItem("rød monster"), 75 },
            { getItem("gammel smultring"), 50 }
        }
    });
    blueprintDatabase[3].push_back({
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
            { getItem("rød monster"), 75 },
            { getItem("gammel smultring"), 50 }
        }
    });

    // LEVEL 4
    blueprintDatabase[4].push_back({
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
        .attackMessage = "Kaster en SQL-injection som sletter vitale deler av dine essensielle funksjoner!",
        .minDataReward = 2000,
        .maxDataReward = 4000,
        .lootTable = {
            { getItem("rød monster"), 75 },
            { getItem("gammel smultring"), 50 }
        }
    });

    // LEVEL 5
    blueprintDatabase[5].push_back({
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
        .attackMessage = "Kaster 'ASAP-panikk' over deg, spiser opp nattesøvnen din, og tvinger deg til å rulle ut uferdig kode i produksjon!",
        .lootTable = {
            { getItem("produksjonsnøklene"), 100 },
            { getItem("gammel smultring"), 5 }
        }
    });
}

std::unique_ptr<monster> monsterManual::getMonsterBlueprint(int level, const std::string& name) {
    if (blueprintDatabase.find(level) != blueprintDatabase.end()) {
        for (const auto& m : blueprintDatabase[level]) {
            if (m.name == name) {
                return std::make_unique<monster>(m);
            }
        }
    }
    return nullptr;
}

std::unique_ptr<monster> monsterManual::getRandomMonster(int level) {
    if (blueprintDatabase.find(level) != blueprintDatabase.end() && !blueprintDatabase[level].empty()) {
        std::uniform_int_distribution<size_t> dist(0, blueprintDatabase[level].size() - 1);
        size_t randomIndex = dist(rng);
        return std::make_unique<monster>(blueprintDatabase[level][randomIndex]);
    }
    return nullptr;
}