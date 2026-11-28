#include "Item.h"
#include "Utils.h"
#include <iostream>

namespace {
    const std::vector<Recipe> recipeDatabase = {
        {
            .ingredient1 = "løs stålfjær",
            .ingredient2 = "gammel cat5-kabel",
            .resultItem = "piskende patchkabel mk.ii",
            .craftText = "Du tvinner stålfjæren rundt nettverkskabelen for ekstra piskesnert. Det knitrer av statisk elektrisitet!"
        },
        {
            .ingredient1 = "inntørket vestlandslefse",
            .ingredient2 = "gammel smultring",
            .resultItem = "kastbar kanel-murstein",
            .craftText = "Du presser den steinharde lefsen og den fossilerte smultringen sammen. Resultatet har samme egenvekt som en murstein."
        },
        {
            .ingredient1 = "rød monster",
            .ingredient2 = "hvit monster",
            .resultItem = "overklokket monster-brygg",
            .craftText = "Du heller den røde og den hvite Monsteren sammen i én boks. Blandingen bruser illevarslende som en overbelastet kjøleribbe."
        },
        {
            .ingredient1 = "defekt strømforsyning",
            .ingredient2 = "usb-minnepinne merket 'ikke rør'",
            .resultItem = "overklokket usb-killer",
            .craftText = "Du lodder ledningene fra strømforsyningen rett inn på USB-pinnen med ren viljestyrke. Den lukter svidd ozon!"
        }
    };

    // ITEMS ITEMS ITEMS ITEMS ITEMS ITEMS ITEMS ITEMS ITEMS ITEMS ITEMS

