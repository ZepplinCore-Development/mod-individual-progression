# Individual Player Progression

## Please Note
The module uses a hook that is not yet merged into AzerothCore (see https://github.com/azerothcore/azerothcore-wotlk/pull/10934).  Until this PR is merged, you may need to comment out `OnQuestComputeXP` method in IndividualProgression.cpp. This function is used to give correct-era XP for Vanilla and TBC quests.

## Azerothcore Module

This module is meant to simulate "progress through expansions and expansion tiers" for individual players. Players must complete each tier in order to access content for the next tier.

Each tier is designed to simulate experience of being within that tier and expansion, within reasonable of the WotLK client. This means Vanilla content is like Vanilla WoW, and so on.

The goal of the module is to focus on journey of the player. All catch-up mechanisms have been removed. There is no goal of defeat end-game or find "fresh" server, because each new character is a fresh server.

Note that this module either requires many players working together on a server for each tier, or adjustments for smaller raid size to allow individual groups to progress. Please see companion modules auto-balance and Small Server Tweaks for some adjustments that improve this process on a less popular server.

For content that is added in later expansion that is provided for use in lower-level content and is not "catch-up" content, I have decided to remain. This includes Draenei and Blood Elf starting zone, Inscription and Jewelcrafting skills, new quests and hubs added to Vanilla world in TBC, and so on. I feel this content does not negative impact the player journey like "catch-up" content.

## Some Included Features
* Power level adjustment and health in Vanilla and TBC content to simulate Vanilla world difficulty
* NPCs and Game Objects aware of individual player progress, visible based on player progression to each player
* Restore of content removed or nerfed or later patch is released, such as Onyxia quests, TBC Attunements, and riding skills and quests.

Options are provided to allow changes to experience based on preference. Please see config file.

## Thank You
Special thanks to all contributors of the following projects which are make this module possible:
* AzerothCore and TrinityCore, along with ancestors Mangos, etc.
* AzerothCore Progression Module for Vanilla content and scripts
* Wrath of the Vanilla Module for more Vanilla world adjustments
* Vmangos for Vanilla game and patch progression data