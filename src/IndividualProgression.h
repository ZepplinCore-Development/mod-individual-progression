#ifndef AZEROTHCORE_INDIVIDUALPROGRESSION_H
#define AZEROTHCORE_INDIVIDUALPROGRESSION_H

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "ScriptedCreature.h"
#include "GameObjectAI.h"
#include "MapMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Group.h"
#include "Pet.h"
#include "DBCEnums.h"
#include "QuestDef.h"
#include "GameObject.h"
#include "IWorld.h"
#include <regex>

typedef std::unordered_map<uint32, uint32> questXpMapType;

enum ProgressionBossIDs
{
    RAGNAROS = 11502,
    ONYXIA = 10184,
    NEFARIAN = 11583,
    CTHUN = 15727,
    KELTHUZAD = 15990,
    KELTHUZAD_40 = 351019,
    MALCHEZAAR = 15690,
    KAELTHAS = 19622,
    ILLIDAN = 22917,
    ZULJIN = 23863,
    KILJAEDEN = 25315,
    YOGGSARON = 33288,
    ANUBARAK = 34564,
    LICH_KING = 36597,
    HALION = 39863
};

enum BuffSpells
{
    HP_AURA_SPELL = 89501,
    ABSORB_SPELL = 89505,
    RANGED_HASTE_SPELL = 89507,
    SPELL_RUNE_TAP = 48982,
    SPELL_LIFE_STEAL = 20004,
    IPP_PHASE = 89509
};

enum ProgressionQuestIDs
{
    MIGHT_OF_KALIMDOR    = 8742,
    QUEST_MORROWGRAIN    = 3803,
    QUEST_TROLL_NECKLACE = 2881,
    QUEST_DEADWOOD       = 6221,
    QUEST_WINTERFALL     = 6241,  

    Horde_Wailing_Caverns = 441161,
    Horde_Ragefire_Chasm = 441162,
    Horde_Deadmines = 441183,
    Horde_Shadowfang_Keep = 441158,
    Horde_Blackfathom_Deeps = 441164,
    Horde_Stormwind_Stockade = 441157,
    Horde_Scarlet_Monastery_Graveyard = 441165,
    Horde_Gnomeregan = 441163,
    Horde_Razorfen_Kraul = 441159,
    Horde_Razorfen_Downs = 441160,
    Horde_Scarlet_Monastery_Library = 441178,
    Horde_Scarlet_Monastery_Armory = 441176,
    Horde_Scarlet_Monastery_Cathedral = 441177,
    Horde_Maraudon_Orange = 441155,
    Horde_ZulFarrak = 441167,
    Horde_Maraudon_Purple = 441156,
    Horde_Uldaman_East = 441166,
    Horde_Maraudon_Pristine_Waters = 441179,
    Horde_Blackrock_Depths_Prison = 441169,
    Horde_Sunken_Temple = 441168,
    Horde_Dire_Maul_East = 441171,
    Horde_Lower_Blackrock_Spire = 441170,
    Horde_Upper_Blackrock_Spire = 441175,
    Horde_Dire_Maul_West = 441172,
    Horde_Dire_Maul_North = 441173,
    Horde_Stratholme_Undead = 441180,
    Horde_Stratholme_Living = 441174,
    Horde_Scholomance = 441182,
    Horde_Blackrock_Depths_Upper_City = 441181,
    Horde_Uldaman_West = 441213,