    const std::vector<item> itemDatabase = {
        // ====== VÅPEN ======
        {
            .name = "kaffekopp",
            .description = "En mye brukt kaffekopp som noen har glemt igjen. Innsiden er misfarget av gammel kaffe. På utsiden står teksten 'There are 10 types of people in the world. Those who can read binary, and those who cannot.'",
            .value = 2,
            .currency = "kB",
            .type = "weapon",
            .damageDiceSides = 2,
            .damageDiceCount = 1,
            .damageType = "Bludgeoning",
            .extraDiceSides = 4,
            .extraDiceCount = 1,
            .extraDamageType = "acid",
            .isFilled = false,
            .throwable = true,
            .isContainer = true,
            .throwFilledText = "PLASK! Koppen var full av glovarm kontorkaffe!",
            .throwEmptyText = "Koppen var tom og treffer med et tørt 'KLONK'!",
            .throwAcReduction = 2,
            .throwAcReductionText = "Gulvet blir såpeglatt av kaffesølet!",
            .hazardName = "caltrops",
            .hazardDiceCount = 1,
            .hazardDiceSides = 4,
            .hazardDamageType = "piercing",
            .hazardCharges = 3,
            .breakChancePercent = 80,
            .spawnsHazardOnThrow = true,
        },
        {
            .name = "penn",
            .description = "En vanlig BIC-penn som har tørket inn. Improvisert stikkvåpen (1d4 skade).",
            .value = 1,
            .currency = "MB",
            .type = "weapon",
            .damageDiceSides = 4,
            .damageDiceCount = 1,
            .damageType = "Piercing",
            .throwable = false
        },
        {
            .name = "mekanisk tastatur",
            .description = "Et blytungt tastatur med Cherry MX Blue-brytere. Skaden er både fysisk og psykisk.",
            .value = 3,
            .currency = "MB",
            .type = "weapon",
            .damageDiceSides = 6,
            .damageDiceCount = 1,
            .damageType = "Bludgeoning",
            .extraDiceSides = 4,
            .extraDiceCount = 1,
            .extraDamageType = "sonic"
        },
        {
            .name = "gammel cat5-kabel",
            .description = "En 5 meter lang nettverkskabel med knekt klips. Gjør 1d6 slashing-skade og gir +2 mot utdaterte rutere. Weapon (whip).",
            .value = 2,
            .currency = "MB",
            .type = "weapon",
            .damageDiceSides = 6,
            .damageDiceCount = 1,
            .damageType = "Slashing",
            .extraDiceSides = 0,
            .extraDiceCount = 0,
            .extraDamageType = ""
        },
        {
            .name = "piskende patchkabel mk.ii",
            .description = "En gammel Cat5-kabel viklet rundt en stålfjær for ekstra snert. Gir en ubehagelig statisk utladning ved treff.",
            .value = 10,
            .currency = "MB",
            .type = "weapon",
            .damageDiceSides = 8,
            .damageDiceCount = 1,
            .damageType = "Slashing",
            .extraDiceSides = 4,
            .extraDiceCount = 1,
            .extraDamageType = "lightning"
        },
        {
            .name = "kastbar kanel-murstein",
            .description = "En inntørket vestlandslefse og en forsteinet smultring presset sammen til ett kompakt prosjektil. Utgått på dato i begge endene.",
            .value = 2,
            .currency = "MB",
            .type = "weapon",
            .damageDiceSides = 6,
            .damageDiceCount = 2,
            .damageType = "Bludgeoning",
            .throwable = true
        },
        {
            .name = "defekt strømforsyning",
            .description = "En blytung PSU fra en stasjonær PC fra 2012. Kondensatorene buler illevarslende. Perfekt som kastevåpen eller til å tappe strøm fra.",
            .value = 3,
            .currency = "MB",
            .type = "weapon",
            .damageDiceSides = 6,
            .damageDiceCount = 1,
            .damageType = "Bludgeoning",
            .throwable = true
        },
        {
            .name = "usb-minnepinne merket 'ikke rør'",
            .description = "En slitt 4GB minnepinne funnet teipet fast under en server. Ingen vet hva som er på den, men den blir varm når du holder den.",
            .value = 10,
            .currency = "MB",
            .type = "crafting",
            .isQuestItem = false
        },
        {
            .name = "overklokket usb-killer",
            .description = "Et livsfarlig, engangs cyber-våpen.",
            .type = "weapon",
            .damageDiceSides = 4,
            .damageDiceCount = 1,
            .damageType = "Bludgeoning",
            .extraDiceSides = 12,
            .extraDiceCount = 2,
            .extraDamageType = "Lightning",
            .throwable = true,
            .throwHitText = "BZZZT! USB-pinnen treffer og utløser en kraftig kortslutning!",
            .destroyOnThrow = true
        },
        {
            .name = "deodorant og lighter",
            .description = "En halvfull boks Axe Dark Temptation og en reklame-lighter fra et IT-seminar. Improvisert flammekaster som garantert bryter HMS-reglementet.",
            .value = 8,
            .currency = "MB",
            .type = "weapon",
            .damageDiceSides = 6,
            .damageDiceCount = 2,
            .damageType = "Fire",
            .throwable = false
        },
        {
            .name = "boks med trykkluft",
            .description = "Til å blåse støv ut av tastaturer. Iskald om du snur den opp-ned.",
            .type = "weapon",
            .damageDiceSides = 4,
            .damageDiceCount = 1,
            .damageType = "Cold",
            .throwable = true,
            .throwHitText = "PSSSSJH! En sky av iskald gass dekker sensorene til fienden!",
            .throwAcReduction = 3,
            .throwAcReductionText = "Fienden er midlertidig blendet av frosttåke!",
            .destroyOnThrow = true
        },

        // ====== CONSUMABLES ======
        {
            .name = "rød monster",
            .description = "En lunken boks med sukker of koffein. Gir deg ny giv.",
            .value = 75,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::healHp,
            .effectDiceSides = 8,
            .effectDiceCount = 1,
            .effectFlatBonus = 2,
            .consumeText = "Du spretter den lunkne boksen. Den smaker som flytende sukkerspinn blandet med overtid, og gir deg akkurat nok viljestyrke til å overleve ett sprintmøte til."
        },
        {
            .name = "grønn monster",
            .description = "Originalen. Lyser svakt radioaktivt i mørket. Gjenoppretter 2d4 +2 HP.",
            .value = 40,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::healHp,
            .effectDiceSides = 4,
            .effectDiceCount = 2,
            .effectFlatBonus = 2,
            .consumeText = "Du spretter boksen. *PSST!* Den syntetiske smaken av batterisyre og eple sender nytt liv gjennom årene dine."
        },
        {
            .name = "hvit monster",
            .description = "Monster Ultra. Sukkerfri og utviklernes foretrukne drivstoff. Gjenoppretter 2d6 Mana.",
            .value = 50,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::restoreMana,
            .effectDiceSides = 6,
            .effectDiceCount = 2,
            .effectFlatBonus = 0,
            .consumeText = "Du tar en slurk iskald hvit Monster. Hjernetåken letter og kodestrukturen blir krystallklar!"
        },
        {
            .name = "mango loco",
            .description = "En tropisk eksplosjon av sukker og koffein. Gir deg både HP (1d6) og en midlertidig koffein-buff!",
            .value = 75,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::caffeineRush,
            .effectDiceSides = 6,
            .effectDiceCount = 1,
            .effectFlatBonus = 2,
            .consumeText = "Du styrter en Mango Loco. Pulsen skyter i været, øynene dirrer og du føler deg ustoppelig!"
        },
        {
            .name = "gammel smultring",
            .description = "Lå igjen etter fredagskosen for to uker siden. Glasur med tvilsom farge.",
            .value = 5,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::sugarCrash,
            .effectDiceSides = 4,
            .effectDiceCount = 1,
            .effectFlatBonus = 0,
            .consumeText = "Du tar en bit. Den knaser på feil steder. Magen din angrer umiddelbart."
        },
        {
            .name = "inntørket vestlandslefse",
            .description = "Umulig å anslå hvor lenge denne har ligget her. Du kan ikke huske sist det ble servert vestlandslefser til kaffekosen.",
            .value = 8,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::sugarCrash, //Hmm, må kanskje heller ha en "poisoned" effect. 1d4 dmg over 3 runder eller noe slikt - det føles ikke riktig at denne skal gi en buff
            .effectDiceSides = 4,
            .effectDiceCount = 1,
            .effectFlatBonus = 0,
            .consumeText = "Du tar en bit. Den er tørrere enn en 400-siders RFC-spesifikasjon, og kanelstøvet støver ut av munnen din som en gammel servervifte. Magen gjør opprør."
        },
        {
            .name = "kald filterkaffe",
            .description = "Har stått på kannen siden lunsj i går. Gir 1d4 HP, men tvinger deg til å ta en 'constitution save' mot halsbrann.",
            .value = 5,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::healHp,
            .effectDiceSides = 4,
            .effectDiceCount = 1,
            .effectFlatBonus = 0,
            .consumeText = "Du tar en slurk av den bekmørke, lunkne væsken. Det svir nedover spiserøret som flytende kompilatorfeil, men du våkner i det minste til."
        },
        {
            .name = "overklokket monster-brygg",
            .description = "En tvilsom blanding av rød og hvit Monster. Væsken vibrerer svakt og hjertet ditt slår allerede i 144 Hz bare av å se på den.",
            .value = 150,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::caffeineRush,
            .effectDiceSides = 8,
            .effectDiceCount = 2,
            .effectFlatBonus = 2,
            .consumeText = "Du styrter det overklokkede brygget. Tiden ser ut til å gå i sakte film mens pulsen treffer taket!"
        },
        {
            .name = "lunken kaffe fra i går",
            .description = "En halvfull pappkopp som har stått på en pult siden forrige sprint. Smaker som flytende anger, men gir 1d4 Mana.",
            .value = 10,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::restoreMana,
            .effectDiceSides = 4,
            .effectDiceCount = 1,
            .effectFlatBonus = 1,
            .consumeText = "Du tvinger i deg den kalde gruten. Den smaker brent gummi, men hjernen våkner motvillig til liv."
        },
        {
            .name = "halvspist energibar",
            .description = "Smak av sjokolade og eksistensiell overtid. Gjenoppretter 1d6 + 1 HP.",
            .value = 25,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::healHp,
            .effectDiceSides = 6,
            .effectDiceCount = 1,
            .effectFlatBonus = 1,
            .consumeText = "Den er seig som viskelær, men gir deg akkurat nok kalorier til å fortsette."
        },
        {
            .name = "veteranens nødtermos",
            .description = "En militærgrønn termos fra 1994 med en bulk på siden og et klistremerke: «FEIL = FUNKSJON». Gjenoppretter 3d4 + 4 Mana.",
            .value = 2, 
            .currency = "MB", 
            .type = "consumable",
            .effect = itemEffect::restoreMana,
            .effectDiceSides = 4, 
            .effectDiceCount = 3, 
            .effectFlatBonus = 4,
            .consumeText = "Du skrur av lokket. Dampen smaker av 1994, brent bønne og ren sta. Hodet klarner som om noen har tatt en full backup av hjernen din."
        },

        // ====== ARMOUR / SHIELDS ======
        // Light armour
        {
            .name = "Utslitt eventhettegenser",     // +1AC
            .description = "En falmet hettegenser fra et hackathon i 2019. Demper de verste slagene og skjuler deg for prosjektledere.",
            .value = 1,
            .currency = "MB",
            .type = "armour",
            .acBonus = 1,
            .equipSlot = 2          // 1 = head, 2 = body, 3 = arms, 4 = legs, 5 = Off hand shield
        },
        {
            .name = "Støydempende headsett",        // +1AC
            .description = "Sony WH-1000XM5 med aktiv støykansellering. Du hører ikke angrepene, så de gjør beviselig mindre vondt.",
            .value = 1500,
            .currency = "kB",
            .type = "armour",
            .acBonus = 1,
            .equipSlot = 1
        },
        {
            .name = "Blålysbriller",                // +2AC
            .description = "Beskytter øynene mot skarpe skjermer og passiv-aggressive Slack-meldinger.",
            .value = 750,
            .currency = "kB",
            .type = "armour",
            .acBonus = 2,
            .equipSlot = 1
        },
        {
            .name = "Ørepropper i skum",            // +1AC
            .description = "Oransje engangspropper. Demper lyden av både scrum-masters og kolleger som tygger eple.",
            .value = 300,
            .currency = "kB",
            .type = "armour",
            .acBonus = 1,
            .equipSlot = 1
        },
        {
            .name = "taktisk cargobukse",
            .description = "En grå turbukse med åtte lommer fulle av kvitteringer, adaptere og gamle kaffekort. Gir +1 AC og uforklarlig selvtillit under driftstans.",
            .value = 2,
            .currency = "MB",
            .type = "armour",
            .acBonus = 1,
            .equipSlot = 4
        },

        //Medium armour
        {
            .name = "Anti-statisk armlenke",        // +2AC
            .description = "Koblet til jord. Jorder elektriske støt og dårlige vibber fra serverrommet.",
            .value = 4,
            .currency = "MB",
            .type = "armour",
            .acBonus = 2,
            .equipSlot = 3
        },
        {
            .name = "Kevlar-forsterket laptopsekk", // +2AC
            .description = "Den klassisk sekken alle konsulenter bærer på ryggen. Stopper det meste som kommer bakfra.",
            .value = 6,
            .currency = "MB",
            .type = "armour",
            .acBonus = 2,
            .equipSlot = 2
        },
        {
            .name = "Laminert GDPR-skjold",         // +2AC - off hand shield
            .description = "Et tungt, innbundet eksemplar av personvernforordningen. Får enhver angriper til å nøle av ren byråkratisk frykt.",
            .value = 5,
            .currency = "MB",
            .type = "shield",
            .acBonus = 2,
            .equipSlot = 5
        },

        // Heavy armour
        {
            .name = "Brannmur-kappe (Enterprise Firewall)", // +4AC
            .description = "En tung kappe vevd av optiske fiberkabler og strenge tilgangslister. Blokkerer 99.9% av uautoriserte treff.",
            .value = 12,
            .currency = "MB",
            .type = "armour",
            .acBonus = 4,
            .equipSlot = 2
        },
        {
            .name = "Zero-Trust Rustning",                  // +5AC
            .description = "En heldekkende drakt som krever tofaktor autentisering for hvert eneste sverdslag som prøver å trenge gjennom.",
            .value = 25,
            .currency = "MB",
            .type = "armour",
            .acBonus = 5,
            .equipSlot = 2
        },
        {
            .name = "Air-Gapped Serverkabinett",            // +6AC
            .description = "Du har bokstavelig talt tredd et 19-tommers stål-rack over overkroppen. Tungt som bly, men fullstendig immun mot nettverksangrep.",
            .value = 45,
            .currency = "MB",
            .type = "armour",
            .acBonus = 6,
            .equipSlot = 2
        },

        // ====== TRINKETS ======
        {
            .name = "liten gummiand",
            .description = "En lommeutgave av den klassiske gule gummianden. Den piper svakt og dømmende når du klemmer på den, men hjelper deg å tenke klart når koden nekter å kompilere.",
            .value = 500,
            .currency = "kB",
            .type = "trinket",
            .isQuestItem = false
        },

        // ====== MISC ======
                {
            .name = "kopipapir",            //Til 'PC LOAD LETTER' puzzle: reward = new spell for spellbook
            .description = "En uåpnet pakke med 500 ark A4-papir (80g). Den hellige gral for enhver kontorskriver som roper 'PC LOAD LETTER'.",
            .value = 500,
            .currency = "kB",
            .type = "misc",
            .isQuestItem = false            //Denne er missable
        },

        // ====== CRAFTING ITEMS ======

        {
            .name = "løs stålfjær",
            .description = "En stram stålfjær du pirket løs fra en slitt lenestol. Den har overraskende mye spenn i seg ... perfekt for kreative ingeniørprosjekter.",
            .value = 1,
            .currency = "MB",
            .type = "crafting",
            .isQuestItem = false
        },


        // ====== QUEST / MAGIC ======
        {
            .name = "necronomicon manualis",
            .description = "En tykk, skinnbundet bok med knekt rygg og kaffeflekker på hvert eneste oppslag. På forsiden står det innprentet med falmet gullskrift: «UNIX Programmer’s Manual, Bell Labs, 1971». Sidene er fylt med små, tette bokstaver og merkelige symboler, og margene er full av håndskrevne notater fra utviklere som for lengst har sluttet i firmaet. Boken åpner seg alltid på siden du ikke trenger. Det sies at ingen noensinne har lest den ferdig, og at de som påstår det lyver.",
            .value = 0,
            .currency = "kB",
            .type = "spellbook",
            .isQuestItem = true
        },
        {
            .name = "adgangskort",
            .description = "Et hvitt plastkort med et falmet bilde av en utvikler som så mye gladere ut på ansettelsesdagen.",
            .value = 0,
            .currency = "kB",
            .type = "key",
            .isQuestItem = true
        },
        {
            .name = "produksjonsnøklene",
            .description = "Et tungt knippe med krypterte USB-nøkler og et laminert kort merket 'ROOT ACCESS - PROD'. Med disse kan du endelig rulle ut fiksen og redde dagen.",
            .value = 0,
            .currency = "kB",
            .type = "key",
            .isQuestItem = true
        },
        {
            .name = "post-it med passord",
            .description = "En gul lapp funnet under et tastatur. Det står 'Sommer2026!' med kulepenn.",
            .value = 0,
            .currency = "kB",
            .type = "password",
            .isQuestItem = true
        },
        {

            .name = "the mythical man-month",
            .description = "Første utgave, 1975. Ryggen er teipet med gaffateip, og margene er fulle av håndskrevne notater i blå kulepenn, hvert av dem avsluttet med «Jeg sa det.» Mellom side 16 og 17 ligger en kaffeflekk fra 1994.",
            .value = 0,
            .currency = "kB",
            .type = "quest",
            .isQuestItem = true
        },

        // ====== SPELLBOOK PAGES ======

        {
            .name = "løs side (grep)",
            .description = "Et utrevet ark fra en gammel manual, fullt av margnotater i tre forskjellige håndskrifter.",
            .value = 256,
            .currency = "kB",
            .type = "spellpage",
            .teachesSpell = "grep"
        },
        {
            .name = "løs side (ping)",
            .description = "Et ark med to små datamaskiner som vinker til hverandre over en stiplet linje. Noen har skrevet «svarer ikke?» i margen og strøket det ut tre ganger.",
            .value = 256, 
            .currency = "kB", 
            .type = "spellpage", 
            .teachesSpell = "ping"
        },
        {
            .name = "løs side (ls -la)",
            .description = "En filliste skrevet ut på papir, så lang at den har rullet seg sammen av seg selv. Nederst står det: «Ingenting er skjult for den som spør pent.»",
            .value = 256, 
            .currency = "kB", 
            .type = "spellpage", 
            .teachesSpell = "ls -la"
        },
        {
            .name = "løs side (git blame)",
            .description = "En commit-logg på papir, med ett enkelt navn understreket i rød tusj. I margen har noen tegnet en pil og skrevet: «DET VAR DEG.»",
            .value = 512, 
            .currency = "kB", 
            .type = "spellpage", 
            .teachesSpell = "git blame"
        },
        {
            .name = "løs side (git stash)",
            .description = "Et ark merket «Midlertidig. Ryddes opp i senere.» Papiret har tydeligvis ligget i en skuff siden 2019, og ingen har ryddet opp i noe som helst.",
            .value = 1, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "git stash"
        },
        {
            .name = "løs side (sudo)",
            .description = "En gul lapp med én setning: «Du er ikke i sudoers-filen. Dette vil bli rapportert.» Under står det, med blyant: «Ikke hvis du sier det riktig.»",
            .value = 2, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "sudo"
        },
        {
            .name = "løs side (ssh)",
            .description = "Et ark med en liten nøkkel tegnet i margen og en liste over servere ingen har tilgang til lenger. Nederst står det: «Når du ikke vet hvor du skal, er alle steder trygge steder.»",
            .value = 2, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "ssh"
        },
        {
            .name = "løs side (chmod 777)",
            .description = "En side fra en gammel sikkerhetsmanual, med ordet ADVARSEL understreket fire ganger. Under står tallet 777, og noen har tegnet et smilefjes ved siden av.",
            .value = 3, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "chmod 777"
        },
        {
            .name = "løs side (ctrl+z)",
            .description = "Et rotete utkast der alt er strøket over, og så strøket under igjen. I margen står det, med stadig større skrift: «Angre. Angre! ANGRE!»",
            .value = 3, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "ctrl+z"
        },
        {
            .name = "løs side (kill -9)",
            .description = "Et ark med ett eneste ord, SIGKILL, skrevet med så hard hånd at pennen har revet hull i papiret. Det lukter svakt av svidd prosess.",
            .value = 4, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "kill -9"
        },
        {
            .name = "løs side (rm -rf /)",
            .description = "Et ark stemplet ALDRI KJØR DENNE i rødt. Tre generasjoner sysadmins har brettet hjørnet på siden likevel. Sekken din føles litt lettere bare av å holde den.",
            .value = 8, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "rm -rf /"
        },
        {
            .name = "løs side (git push --force)",
            .description = "En kaffeflekket side revet ut av en retrospektiv-rapport. «Aldri deploy på en fredag» står det, og under: «Gjort det. Fredag. Aldri igjen.»",
            .value = 16, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "git push --force"
        },
        { 
            .name = "løs side (iptables -j drop)",
            .description = "Et ark med en murvegg tegnet i margen og én regel: «Alt som ikke er tillatt, er forbudt.» Noen har føyd til: «Inkludert deg.»",
            .value = 1, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "iptables -j drop" 
        },
        { 
            .name = "løs side (nmap)",
            .description = "En skriveroppgave full av IP-adresser, med to av dem sirklet inn og «hvorfor svarer DEN?» skrevet ved siden av.",
            .value = 1, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "nmap" 
        },
        { 
            .name = "løs side (chroot)",
            .description = "Et ark brettet til en liten boks. Det står «IKKE ÅPNE» på innsiden av lokket. Utenpå står det ingenting.",
            .value = 6, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "chroot" 
        },
        { 
            .name = "løs side (sudo make me a sandwich)",
            .description = "En tegneserierute revet ut av en bok. Den ene personen sier: «Lag en sandwich.» Den andre: «Lag den selv.» Så: «sudo.» Så: «Okay.»",
            .value = 4, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "sudo make me a sandwich" 
        },
        { 
            .name = "løs side (locate)",
            .description = "En utskrift av en søkeindeks. Nederst: «Sist oppdatert: for lenge siden.» Den lukter støv og gammel toner.",
            .value = 4, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "locate" 
        },
        { 
            .name = "løs side (git revert)",
            .description = "En side fra en retrospektiv, der noen har skrevet «Vi skulle ha reverta på mandag» med stadig større bokstaver.",
            .value = 12, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "git revert" 
        },
        { 
            .name = "løs side (fork bomb)",
            .description = "Et ark med bare tretten tegn skrevet midt på. Papiret har blitt litt varmt, og det vokser stadig flere identiske ark ut av stabelen.",
            .value = 12, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "fork bomb" 
        },
        { 
            .name = "løs side (rubber duck debugging)",
            .description = "En side med en tegning av en gummiand og teksten: «Den dømmer deg ikke. Den finner bare feilen mens du snakker.»",
            .value = 10, 
            .currency = "MB", 
            .type = "spellpage", 
            .teachesSpell = "rubber duck debugging" 
        },
        {
            .name = "løs side (regex sanitize)",
            .description = "Et krøllete ark dekket av kryptiske tegn som ^[a-zA-Z0-9_]+$. I margen står det med rød tusj: 'ALDRI stol på input fra brukeren – spesielt ikke hvis han heter Bobby.'",
            .value = 6,
            .currency = "MB",
            .type = "spellpage",
            .teachesSpell = "regex sanitize"
        },

        // ====== ARKIV-SKROT ======
        { 
            .name = "hullkort", 
            .description = "Et hullkort med et mønster som sikkert betyr noe. Alle som kunne lese det er pensjonert, flyttet til hytta, eller begge deler.",
            .value = 40, 
            .currency = "kB", 
            .type = "junk" 
        },
        { 
            .name = "dokumentmappe", 
            .description = "En forvitret mappe merket «final_v2_NY_ENDELIG». Inni ligger ett ark med teksten «Se vedlegg».",
            .value = 30, 
            .currency = "kB", 
            .type = "junk" 
        },
        { 
            .name = "diskett", 
            .description = "Rommer 1,44 MB. Etiketten sier «PRIVAT – IKKE SLETT». Du er ganske sikker på at den ikke har vært lesbar siden Clinton.",
            .value = 300, 
            .currency = "kB", 
            .type = "junk" 
        },
        { 
            .name = "zip-disk", 
            .description = "En 100 MB zip-disk merket «Click of Death». Å sette den i en maskin er en beslutning du bare angrer på én gang.",
            .value = 1, 
            .currency = "MB", 
            .type = "junk" 
        },
        { 
            .name = "stiftemaskin", 
            .description = "En rusten stiftemaskin i tung stål. Den har holdt sammen flere prosjekter enn noen prosjektleder.",
            .value = 25, 
            .currency = "kB", 
            .type = "junk" 
        },
        { 
            .name = "ansattkort", 
            .description = "Et utløpt ansattkort fra 1999. Bildet viser en smilende mann med bart. Navnet er overstrøket, og noen har skrevet «IKKE LENGER ANSATT (LYKKE TIL)».",
            .value = 80, 
            .currency = "kB", 
            .type = "junk" 
        },
        { 
            .name = "arkivperm", 
            .description = "En tung perm merket «KUN FOR INTERNT BRUK». Den lander med et dunk som kan høres i tre etasjer. Perfekt til kasting.",
            .value = 120, 
            .currency = "kB", 
            .type = "junk",
            .damageDiceSides = 6, 
            .damageDiceCount = 1, 
            .damageType = "bludgeoning",
            .throwable = true,
            .throwHitText = "Permen treffer med et dumpt «THUD», og en liten sky av støv og utdaterte rutiner stiger opp." 
        },
        { 
            .name = "telefonkatalog", 
            .description = "Oslo, 1988. Tung, støvete og full av folk som ikke bor der lenger. Egner seg bedre som slagvåpen enn som oppslagsverk.",
            .value = 250, 
            .currency = "kB", 
            .type = "junk",
            .damageDiceSides = 8, 
            .damageDiceCount = 1, 
            .damageType = "bludgeoning",
            .throwable = true,
            .throwHitText = "Katalogen treffer med et dunk som slår luften ut av alle som noen gang har ønsket seg et telefonnummer." 
        },
        { 
            .name = "makuleringspose", 
            .description = "En klar plastpose full av remser. Med tid og tålmodighet kunne du nesten satt sammen igjen hva som sto der. Det har du ikke.",
            .value = 20, 
            .currency = "kB", 
            .type = "junk" 
        },
        { 
            .name = "vhs-kassett", 
            .description = "Merket «Opplæring i brannsikkerhet (1992)». Du føler deg plutselig mistenkelig skyldig i noe.",
            .value = 200, 
            .currency = "kB", 
            .type = "junk" 
        },
        { 
            .name = "sikkerhetskopi", 
            .description = "En båndkassett merket «BACKUP (IKKE SLETT)». Datoen er 2009. Ingen vet hva som ligger på den, og ingen tør å spørre.",
            .value = 500, 
            .currency = "kB", 
            .type = "junk" 
        },
        { 
            .name = "gummistempel", 
            .description = "Et gummistempel med teksten «GODKJENT». Trolig det mektigste våpenet i enhver byråkratisk organisasjon.",
            .value = 400, 
            .currency = "kB", 
            .type = "junk" 
        },
        { 
            .name = "organisasjonskart", 
            .description = "Et kaffeflekket organisasjonskart der alle rapporterer til seg selv. Øverst står det «DIREKTØR». Under står det «DIREKTØR». Så «DIREKTØR».",
            .value = 150, 
            .currency = "kB", 
            .type = "junk" 
        },
        { 
            .name = "post-it", 
            .description = "En gul lapp med teksten «Husk backup!». Skrevet i 2006. Ingen husket.",
            .value = 10, 
            .currency = "kB", 
            .type = "junk" 
        }
    };
}  // namespace

const std::vector<item>& itemList() {
    return itemDatabase;
}

bool itemExists(const std::string& name) {
    std::string wanted = toLowerNorwegian(name);
    for (const auto& it : itemDatabase) {
        if (toLowerNorwegian(it.name) == wanted) {
            return true;
        }
    }
    return false;
}

item getItem(const std::string& name) {
    std::string wanted = toLowerNorwegian(name);
    for (const auto& it : itemDatabase) {
        if (toLowerNorwegian(it.name) == wanted) {
            return it;
        }
    }

    std::cerr << "[DEBUG] Advarsel: Fant ikke item '" << name << "' i itemDatabase!\n";
    return item{
        .name = "undefined (" + name + ")",
        .description = "En merkelig glitch i virkeligheten. Noen har stavet et item-navn feil i koden.",
        .value = 0,
        .currency = "kB",
        .type = "junk"
    };
}

void inspectItem(const item& it) {
    std::cout << "\n=== " << toUpperNorwegian(it.name) << " ===\n";
    std::cout << it.description << "\n";

    //Stats basert på item-type
    if (it.type == "consumable") {
        std::cout << "\n[Næringsinnhold & Bivirkninger]\n";

        //Terninger
        std::string formula = std::to_string(it.effectDiceCount) + "d" + std::to_string(it.effectDiceSides);
        if (it.effectFlatBonus > 0) {
            formula += " + " + std::to_string(it.effectFlatBonus);
        }

        switch (it.effect) {
            case itemEffect::healHp:
                std::cout << "  * Helse-boost:    +" << formula << " HP\n";
                std::cout << "  * Koffein:        160mg (Godkjent av arbeidsmiljøloven)\n";
                break;

            case itemEffect::restoreMana:
                std::cout << "  * Fokus-boost:    +" << formula << " Mana\n";
                std::cout << "  * Sukker:         0 g (Ren utvikler-magi)\n";
                break;
            
            case itemEffect::caffeineRush:
                std::cout << "  * Energi-sjokk:   +" << formula << " HP + [Koffeinsjokk] (3 runder)\n";
                std::cout << "  * Advarsel:       Kan føre til ukontrollert refaktorering av produksjonskode.\n";
                break;

            case itemEffect::sugarCrash:
                std::cout << "  * Risiko:         -" << formula << " HP (Matforgiftning)\n";
                std::cout << "  * Utløpsdato:     Uleselig (trolig fra forrige kvartal)\n";
                break;

            default:
                std::cout << "  * Effekt:         Ukjent kjemisk sammensetning\n";
                break;
        }
    }
    else if (it.type == "weapon") {
        std::cout << "\n[Spesifikasjoner]\n";
        std::cout << "  * Skade:         " << it.damageDiceCount << "d" << it.damageDiceSides << " (" << it.damageType << ")\n";

        if (it.extraDiceCount > 0) {
            std::cout << "  * Bonusskade:    " << it.extraDiceCount << "d" << it.extraDiceSides << " (" << it.extraDamageType << ")";
            if (it.name == "kaffekopp" && !it.isFilled) {
                std::cout << " [Krever varm kaffe]";
            }
            std::cout << "\n";
        }
        if (it.throwable) {
            std::cout << "  * Egenskap:     Kastbar (engangs i kamp)\n";
        }
    }
    // Verdi og quest-status nederst
    if (it.isQuestItem) {
        std::cout << "  * Status:    Viktig gjenstand (kan ikke kastes/selges)\n";
    } else if (it.value > 0) {
        std::cout << "  * Markedsverdi:  " << it.value << " " << it.currency << "\n";
    }
    std::cout << "=========================\n";
}