    Alliance_Wailing_Caverns = 441188,
    Alliance_Ragefire_Chasm = 441189,
    Alliance_Deadmines = 441212,
    Alliance_Shadowfang_Keep = 441185,
    Alliance_Stormwind_Stockade = 441184,
    Alliance_Gnomeregan = 441190,
    Alliance_Blackfathom_Deeps = 441191,
    Alliance_Scarlet_Monastery_Graveyard = 441192,
    Alliance_Razorfen_Kraul = 441186,
    Alliance_Razorfen_Downs = 441187,
    Alliance_Scarlet_Monastery_Armory = 441204,
    Alliance_Scarlet_Monastery_Cathedral = 441205,
    Alliance_Scarlet_Monastery_Library = 441206,
    Alliance_Uldaman_East = 441193,
    Alliance_ZulFarrak = 441194,
    Alliance_Maraudon_Orange = 441195,
    Alliance_Maraudon_Purple = 441207,
    Alliance_Maraudon_Pristine_Waters = 441208,
    Alliance_Blackrock_Depths_Prison = 441197,
    Alliance_Sunken_Temple = 441196,
    Alliance_Dire_Maul_East = 441199,
    Alliance_Lower_Blackrock_Spire = 441198,
    Alliance_Dire_Maul_West = 441200,
    Alliance_Dire_Maul_North = 441201,
    Alliance_Stratholme_Living = 441202,
    Alliance_Stratholme_Undead = 441209,
    Alliance_Upper_Blackrock_Spire = 441203,
    Alliance_Blackrock_Depths_Upper_City = 441210,
    Alliance_Scholomance = 441211,
    Alliance_Uldaman_West = 441214
};

enum ProgressionMaps
{
    MAP_BLACKWING_LAIR = 469,
    MAP_ZUL_GURUB = 309,
    MAP_AQ_20 = 509,
    MAP_AQ_40 = 531,
    MAP_OUTLANDS = 530,
    MAP_NAXXRAMAS = 533,
    MAP_ZUL_AMAN = 568,
    MAP_NORTHREND = 571,
    MAP_ULDUAR = 603,
    MAP_TRIAL_OF_THE_CRUSADER = 649,
    MAP_TRIAL_OF_THE_CHAMPION = 650,
    MAP_ICC = 631,
    MAP_FORGE_OF_SOULS = 632,
    MAP_RUBY_SANCTUM = 724
};

enum ProgressionZones
{
    ZONE_AZUREMYST = 3524,
    ZONE_BLOODMYST = 3525,
    ZONE_GHOSTLANDS = 3433,
    ZONE_EVERSONG = 3430,
    ZONE_EXODAR = 3557,
    ZONE_SILVERMOON = 3487,
    ZONE_QUELDANAS = 4080,
    ZONE_VEILED_SEA = 3479
};

enum ProgressionAreas
{
    AREA_LIGHTS_HOPE = 2268,
    AREA_ARGENT_TOURNAMENT = 4658,
    AREA_ARGENT_SUNREAVER_PAVILION = 4666,
    AREA_ARGENT_SILVER_COVENANT_PAVILION = 4667,
    AREA_ARGENT_RING_OF_CHAMPIONS = 4669,
    AREA_ARGENT_ASPIRANTS_RING = 4670,
    AREA_ARGENT_VALIANTS_RING = 4671,
    AREA_ARGENT_ALLIANCE_VALIANTS_RING = 4672,
    AREA_ARGENT_HORDE_VALIANTS_RING = 4673,
    AREA_ARGENT_PAVILION = 4674
};

enum ProgressionSettings
{
    SETTING_PROGRESSION_STATE = 0
};

enum ProgressionState : uint8
{
    PROGRESSION_START           = 0,
    PROGRESSION_MOLTEN_CORE     = 1,
    PROGRESSION_ONYXIA          = 2,
    PROGRESSION_BLACKWING_LAIR  = 3,
    PROGRESSION_PRE_AQ          = 4,
    PROGRESSION_AQ              = 5,
    PROGRESSION_NAXX40          = 6,
    PROGRESSION_TBC_TIER_1      = 7,  // Karazhan, Gruul's Lair, Magtheridon's Lair
    PROGRESSION_TBC_TIER_2      = 8,  // Serpentshrine Cavern, Tempest Keep
    PROGRESSION_TBC_TIER_3      = 9,  // Hyjal Summit and Black Temple
    PROGRESSION_TBC_TIER_4      = 10, // Zul'Aman
    PROGRESSION_TBC_TIER_5      = 11, // Sunwell Plateau
    PROGRESSION_WOTLK_TIER_1    = 12, // WotLK Naxx, EoE, OS
    PROGRESSION_WOTLK_TIER_2    = 13, // Ulduar
    PROGRESSION_WOTLK_TIER_3    = 14, // TotC
    PROGRESSION_WOTLK_TIER_4    = 15, // ICC
    PROGRESSION_WOTLK_TIER_5    = 16  // Ruby Sanctum
};