const Recipe* findRecipe(const std::string& itemA, const std::string& itemB) {
    std::string a = toLowerNorwegian(itemA);
    std::string b = toLowerNorwegian(itemB);

    for (const auto& r : recipeDatabase) {
        std::string r1 = toLowerNorwegian(r.ingredient1);
        std::string r2 = toLowerNorwegian(r.ingredient2);

        if ((a == r1 && b == r2) || (a == r2 && b == r1)) {
            return &r;
        }
    }
    return nullptr;
}

bool fillContainer(item& it, const std::string& liquid) {
    if (!it.isContainer) return false;
    if (liquid == "kaffe") {
        it.isFilled = true;
        it.contents = "kaffe";
        it.description = "Fylt med lett svidd, glovarm, beksvart og etsende trakterkaffe! (1d2 bludgeoning + 1d4 acid).";
        it.extraDiceSides = 4,
        it.extraDiceCount = 1;
        it.extraDamageType = "Acid";
        return true;
    }
    if (liquid == "vann") {
        it.isFilled = true;
        it.contents = "vann";
        it.description = "Fylt med lunkent springvann.";
        it.extraDiceSides = 0;
        it.extraDiceCount = 0,
        it.extraDamageType = "";
        return true;
    }
    return false;
}

void emptyContainer(item& it) {
    const item base = getItem(it.name);
    it.isFilled = false;
    it.contents = base.contents;
    it.description = base.description;
    it.extraDiceSides = base.extraDiceSides;
    it.extraDiceCount = base.extraDiceCount;
    it.extraDamageType = base.extraDamageType;
}