enum RandomDungeonIds : uint16
{
    // Values from LFGDungeons.dbc
    RDF_CLASSIC = 258,
    RDF_THE_BURNING_CRUSADE = 259,
    RDF_THE_BURNING_CRUSADE_HEROIC = 260,
    RDF_WRATH_OF_THE_LICH_KING = 261,
    RDF_WRATH_OF_THE_LICH_KING_HEROIC = 262
};

enum PvPTitles : uint32
{
    // Alliance
    PRIVATE                  = 1,
    CORPORAL                 = 2,
    SERGEANT                 = 3,
    MASTER_SERGEANT          = 4,
    SERGEANT_MAJOR           = 5,
    KNIGHT                   = 6,
    KNIGHT_LIEUTENANT        = 7,
    KNIGHT_CAPTAIN           = 8,
    KNIGHT_CHAMPION          = 9,
    LIEUTENANT_COMMANDER     = 10,
    COMMANDER                = 11,
    MARSHAL                  = 12,
    FIELD_MARSHAL            = 13,
    GRAND_MARSHAL            = 14,

    // Horde
    SCOUT                    = 15,
    GRUNT                    = 16,
    SERGEANT_H               = 17,
    SENIOR_SERGEANT          = 18,
    FIRST_SERGEANT           = 19,
    STONE_GUARD              = 20,
    BLOOD_GUARD              = 21,
    LEGIONNAIRE              = 22,
    CENTURION                = 23,
    CHAMPION                 = 24,
    LIEUTENANT_GENERAL       = 25,
    GENERAL                  = 26,
    WARLORD                  = 27,
    HIGH_WARLORD             = 28
};

class IndividualProgression
{
public:
    static IndividualProgression* instance();

    std::map<uint32, uint8> customProgressionMap;
    questXpMapType questXpMap;
    float vanillaPowerAdjustment, vanillaHealthAdjustment, tbcPowerAdjustment, tbcHealthAdjustment, vanillaHealingAdjustment, tbcHealingAdjustment, previousGearTuning;
    bool enabled, questXpFix, hunterPetLevelFix, requirePreAQQuests, enforceGroupRules, fishingFix, simpleConfigOverride, questMoneyAtLevelCap, repeatableVanillaQuestsXp, disableDefaultProgression, earlyDungeonSet2, requireNaxxStrath, pvpGearRequirements, excludeAccounts;
    int progressionLimit, startingProgression, tbcRacesProgressionLevel, deathKnightProgressionLevel, deathKnightStartingProgression;
    std::string excludedAccountsRegex;

    bool hasPassedProgression(Player* player, ProgressionState state) const;
    static bool isBeforeProgression(Player* player, ProgressionState state) ;
    void UpdateProgressionState(Player* player, ProgressionState newState) const;
    static void ForceUpdateProgressionState(Player* player, ProgressionState newState);
    void CheckAdjustments(Player* player) const;
    void ApplyGearStatsTuning(Player* player, float& computedAdjustment, ItemTemplate const* item) const;
    void ComputeGearTuning(Player* player, float& computedAdjustment, ItemTemplate const* item) const;
    void AdjustVanillaStats(Player* player) const;
    void AdjustTBCStats(Player* player) const;
    void AdjustWotLKStats(Player* player) const;
    bool hasCustomProgressionValue(uint32 creatureEntry);
    void checkKillProgression(Player* player, Creature* killed);
    static void LoadCustomProgressionEntries(const std::string& customProgressionString);
    static void AdjustStats(Player* player, float computedAdjustment, float computedHealingAdjustment);
    static float ComputeVanillaAdjustment(uint8 playerLevel, float configAdjustmentValue);
    static uint8 GetAccountProgression(uint32 accountId);
};

#define sIndividualProgression IndividualProgression::instance()

#endif //AZEROTHCORE_INDIVIDUALPROGRESSION_H
