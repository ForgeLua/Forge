/*
 * Part of Forge <https://github.com/iThorgrim/Forge>, a standalone fork of Eluna Lua Engine.
 * 
 * Copyright (C) Forge contributors
 * Based on Eluna <https://elunaluaengine.github.io/>
 * Copyright (C) Eluna Lua Engine contributors
 * 
 * Licensed under the GNU GPL v3 only.
 * See LICENSE file or <https://www.gnu.org/licenses/>.
 */

#ifndef PLAYERMETHODS_H
#define PLAYERMETHODS_H

#include "LuaValue.h"
#include "NPCPackets.h"
#include "PartyPackets.h"
#include "Unit.h"
#include <boost/callable_traits/args.hpp>

/***
 * Inherits all methods from: [Object], [WorldObject], [Unit]
 */
namespace LuaPlayer
{
    /**
     * Returns 'true' if the [Player] can Titan Grip the specific [Item], 'false' otherwise.
     * @param [Item] item : an instance of an item
     * @return bool canTitanGrip
     */
    int CanTitanGrip(Forge* F, Player* player)
    {
        Item* item = F->CHECKOBJ<Item>(2);

        F->Push(player->CanTitanGrip(item));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] has a talent by ID in specified spec, 'false' otherwise.
     *
     * @param uint32 spellId : talent spellId to check
     * @param uint8 spec : specified spec. 0 for primary, 1 for secondary.
     * @return bool hasTalent
     */
    int HasTalent(Forge* F, Player* player)
    {
        uint32 spellId = F->CHECKVAL<uint32>(2);
        uint8 maxSpecs = MAX_TALENT_SPECS;
        uint8 spec = F->CHECKVAL<uint8>(3);

        if (spec >= maxSpecs)
            return 1;

        F->Push(player->HasTalent(spellId, spec));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] has completed the specified achievement, 'false' otherwise.
     *
     * @param uint32 achievementId
     * @return bool hasAchieved
     */
    int HasAchieved(Forge* F, Player* player)
    {
        uint32 achievementId = F->CHECKVAL<uint32>(2);

        F->Push(player->HasAchieved(achievementId));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] has an active [Quest] by specific ID, 'false' otherwise.
     *
     * @param uint32 questId
     * @return bool hasQuest
     */
    int HasQuest(Forge* F, Player* player)
    {
        uint32 quest = F->CHECKVAL<uint32>(2);

        F->Push(player->IsActiveQuest(quest));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] has a skill by specific ID, 'false' otherwise.
     *
     * @param uint32 skill
     * @return bool hasSkill
     */
    int HasSkill(Forge* F, Player* player)
    {
        uint32 skill = F->CHECKVAL<uint32>(2);

        F->Push(player->HasSkill(skill));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] has a [Spell] by specific ID, 'false' otherwise.
     *
     * @param uint32 spellId
     * @return bool hasSpell
     */
    int HasSpell(Forge* F, Player* player)
    {
        uint32 id = F->CHECKVAL<uint32>(2);

        F->Push(player->HasSpell(id));
        return 1;
    }

    /**
     * Returns true if [Player] has specified login flag
     *
     * @param uint32 flag
     * @return bool hasLoginFlag
     */
    int HasAtLoginFlag(Forge* F, Player* player)
    {
        uint32 flag = F->CHECKVAL<uint32>(2);

        F->Push(player->HasAtLoginFlag((AtLoginFlags)flag));
        return 1;
    }

    /**
     * Returns true if [Player] has [Quest] for [GameObject]
     *
     * @param int32 entry : entry of a [GameObject]
     * @return bool hasQuest
     */
    int HasQuestForGO(Forge* F, Player* player)
    {
        int32 entry = F->CHECKVAL<int32>(2);

        F->Push(player->HasQuestForGO(entry));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] has a title by specific ID, 'false' otherwise.
     *
     * @param uint32 titleId
     * @return bool hasTitle
     */
    int HasTitle(Forge* F, Player* player)
    {
        uint32 id = F->CHECKVAL<uint32>(2);
        CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id);
        if (titleInfo)
            F->Push(player->HasTitle(titleInfo));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] has the given amount of item entry specified, 'false' otherwise.
     *
     * @param uint32 itemId : entry of the item
     * @param uint32 count = 1 : amount of items the player needs should have
     * @param bool check_bank = false : determines if the item can be in player bank
     * @return bool hasItem
     */
    int HasItem(Forge* F, Player* player)
    {
        uint32 itemId = F->CHECKVAL<uint32>(2);
        uint32 count = F->CHECKVAL<uint32>(3, 1);
        bool check_bank = F->CHECKVAL<bool>(4, false);
        F->Push(player->HasItemCount(itemId, count, check_bank));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] has a quest for the item entry specified, 'false' otherwise.
     *
     * @param uint32 entry : entry of the item
     * @return bool hasQuest
     */
    int HasQuestForItem(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        F->Push(player->HasQuestForItem(entry));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] can use the item or item entry specified, 'false' otherwise.
     *
     * @proto canUse = (item)
     * @proto canUse = (entry)
     * @param [Item] item : an instance of an item
     * @param uint32 entry : entry of the item
     * @return bool canUse
     */
    int CanUseItem(Forge* F, Player* player)
    {
        Item* item = F->CHECKOBJ<Item>(2, false);
        if (item)
            F->Push(player->CanUseItem(item) == EQUIP_ERR_OK);
        else
        {
            uint32 entry = F->CHECKVAL<uint32>(2);
            const ItemTemplate* temp = eObjectMgr->GetItemTemplate(entry);
            if (temp)
                F->Push(player->CanUseItem(temp) == EQUIP_ERR_OK);
            else
                F->Push(false);
        }
        return 1;
    }

    /**
     * Returns 'true' if the [Spell] specified by ID is currently on cooldown for the [Player], 'false' otherwise.
     *
     * @param uint32 spellId
     * @return bool hasSpellCooldown
     */
    int HasSpellCooldown(Forge* F, Player* player)
    {
        uint32 spellId = F->CHECKVAL<uint32>(2);

        F->Push(player->GetSpellHistory()->HasCooldown(spellId));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] can share [Quest] specified by ID, 'false' otherwise.
     *
     * @param uint32 entryId
     * @return bool hasSpellCooldown
     */
    int CanShareQuest(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        F->Push(player->CanShareQuest(entry));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] can currently communicate through chat, 'false' otherwise.
     *
     * @return bool canSpeak
     */
    int CanSpeak(Forge* F, Player* player)
    {
        F->Push(player->GetSession()->CanSpeak());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] has permission to uninvite others from the current group, 'false' otherwise.
     *
     * @return bool canUninviteFromGroup
     */
    int CanUninviteFromGroup(Forge* F, Player* player)
    {
        F->Push(player->CanUninviteFromGroup() == ERR_PARTY_RESULT_OK);
        return 1;
    }

    /**
     * Returns 'true' if the [Player] can fly, 'false' otherwise.
     *
     * @return bool canFly
     */
    int CanFly(Forge* F, Player* player)
    {
        F->Push(player->CanFly());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is currently in water, 'false' otherwise.
     *
     * @return bool isInWater
     */
    int IsInWater(Forge* F, Player* player)
    {
        F->Push(player->IsInWater());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is currently moving, 'false' otherwise.
     *
     * @return bool isMoving
     */
    int IsMoving(Forge* F, Player* player) // enable for unit when mangos support it
    {
        F->Push(player->isMoving());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is currently flying, 'false' otherwise.
     *
     * @return bool isFlying
     */
    int IsFlying(Forge* F, Player* player) // enable for unit when mangos support it
    {
        F->Push(player->IsFlying());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is in a [Group], 'false' otherwise.
     *
     * @return bool isInGroup
     */
    int IsInGroup(Forge* F, Player* player)
    {
        F->Push((player->GetGroup() != NULL));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is in a [Guild], 'false' otherwise.
     *
     * @return bool isInGuild
     */
    int IsInGuild(Forge* F, Player* player)
    {
        F->Push((player->GetGuildId() != 0));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is a Game Master, 'false' otherwise.
     *
     * Note: This is only true when GM tag is activated! For alternative see [Player:GetGMRank]
     *
     * @return bool isGM
     */
    int IsGM(Forge* F, Player* player)
    {
        F->Push(player->IsGameMaster());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is in an arena team specified by type, 'false' otherwise.
     *
     * @param uint32 type
     * @return bool isInArenaTeam
     */
    int IsInArenaTeam(Forge* F, Player* player)
    {
        uint32 type = F->CHECKVAL<uint32>(2);
        if (type < MAX_ARENA_SLOT && player->GetArenaTeamId(type))
            F->Push(true);
        else
            F->Push(false);
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is immune to everything.
     *
     * @return bool isImmune
     */
    int IsImmuneToDamage(Forge* F, Player* player)
    {
        F->Push(player->isTotalImmune());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] satisfies all requirements to complete the quest entry.
     *
     * @param uint32 entry
     * @return bool canComplete
     */
    int CanCompleteQuest(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        F->Push(player->CanCompleteQuest(entry));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] satisfies all requirements to complete the repeatable quest entry.
     *
     * @param uint32 entry
     * @return bool canComplete
     */
    int CanCompleteRepeatableQuest(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        Quest const* qInfo = sObjectMgr->GetQuestTemplate(entry);
        if (qInfo)
            F->Push(player->CanCompleteRepeatableQuest(qInfo));
        else
            F->Push(false);

        return 1;
    }

    /**
     * Returns 'true' if the [Player] satisfies all requirements to turn in the quest.
     *
     * @param uint32 entry
     * @return bool canReward
     */
    int CanRewardQuest(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        Quest const* qInfo = sObjectMgr->GetQuestTemplate(entry);
        if (qInfo)
            F->Push(player->CanRewardQuest(qInfo, true));
        else
            F->Push(false);

        return 1;
    }

    /**
     * Returns 'true' if the [Player] is a part of the Horde faction, 'false' otherwise.
     *
     * @return bool isHorde
     */
    int IsHorde(Forge* F, Player* player)
    {
        F->Push((player->GetTeam() == HORDE));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is a part of the Alliance faction, 'false' otherwise.
     *
     * @return bool isAlliance
     */
    int IsAlliance(Forge* F, Player* player)
    {
        F->Push((player->GetTeam() == ALLIANCE));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is 'Do Not Disturb' flagged, 'false' otherwise.
     *
     * @return bool isDND
     */
    int IsDND(Forge* F, Player* player)
    {
        F->Push(player->isDND());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is 'Away From Keyboard' flagged, 'false' otherwise.
     *
     * @return bool isAFK
     */
    int IsAFK(Forge* F, Player* player)
    {
        F->Push(player->isAFK());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is currently falling, 'false' otherwise.
     *
     * @return bool isFalling
     */
    int IsFalling(Forge* F, Player* player)
    {
        F->Push(player->IsFalling());
        return 1;
    }

    /**
     * Returns whether or not the [Player]s [Group] is visible for the other specific [Player].
     *
     * @param [Player] player
     * @return bool isGroupVisible
     */
    int IsGroupVisibleFor(Forge* F, Player* player)
    {
        Player* target = F->CHECKOBJ<Player>(2);
        F->Push(player->IsGroupVisibleFor(target));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is currently in the same raid as another [Player] by object, 'false' otherwise.
     *
     * @param [Player] player
     * @return bool isInSameRaidWith
     */
    int IsInSameRaidWith(Forge* F, Player* player)
    {
        Player* target = F->CHECKOBJ<Player>(2);
        F->Push(player->IsInSameRaidWith(target));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is currently in the same [Group] as another [Player] by object, 'false' otherwise.
     *
     * @param [Player] player
     * @return bool isInSameGroupWith
     */
    int IsInSameGroupWith(Forge* F, Player* player)
    {
        Player* target = F->CHECKOBJ<Player>(2);
        F->Push(player->IsInSameGroupWith(target));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is eligible for Honor or XP gain by [Unit] specified, 'false' otherwise.
     *
     * @param [Unit] unit
     * @return bool isHonorOrXPTarget
     */
    int IsHonorOrXPTarget(Forge* F, Player* player)
    {
        Unit* victim = F->CHECKOBJ<Unit>(2);

        F->Push(player->isHonorOrXPTarget(victim));
        return 1;
    }

    /**
     * Returns 'true' if the [Player] can see anoter [Player] specified by object, 'false' otherwise.
     *
     * @param [Player] player
     * @return bool isVisibleForPlayer
     */
    int IsVisibleForPlayer(Forge* F, Player* player)
    {
        Player* target = F->CHECKOBJ<Player>(2);

        F->Push(player->IsVisibleGloballyFor(target));
        return 1;
    }

    /**
     * Returns whether or not the [Player] has GM invisibility active
     *
     * @return bool isGMVisible
     */
    int IsGMVisible(Forge* F, Player* player)
    {
        F->Push(player->isGMVisible());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] has taxi cheat activated, 'false' otherwise.
     *
     * @return bool isTaxiCheater
     */
    int IsTaxiCheater(Forge* F, Player* player)
    {
        F->Push(player->isTaxiCheater());
        return 1;
    }

    /**
     * Returns whether or not the [Player] has the GM chat flag active
     *
     * @return bool isGMChatActive
     */
    int IsGMChat(Forge* F, Player* player)
    {
        F->Push(player->isGMChat());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is accepting whispers, 'false' otherwise.
     *
     * @return bool isAcceptingWhispers
     */
    int IsAcceptingWhispers(Forge* F, Player* player)
    {
        F->Push(player->isAcceptWhispers());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is currently rested, 'false' otherwise.
     *
     * @return bool isRested
     */
    int IsRested(Forge* F, Player* player)
    {
        F->Push(player->GetRestBonus() > 0.0f);
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is currently in a [BattleGround] queue, 'false' otherwise.
     *
     * @return bool inBattlegroundQueue
     */
    int InBattlegroundQueue(Forge* F, Player* player)
    {
        F->Push(player->InBattlegroundQueue());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is currently in an arena, 'false' otherwise.
     *
     * @return bool inArena
     */
    int InArena(Forge* F, Player* player)
    {
        F->Push(player->InArena());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] is currently in a [BattleGround], 'false' otherwise.
     *
     * @return bool inBattleGround
     */
    int InBattleground(Forge* F, Player* player)
    {
        F->Push(player->InBattleground());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] can block incomming attacks, 'false' otherwise.
     *
     * @return bool canBlock
     */
    int CanBlock(Forge* F, Player* player)
    {
        F->Push(player->CanBlock());
        return 1;
    }

    /**
     * Returns 'true' if the [Player] can parry incomming attacks, 'false' otherwise.
     *
     * @return bool canParry
     */
    int CanParry(Forge* F, Player* player)
    {
        F->Push(player->CanParry());
        return 1;
    }

    /**
     * Returns whether or not the [Player] has received the reward for a specific [Quest] ID
     *
     * @param uint32 questId
     * @return bool isQuestRewarded
     */
    int HasReceivedQuestReward(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        F->Push(player->IsQuestRewarded(entry));
        return 1;
    }

    /**
     * Returns whether or not the [Player] is currently flagged for outdoors PvP
     *
     * @return bool isPvPActive
     */
    int IsOutdoorPvPActive(Forge* F, Player* player)
    {
        F->Push(player->IsOutdoorPvPActive());
        return 1;
    }

    /**
     * Returns whether or not the [Player] is currently immune to environmental damage
     *
     * @return bool isImmuneToEnv
     */
    int IsImmuneToEnvironmentalDamage(Forge* F, Player* player)
    {
        F->Push(player->IsImmuneToEnvironmentalDamage());
        return 1;
    }

    /**
     * Returns whether or not the [Player] is currently in a random LFG dungeon
     *
     * @return bool isInRandomLFG
     */
    int InRandomLfgDungeon(Forge* F, Player* player)
    {
        F->Push(player->inRandomLfgDungeon());
        return 1;
    }

    /**
     * Returns whether or not the [Player] is currently queued in LFG
     *
     * @return bool isUsingLFG
     */
    int IsUsingLfg(Forge* F, Player* player)
    {
        F->Push(player->isUsingLfg());
        return 1;
    }

    /**
     * Returns whether or not the [Player] is never visible for other [Unit]s
     *
     * @return bool isNeverVisible
     */
    int IsNeverVisible(Forge* F, Player* player)
    {
        F->Push(player->IsNeverVisible(true));
        return 1;
    }

    /**
     * Returns whether or not the [Player] has any pending dungeon bind
     *
     * @return bool hasPendingBind
     */
    int HasPendingBind(Forge* F, Player* player)
    {
        F->Push(player->HasPendingBind());
        return 1;
    }

    /**
     * Returns whether or not the [Player] is a recruiter
     *
     * @return bool isARecruiter
     */
    int IsARecruiter(Forge* F, Player* player)
    {
        F->Push(player->GetSession()->IsARecruiter());
        return 1;
    }

    /**
     * Returns whether or not the [Player] has been recruited
     *
     * @return bool isRecruited
     */
    int IsRecruited(Forge* F, Player* player)
    {
        F->Push(player->GetSession()->GetRecruiterId() != 0);
        return 1;
    }

    /**
     * Returns whether or not the [Player] recruited the other [Player]
     *
     * @param [Player] recruit
     * @return bool hasRecruited : returns 'true' if the [Player] recruited the other [Player], false otherwise
     */
    int HasRecruited(Forge* F, Player* player)
    {
        Player* recruit = F->CHECKOBJ<Player>(2);
        F->Push(recruit->GetSession()->GetRecruiterId() == player->GetSession()->GetAccountId());
        return 1;
    }

    /**
     * Returns the amount of available specs the [Player] currently has
     *
     * @return uint8 specCount
     */
    int GetSpecsCount(Forge* F, Player* player)
    {
        F->Push(player->GetSpecsCount());
        return 1;
    }

    /**
     * Returns the [Player]s active spec ID
     *
     * @return uint32 specId
     */
    int GetActiveSpec(Forge* F, Player* player)
    {
        F->Push(player->GetActiveSpec());
        return 1;
    }

    /**
     * Returns the normal phase of the player instead of the actual phase possibly containing GM phase
     *
     * @return uint32 phasemask
     */
    int GetPhaseMaskForSpawn(Forge* F, Player* player)
    {
        F->Push(player->GetPhaseMaskForSpawn());
        return 1;
    }

    /**
     * Returns the [Player]s current amount of Arena Points
     *
     * @return uint32 arenaPoints
     */
    int GetArenaPoints(Forge* F, Player* player)
    {
        F->Push(player->GetArenaPoints());
        return 1;
    }

    /**
     * Returns the [Player]s current amount of Honor Points
     *
     * @return uint32 honorPoints
     */
    int GetHonorPoints(Forge* F, Player* player)
    {
        F->Push(player->GetHonorPoints());
        return 1;
    }

    /**
     * Returns the [Player]s current shield block value
     *
     * @return uint32 blockValue
     */
    int GetShieldBlockValue(Forge* F, Player* player)
    {
        F->Push(player->GetShieldBlockValue());
        return 1;
    }

    /**
     * Returns the [Player]s cooldown delay by specified [Spell] ID
     *
     * @param uint32 spellId
     * @return uint32 spellCooldownDelay
     */
    int GetSpellCooldownDelay(Forge* F, Player* player)
    {
        uint32 spellId = F->CHECKVAL<uint32>(2);

        if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId))
            F->Push(player->GetSpellHistory()->GetRemainingCooldown(spellInfo));
        else
            F->Push(0);

        return 1;
    }

    /**
     * Returns the [Player]s current latency in MS
     *
     * @return uint32 latency
     */
    int GetLatency(Forge* F, Player* player)
    {
        F->Push(player->GetSession()->GetLatency());
        return 1;
    }

    /**
     * Returns the faction ID the [Player] is currently flagged as champion for
     *
     * @return uint32 championingFaction
     */
    int GetChampioningFaction(Forge* F, Player* player)
    {
        F->Push(player->GetChampioningFaction());
        return 1;
    }

    /**
     * Returns [Player]s original sub group
     *
     * @return uint8 subGroup
     */
    int GetOriginalSubGroup(Forge* F, Player* player)
    {
        F->Push(player->GetOriginalSubGroup());
        return 1;
    }

    /**
     * Returns [Player]s original [Group] object
     *
     * @return [Group] group
     */
    int GetOriginalGroup(Forge* F, Player* player)
    {
        F->Push(player->GetOriginalGroup());
        return 1;
    }

    /**
     * Returns a random Raid Member [Player] object within radius specified of [Player]
     *
     * @param float radius
     * @return [Player] player
     */
    int GetNextRandomRaidMember(Forge* F, Player* player)
    {
        float radius = F->CHECKVAL<float>(2);

        F->Push(player->GetNextRandomRaidMember(radius));
        return 1;
    }

    /**
     * Returns [Player]s current sub group
     *
     * @return uint8 subGroup
     */
    int GetSubGroup(Forge* F, Player* player)
    {
        F->Push(player->GetSubGroup());
        return 1;
    }

    /**
     * Returns [Group] invitation
     *
     * @return [Group] group
     */
    int GetGroupInvite(Forge* F, Player* player)
    {
        F->Push(player->GetGroupInvite());
        return 1;
    }

    /**
     * Returns rested experience bonus
     *
     * @param uint32 xp
     * @return uint32 xpBonus
     */
    int GetXPRestBonus(Forge* F, Player* player)
    {
        uint32 xp = F->CHECKVAL<uint32>(2);

        F->Push(player->GetXPRestBonus(xp));
        return 1;
    }

    /**
     * Returns the [Player]s current [BattleGround] type ID
     *
     * @return [BattleGroundTypeId] typeId
     */
    int GetBattlegroundTypeId(Forge* F, Player* player)
    {
        F->Push(player->GetBattlegroundTypeId());
        return 1;
    }

    /**
     * Returns the [Player]s current [BattleGround] ID
     *
     * @return uint32 battleGroundId
     */
    int GetBattlegroundId(Forge* F, Player* player)
    {
        F->Push(player->GetBattlegroundId());
        return 1;
    }

    /**
     * Returns the [Player]s reputation rank of faction specified
     *
     * @param uint32 faction
     * @return [ReputationRank] rank
     */
    int GetReputationRank(Forge* F, Player* player)
    {
        uint32 faction = F->CHECKVAL<uint32>(2);

        F->Push(player->GetReputationRank(faction));
        return 1;
    }

    /**
     * Returns the [Player]s current level of intoxication
     *
     * @return uint16 drunkValue
     */
    int GetDrunkValue(Forge* F, Player* player)
    {
        F->Push(player->GetDrunkValue());
        return 1;
    }

    /**
     * Returns skill temporary bonus value
     *
     * @param uint32 skill
     * @return int16 bonusVal
     */
    int GetSkillTempBonusValue(Forge* F, Player* player)
    {
        uint32 skill = F->CHECKVAL<uint32>(2);

        F->Push(player->GetSkillTempBonusValue(skill));
        return 1;
    }

    /**
     * Returns skill permanent bonus value
     *
     * @param uint32 skill
     * @return int16 bonusVal
     */
    int GetSkillPermBonusValue(Forge* F, Player* player)
    {
        uint32 skill = F->CHECKVAL<uint32>(2);

        F->Push(player->GetSkillPermBonusValue(skill));
        return 1;
    }

    /**
     * Returns skill value without bonus'
     *
     * @param uint32 skill
     * @return uint16 pureVal
     */
    int GetPureSkillValue(Forge* F, Player* player)
    {
        uint32 skill = F->CHECKVAL<uint32>(2);

        F->Push(player->GetPureSkillValue(skill));
        return 1;
    }

    /**
     * Returns base skill value
     *
     * @param uint32 skill
     * @return uint16 baseVal
     */
    int GetBaseSkillValue(Forge* F, Player* player)
    {
        uint32 skill = F->CHECKVAL<uint32>(2);

        F->Push(player->GetBaseSkillValue(skill));
        return 1;
    }

    /**
     * Returns skill value
     *
     * @param uint32 skill
     * @return uint16 val
     */
    int GetSkillValue(Forge* F, Player* player)
    {
        uint32 skill = F->CHECKVAL<uint32>(2);

        F->Push(player->GetSkillValue(skill));
        return 1;
    }

    /**
     * Returns max value of specified skill without bonus'
     *
     * @param uint32 skill
     * @return uint16 pureVal
     */
    int GetPureMaxSkillValue(Forge* F, Player* player)
    {
        uint32 skill = F->CHECKVAL<uint32>(2);

        F->Push(player->GetPureMaxSkillValue(skill));
        return 1;
    }

    /**
     * Returns max value of specified skill
     *
     * @param uint32 skill
     * @return uint16 val
     */
    int GetMaxSkillValue(Forge* F, Player* player)
    {
        uint32 skill = F->CHECKVAL<uint32>(2);

        F->Push(player->GetMaxSkillValue(skill));
        return 1;
    }

    /**
     * Returns mana bonus from amount of intellect
     *
     * @return float bonus
     */
    int GetManaBonusFromIntellect(Forge* F, Player* player)
    {
        F->Push(player->GetManaBonusFromIntellect());
        return 1;
    }

    /**
     * Returns health bonus from amount of stamina
     *
     * @return float bonus
     */
    int GetHealthBonusFromStamina(Forge* F, Player* player)
    {
        F->Push(player->GetHealthBonusFromStamina());
        return 1;
    }

    /**
     * Returns raid or dungeon difficulty
     *
     * @param bool isRaid = true : argument is TrinityCore only
     * @return int32 difficulty
     */
    int GetDifficulty(Forge* F, Player* player)
    {
        bool isRaid = F->CHECKVAL<bool>(2, true);

        F->Push(player->GetDifficulty(isRaid));
        return 1;
    }

    /**
     * Returns the [Player]s current guild rank
     *
     * @return uint32 guildRank
     */
    int GetGuildRank(Forge* F, Player* player) // TODO: Move to Guild Methods
    {
        F->Push(player->GetGuildRank());
        return 1;
    }

    /**
     * Returns the [Player]s free talent point amount
     *
     * @return uint32 freeTalentPointAmt
     */
    int GetFreeTalentPoints(Forge* F, Player* player)
    {
        F->Push(player->GetFreeTalentPoints());
        return 1;
    }

    /**
     * Returns the name of the [Player]s current [Guild]
     *
     * @return string guildName
     */
    int GetGuildName(Forge* F, Player* player)
    {
        if (!player->GetGuildId())
            return 1;
        F->Push(eGuildMgr->GetGuildNameById(player->GetGuildId()));
        return 1;
    }

    /**
     * Returns the amount of reputation the [Player] has with the faction specified
     *
     * @param uint32 faction
     * @return int32 reputationAmt
     */
    int GetReputation(Forge* F, Player* player)
    {
        uint32 faction = F->CHECKVAL<uint32>(2);

        F->Push(player->GetReputationMgr().GetReputation(faction));
        return 1;
    }

    /**
     * Returns [Unit] target combo points are on
     *
     * @return [Unit] target
     */
    int GetComboTarget(Forge* F, Player* player)
    {
        F->Push(player->GetComboTarget());
        return 1;
    }

    /**
     * Returns [Player]'s combo points
     *
     * @return uint8 comboPoints
     */
    int GetComboPoints(Forge* F, Player* player)
    {
        F->Push(player->GetComboPoints());
        return 1;
    }

    /**
     * Returns the amount of time the [Player] has spent ingame
     *
     * @return uint32 inGameTime
     */
    int GetInGameTime(Forge* F, Player* player)
    {
        F->Push(player->GetInGameTime());
        return 1;
    }

    /**
     * Returns the status of the [Player]s [Quest] specified by entry ID
     *
     * @param uint32 questId
     * @return [QuestStatus] questStatus
     */
    int GetQuestStatus(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        F->Push(player->GetQuestStatus(entry));
        return 1;
    }

    /**
     * Returns 'true' if the [Player]s [Quest] specified by entry ID has been rewarded, 'false' otherwise.
     *
     * @param uint32 questId
     * @return bool questRewardStatus
     */
    int GetQuestRewardStatus(Forge* F, Player* player)
    {
        uint32 questId = F->CHECKVAL<uint32>(2);

        F->Push(player->GetQuestRewardStatus(questId));
        return 1;
    }

    /**
     * Returns [Quest] required [Creature] or [GameObject] count
     *
     * @param uint32 quest : entry of a quest
     * @param int32 entry : entry of required [Creature]
     * @return uint16 count
     */
    int GetReqKillOrCastCurrentCount(Forge* F, Player* player)
    {
        uint32 questId = F->CHECKVAL<uint32>(2);
        int32 entry = F->CHECKVAL<int32>(3);

        F->Push(player->GetReqKillOrCastCurrentCount(questId, entry));
        return 1;
    }

    /**
     * Returns the quest level of the [Player]s [Quest] specified by object
     *
     * @param uint32 questId
     * @return [QuestStatus] questRewardStatus
     */
    int GetQuestLevel(Forge* F, Player* player)
    {
        Quest* quest = F->CHECKOBJ<Quest>(2);

        F->Push(player->GetQuestLevel(quest));
        return 1;
    }

    /**
     * Returns a [Player]s [Item] object by gear slot specified
     *
     * @param uint8 slot
     * @return [Item] item
     */
    int GetEquippedItemBySlot(Forge* F, Player* player)
    {
        uint8 slot = F->CHECKVAL<uint8>(2);
        if (slot >= EQUIPMENT_SLOT_END)
            return 1;

        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        F->Push(item);
        return 1;
    }

    /**
     * Returns the [Player]s current resting bonus
     *
     * @return float restBonus
     */
    int GetRestBonus(Forge* F, Player* player)
    {
        F->Push(player->GetRestBonus());
        return 1;
    }

    /**
     * Returns active GM chat tag
     *
     * @return uint8 tag
     */
    int GetChatTag(Forge* F, Player* player)
    {
        F->Push(player->GetChatTag());
        return 1;
    }

    /**
     * Returns an item in given bag on given slot.
     *
     * <pre>
     * Possible and most commonly used combinations:
     *
     * bag = 255
     * slots 0-18 equipment
     * slots 19-22 equipped bag slots
     * slots 23-38 backpack
     * slots 39-66 bank main slots
     * slots 67-74 bank bag slots
     * slots 86-117 keyring
     *
     * bag = 19-22
     * slots 0-35 for equipped bags
     *
     * bag = 67-74
     * slots 0-35 for bank bags
     * </pre>
     *
     * @param uint8 bag : the bag the [Item] is in, you can get this with [Item:GetBagSlot]
     * @param uint8 slot : the slot the [Item] is in within the bag, you can get this with [Item:GetSlot]
     * @return [Item] item : [Item] or nil
     */
    int GetItemByPos(Forge* F, Player* player)
    {
        uint8 bag = F->CHECKVAL<uint8>(2);
        uint8 slot = F->CHECKVAL<uint8>(3);

        F->Push(player->GetItemByPos(bag, slot));
        return 1;
    }

    /**
     * Returns an [Item] from the player by guid.
     *
     * The item can be equipped, in bags or in bank.
     *
     * @param ObjectGuid guid : an item guid
     * @return [Item] item
     */
    int GetItemByGUID(Forge* F, Player* player)
    {
        ObjectGuid guid = F->CHECKVAL<ObjectGuid>(2);

        F->Push(player->GetItemByGuid(guid));
        return 1;
    }

    /**
     * Returns a mailed [Item] by guid.
     *
     * @param ObjectGuid guid : an item guid
     * @return [Item] item
     */
    int GetMailItem(Forge* F, Player* player)
    {
        ObjectGuid guid = F->CHECKVAL<ObjectGuid>(2);

        F->Push(player->GetMItem(guid.GetCounter()));
        return 1;
    }

    /**
     * Returns an [Item] from the player by entry.
     *
     * The item can be equipped, in bags or in bank.
     *
     * @param uint32 entryId
     * @return [Item] item
     */
    int GetItemByEntry(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        F->Push(player->GetItemByEntry(entry));
        return 1;
    }

    /**
     * Returns the database textID of the [WorldObject]'s gossip header text for the [Player]
     *
     * @param [WorldObject] object
     * @return uint32 textId : key to npc_text database table
     */
    int GetGossipTextId(Forge* F, Player* player)
    {
        WorldObject* obj = F->CHECKOBJ<WorldObject>(2);
        F->Push(player->GetGossipTextId(obj));
        return 1;
    }

    /**
     * Returns the [Player]s currently selected [Unit] object
     *
     * @return [Unit] unit
     */
    int GetSelection(Forge* F, Player* player)
    {
        F->Push(player->GetSelectedUnit());
        return 1;
    }

    /**
     * Returns the [Player]s GM Rank
     *
     * @return [AccountTypes] gmRank
     */
    int GetGMRank(Forge* F, Player* player)
    {
        F->Push(player->GetSession()->GetSecurity());
        return 1;
    }

    /**
     * Returns the [Player]s amount of money in copper
     *
     * @return uint32 coinage
     */
    int GetCoinage(Forge* F, Player* player)
    {
        F->Push(player->GetMoney());
        return 1;
    }

    /**
     * Returns the [Player]s current [Guild] ID
     *
     * @return uint32 guildId
     */
    int GetGuildId(Forge* F, Player* player)
    {
        F->Push(player->GetGuildId());
        return 1;
    }

    /**
     * Returns the [Player]s [TeamId]
     *
     * @return [TeamId] teamId
     */
    int GetTeam(Forge* F, Player* player)
    {
        F->Push(player->GetTeamId());
        return 1;
    }

    /**
     * Returns amount of the specified [Item] the [Player] has.
     *
     * @param uint32 entry : entry of the item
     * @param bool checkinBank = false : also counts the items in player's bank if true
     * @return uint32 itemamount
     */
    int GetItemCount(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);
        bool checkinBank = F->CHECKVAL<bool>(3, false);
        F->Push(player->GetItemCount(entry, checkinBank));
        return 1;
    }

    /**
     * Returns the [Player]s lifetime Honorable Kills
     *
     * @return uint32 lifeTimeKils
     */
    int GetLifetimeKills(Forge* F, Player* player)
    {
        F->Push(player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS));
        return 1;
    }

    /**
     * Returns the [Player]s IP address
     *
     * @return string ip
     */
    int GetPlayerIP(Forge* F, Player* player)
    {
        F->Push(player->GetSession()->GetRemoteAddress());
        return 1;
    }

    /**
     * Returns the [Player]s time played at current level
     *
     * @return uint32 currLevelPlayTime
     */
    int GetLevelPlayedTime(Forge* F, Player* player)
    {
        F->Push(player->GetLevelPlayedTime());
        return 1;
    }

    /**
     * Returns the [Player]s total time played
     *
     * @return uint32 totalPlayTime
     */
    int GetTotalPlayedTime(Forge* F, Player* player)
    {
        F->Push(player->GetTotalPlayedTime());
        return 1;
    }

    /**
     * Returns the [Player]s [Guild] object
     *
     * @return [Guild] guild
     */
    int GetGuild(Forge* F, Player* player)
    {
        F->Push(eGuildMgr->GetGuildById(player->GetGuildId()));
        return 1;
    }

    /**
     * Returns the [Player]s [Group] object
     *
     * @return [Group] group
     */
    int GetGroup(Forge* F, Player* player)
    {
        F->Push(player->GetGroup());
        return 1;
    }

    /**
     * Returns the [Player]s account ID
     *
     * @return uint32 accountId
     */
    int GetAccountId(Forge* F, Player* player)
    {
        F->Push(player->GetSession()->GetAccountId());
        return 1;
    }

    /**
     * Returns the [Player]s account name
     *
     * @return string accountName
     */
    int GetAccountName(Forge* F, Player* player)
    {
        std::string accName;
        if (eAccountMgr->GetName(player->GetSession()->GetAccountId(), accName))
            F->Push(accName);

        return 1;
    }

    /**
     * Returns the [Player]s [Corpse] object
     *
     * @return [Corpse] corpse
     */
    int GetCorpse(Forge* F, Player* player)
    {
        F->Push(player->GetCorpse());
        return 1;
    }

    /**
     * Returns the [Player]s database locale index
     *
     * @return int localeIndex
     */
    int GetDbLocaleIndex(Forge* F, Player* player)
    {
        F->Push(player->GetSession()->GetSessionDbLocaleIndex());
        return 1;
    }

    /**
     * Returns the [Player]s game client locale
     *
     * @return [LocaleConstant] locale
     */
    int GetDbcLocale(Forge* F, Player* player)
    {
        F->Push(player->GetSession()->GetSessionDbcLocale());
        return 1;
    }

    /**
     * Returns the [Player]s recruit-a-friend recruiter account ID
     *
     * @return uint32 recruiterId
     */
    int GetRecruiterId(Forge* F, Player* player)
    {
        F->Push(player->GetSession()->GetRecruiterId());
        return 1;
    }

    /**
     * Returns the [Player]s selected [Player] or nil.
     *
     * @return [Player] selection
     */
    int GetSelectedPlayer(Forge* F, Player* player)
    {
        F->Push(player->GetSelectedPlayer());
        return 1;
    }

    /**
     * Returns the [Player]s selected [Unit].
     *
     * @return [Unit] selection
     */
    int GetSelectedUnit(Forge* F, Player* player)
    {
        F->Push(player->GetSelectedUnit());
        return 1;
    }

    /**
     * Returns the closest [GameObject] to the [Player].
     *
     * @return [GameObject] gameobject
     */
    int GetNearbyGameObject(Forge* F, Player* player)
    {
        F->Push(ChatHandler(player->GetSession()).GetNearbyGameObject());
        return 1;
    }

    /**
     * Returns the amount of mails in the [Player]s mailbox
     *
     * @return uint32 count
     */
    int GetMailCount(Forge* F, Player* player)
    {
        F->Push(player->GetMailSize());
        return 1;
    }

    /**
     * Returns the [Player]s current experience points
     *
     * @return uint32 xp
     */
    int GetXP(Forge* F, Player* player)
    {
        F->Push(player->GetXP());
        return 1;
    }

    /**
     * Returns the [Player]s required experience points for next level
     *
     * @return uint32 xp
     */
    int GetXPForNextLevel(Forge* F, Player* player)
    {
        F->Push(player->GetXPForNextLevel());
        return 1;
    }

    /**
     * Locks the player controls and disallows all movement and casting.
     *
     * @param bool apply = true : lock if true and unlock if false
     */
    int SetPlayerLock(Forge* F, Player* player)
    {
        bool apply = F->CHECKVAL<bool>(2, true);

        if (apply)
        {
            player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_SILENCED);
            player->SetClientControl(player, 0);
        }
        else
        {
            player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_SILENCED);
            player->SetClientControl(player, 1);
        }
        return 0;
    }

    /**
     * Sets the [Player]s login flag to the flag specified
     *
     * @param uint32 flag
     */
    int SetAtLoginFlag(Forge* F, Player* player)
    {
        uint32 flag = F->CHECKVAL<uint32>(2);

        player->SetAtLoginFlag((AtLoginFlags)flag);
        return 0;
    }

    /**
     * Sets the [Player]s sheathe state to the state specified
     *
     * @param uint32 sheatheState
     */
    int SetSheath(Forge* F, Player* player)
    {
        uint32 sheathed = F->CHECKVAL<uint32>(2);
        if (sheathed >= MAX_SHEATH_STATE)
            return 0;

        player->SetSheath((SheathState)sheathed);
        return 0;
    }

    /**
     * Sets the [Player]s intoxication level to the level specified
     *
     * @param uint8 drunkValue
     */
    int SetDrunkValue(Forge* F, Player* player)
    {
        uint8 newDrunkValue = F->CHECKVAL<uint8>(2);

        player->SetDrunkValue(newDrunkValue);
        return 0;
    }

    /**
     * Sets the [Player]s faction standing to that of the race specified
     *
     * @param uint8 raceId
     */
    int SetFactionForRace(Forge* F, Player* player)
    {
        uint8 race = F->CHECKVAL<uint8>(2);

        player->SetFactionForRace(race);
        return 0;
    }

    /**
     * Sets (increases) skill of the [Player]
     *
     * @param uint16 id
     * @param uint16 step
     * @param uint16 currVal
     * @param uint16 maxVal
     */
    int SetSkill(Forge* F, Player* player)
    {
        uint16 id = F->CHECKVAL<uint16>(2);
        uint16 step = F->CHECKVAL<uint16>(3);
        uint16 currVal = F->CHECKVAL<uint16>(4);
        uint16 maxVal = F->CHECKVAL<uint16>(5);

        player->SetSkill(id, step, currVal, maxVal);
        return 0;
    }

    /**
     * Sets the [Player]s guild rank to the rank specified
     *
     * @param uint8 rank
     */
    int SetGuildRank(Forge* F, Player* player) // TODO: Move to Guild Methods
    {
        uint8 rank = F->CHECKVAL<uint8>(2);

        if (!player->GetGuildId())
            return 0;

        player->SetGuildRank(rank);
        return 0;
    }

    /**
     * Sets the [Player]s free talent points to the amount specified for the current spec
     *
     * @param uint32 talentPointAmt
     */
    int SetFreeTalentPoints(Forge* F, Player* player)
    {
        uint32 points = F->CHECKVAL<uint32>(2);

        player->SetFreeTalentPoints(points);
        player->SendTalentsInfoData(false);
        return 0;
    }

    /**
     * Sets the [Player]s reputation amount for the faction specified
     *
     * @param uint32 factionId
     * @param int32 reputationValue
     */
    int SetReputation(Forge* F, Player* player)
    {
        uint32 faction = F->CHECKVAL<uint32>(2);
        int32 value = F->CHECKVAL<int32>(3);
        FactionEntry const* factionEntry = sFactionStore.LookupEntry(faction);

        player->GetReputationMgr().SetReputation(factionEntry, value);
        return 0;
    }

    /**
     * Sets [Quest] state
     *
     * @param uint32 entry : entry of a quest
     * @param uint32 status
     */
    int SetQuestStatus(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);
        uint32 status = F->CHECKVAL<uint32>(3);
        if (status >= MAX_QUEST_STATUS)
            return 0;

        player->SetQuestStatus(entry, (QuestStatus)status);
        return 0;
    }

    /**
     * Sets the [Player]s rest bonus to the amount specified
     *
     * @param float restBonus
     */
    int SetRestBonus(Forge* F, Player* player)
    {
        float bonus = F->CHECKVAL<float>(2);

        player->SetRestBonus(bonus);
        return 0;
    }

    /**
     * Toggles whether the [Player] accepts whispers or not
     *
     * @param bool acceptWhispers = true
     */
    int SetAcceptWhispers(Forge* F, Player* player)
    {
        bool on = F->CHECKVAL<bool>(2, true);

        player->SetAcceptWhispers(on);
        return 0;
    }

    /**
     * Toggles PvP Death
     *
     * @param bool on = true
     */
    int SetPvPDeath(Forge* F, Player* player)
    {
        bool on = F->CHECKVAL<bool>(2, true);

        player->SetPvPDeath(on);
        return 0;
    }

    /**
     * Toggles whether the [Player] has GM visibility on or off
     *
     * @param bool gmVisible = true
     */
    int SetGMVisible(Forge* F, Player* player)
    {
        bool on = F->CHECKVAL<bool>(2, true);

        player->SetGMVisible(on);
        return 0;
    }

    /**
     * Toggles whether the [Player] has taxi cheat enabled or not
     *
     * @param bool taxiCheat = true
     */
    int SetTaxiCheat(Forge* F, Player* player)
    {
        bool on = F->CHECKVAL<bool>(2, true);

        player->SetTaxiCheater(on);
        return 0;
    }

    /**
     * Toggle Blizz (GM) tag
     *
     * @param bool on = true
     */
    int SetGMChat(Forge* F, Player* player)
    {
        bool on = F->CHECKVAL<bool>(2, true);

        player->SetGMChat(on);
        return 0;
    }

    /**
     * Toggles the [Player]s GM mode on or off
     *
     * @param bool setGmMode = true
     */
    int SetGameMaster(Forge* F, Player* player)
    {
        bool on = F->CHECKVAL<bool>(2, true);

        player->SetGameMaster(on);
        return 0;
    }

    /**
     * Sets the [Player]s gender to gender specified
     *
     * @table
     * @columns [Gender, ID]
     * @values [GENDER_MALE, 0]
     * @values [GENDER_FEMALE, 1]
     *
     * @param [Gender] gender
     */
    int SetGender(Forge* F, Player* player)
    {
        uint32 _gender = F->CHECKVAL<uint32>(2);

        Gender gender;
        switch (_gender)
        {
            case 0:
                gender = GENDER_MALE;
                break;
            case 1:
                gender = GENDER_FEMALE;
                break;
            default:
                return luaL_argerror(F->L, 2, "valid Gender expected");
        }

        player->SetByteValue(UNIT_FIELD_BYTES_0, 2, gender);
        player->SetByteValue(PLAYER_BYTES_3, 0, gender);
        player->InitDisplayIds();
        return 0;
    }

    /**
     * Sets the [Player]s Arena Points to the amount specified
     *
     * @param uint32 arenaPoints
     */
    int SetArenaPoints(Forge* F, Player* player)
    {
        uint32 arenaP = F->CHECKVAL<uint32>(2);
        player->SetArenaPoints(arenaP);
        return 0;
    }

    /**
     * Sets the [Player]s Honor Points to the amount specified
     *
     * @param uint32 honorPoints
     */
    int SetHonorPoints(Forge* F, Player* player)
    {
        uint32 honorP = F->CHECKVAL<uint32>(2);
        player->SetHonorPoints(honorP);
        return 0;
    }

    /**
     * Sets the [Player]s amount of Lifetime Honorable Kills to the value specified
     *
     * @param uint32 honorableKills
     */
    int SetLifetimeKills(Forge* F, Player* player)
    {
        uint32 val = F->CHECKVAL<uint32>(2);
        player->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS, val);
        return 0;
    }

    /**
     * Sets the [Player]s amount of money to copper specified
     *
     * @param uint32 copperAmt
     */
    int SetCoinage(Forge* F, Player* player)
    {
        using MoneyType = std::tuple_element_t<1, boost::callable_traits::args_t<decltype(&Player::SetMoney)>>;

        MoneyType amt = F->CHECKVAL<MoneyType>(2);
        player->SetMoney(amt);
        return 0;
    }

    /**
     * Sets the [Player]s home location to the location specified
     *
     * @param float x : X Coordinate
     * @param float y : Y Coordinate
     * @param float z : Z Coordinate
     * @param uint32 mapId : Map ID
     * @param uint32 areaId : Area ID
     */
    int SetBindPoint(Forge* F, Player* player)
    {
        float x = F->CHECKVAL<float>(2);
        float y = F->CHECKVAL<float>(3);
        float z = F->CHECKVAL<float>(4);
        uint32 mapId = F->CHECKVAL<uint32>(5);
        uint32 areaId = F->CHECKVAL<uint32>(6);

        WorldLocation loc(mapId, x, y, z);

        player->SetHomebind(loc, areaId);
        return 0;
    }

    /**
     * Adds the specified title to the [Player]s list of known titles
     *
     * @param uint32 titleId
     */
    int SetKnownTitle(Forge* F, Player* player)
    {
        uint32 id = F->CHECKVAL<uint32>(2);
        CharTitlesEntry const* t = sCharTitlesStore.LookupEntry(id);
        if (t)
            player->SetTitle(t, false);
        return 0;
    }

    /**
     * Adds the specified achievement to the [Player]s
     *
     * @param uint32 achievementid
     */
    int SetAchievement(Forge* F, Player* player)
    {
        uint32 id = F->CHECKVAL<uint32>(2);
        AchievementEntry const* t = sAchievementStore.LookupEntry(id);
        if (t)
            player->CompletedAchievement(t);
        return 0;
    }

    /**
     * Toggle the [Player]s FFA flag
     *
     * @param bool applyFFA = true
     */
    int SetFFA(Forge* F, Player* player)
    {
        bool apply = F->CHECKVAL<bool>(2, true);

        if(apply)
            player->SetPvpFlag(UNIT_BYTE2_FLAG_FFA_PVP);
        else
            player->RemovePvpFlag(UNIT_BYTE2_FLAG_FFA_PVP);
        return 0;
    }

    /**
     * Sets the [Player]s movement to the provided movement type
     *
     * @table
     * @columns [movementType, ID]
     * @values [MOVE_ROOT, 1]
     * @values [MOVE_UNROOT, 2]
     * @values [MOVE_WATER_WALK, 3]
     * @values [MOVE_LAND_WALK, 4]
     *
     * @param int32 movementType
     */
    int SetMovement(Forge* F, Player* player)
    {
        int32 pType = F->CHECKVAL<int32>(2);

        switch (pType)
        {
            case 1: // MOVE_ROOT
                player->SetRooted(true);
                break;
            case 2: // MOVE_UNROOT
                player->SetRooted(false);
                break;
            case 3: // MOVE_WATER_WALK
                player->SetWaterWalking(true);
                break;
            case 4: // MOVE_LAND_WALK
                player->SetWaterWalking(false);
                break;
            default:
                break;
        }

        return 0;
    }

    /**
     * Resets the [Player]s pets talent points
     */
    int ResetPetTalents(Forge* /*E*/, Player* player)
    {
        player->ResetPetTalents();
        player->SendTalentsInfoData(true);
        return 0;
    }

    /**
     * Reset the [Player]s completed achievements
     */
    int ResetAchievements(Forge* /*E*/, Player* player)
    {
        player->ResetAchievements();
        return 0;
    }

    /**
     * Shows the mailbox window to the player from specified guid.
     *
     * @param ObjectGuid guid = playerguid : guid of the mailbox window sender
     */
    int SendShowMailBox(Forge* F, Player* player)
    {
        ObjectGuid guid = F->CHECKVAL<ObjectGuid>(2, player->GET_GUID());

        player->GetSession()->SendShowMailBox(guid);
        return 0;
    }

    /**
     * Adds or detracts from the [Player]s current Arena Points
     *
     * @param int32 amount
     */
    int ModifyArenaPoints(Forge* F, Player* player)
    {
        int32 amount = F->CHECKVAL<int32>(2);

        player->ModifyArenaPoints(amount);
        return 0;
    }

    /**
     * Adds or detracts from the [Player]s current Honor Points
     *
     * @param int32 amount
     */
    int ModifyHonorPoints(Forge* F, Player* player)
    {
        int32 amount = F->CHECKVAL<int32>(2);

        player->ModifyHonorPoints(amount);
        return 0;
    }

    /**
     * Saves the [Player] to the database
     */
    int SaveToDB(Forge* /*E*/, Player* player)
    {
        player->SaveToDB();
        return 0;
    }

    /**
     * Sends a summon request to the player from the given summoner
     *
     * @param [Unit] summoner
     */
    int SummonPlayer(Forge* F, Player* player)
    {
        Unit* summoner = F->CHECKOBJ<Unit>(2);

        player->SendSummonRequestFrom(summoner);
        return 0;
    }

    /**
     * Mutes the [Player] for the amount of seconds specified
     *
     * @param uint32 muteTime
     */
    int Mute(Forge* F, Player* player)
    {
        uint32 muteseconds = F->CHECKVAL<uint32>(2);

        time_t muteTime = time(NULL) + muteseconds;
        player->GetSession()->m_muteTime = muteTime;
        LoginDatabase.PExecute("UPDATE account SET mutetime = {} WHERE id = {}", muteTime, player->GetSession()->GetAccountId());
        return 0;
    }

    /**
     * Rewards the given quest entry for the [Player] if he has completed it.
     *
     * @param uint32 entry : quest entry
     */
    int RewardQuest(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        Quest const* quest = eObjectMgr->GetQuestTemplate(entry);

        // If player doesn't have the quest
        if (!quest || player->GetQuestStatus(entry) != QUEST_STATUS_COMPLETE)
            return 0;

        player->RewardQuest(quest, 0, player);
        return 0;
    }

    /**
     * Sends an auction house window to the [Player] from the [Unit] specified
     *
     * @param [Unit] sender
     */
    int SendAuctionMenu(Forge* F, Player* player)
    {
        Unit* unit = F->CHECKOBJ<Unit>(2);

        player->GetSession()->SendAuctionHello(unit->GET_GUID(), unit);
        return 0;
    }

    /**
     * Sends a flightmaster window to the [Player] from the [Creature] specified
     *
     * @param [Creature] sender
     */
    int SendTaxiMenu(Forge* F, Player* player)
    {
        Creature* creature = F->CHECKOBJ<Creature>(2);

        player->GetSession()->SendTaxiMenu(creature);
        return 0;
    }

    /**
     * Sends a spirit resurrection request to the [Player]
     */
    int SendSpiritResurrect(Forge* /*E*/, Player* player)
    {
        player->GetSession()->SendSpiritResurrect();
        return 0;
    }

    /**
     * Sends a tabard vendor window to the [Player] from the [WorldObject] specified
     *
     * @param [WorldObject] sender
     */
    int SendTabardVendorActivate(Forge* F, Player* player)
    {
        WorldObject* obj = F->CHECKOBJ<WorldObject>(2);

        player->GetSession()->SendTabardVendorActivate(obj->GET_GUID());
        return 0;
    }

    /**
     * Sends a bank window to the [Player] from the [WorldObject] specified.
     *
     * @param [WorldObject] sender
     */
    int SendShowBank(Forge* F, Player* player)
    {
        WorldObject* obj = F->CHECKOBJ<WorldObject>(2);

        player->GetSession()->SendShowBank(obj->GET_GUID());
        return 0;
    }

    /**
     * Sends a vendor window to the [Player] from the [WorldObject] specified.
     *
     * @param [WorldObject] sender
     */
    int SendListInventory(Forge* F, Player* player)
    {
        WorldObject* obj = F->CHECKOBJ<WorldObject>(2);

        player->GetSession()->SendListInventory(obj->GET_GUID());
        return 0;
    }

    /**
     * Sends a trainer window to the [Player] from the [Creature] specified
     *
     * @param [Creature] sender
     */
    int SendTrainerList(Forge* F, Player* player)
    {
        Creature* obj = F->CHECKOBJ<Creature>(2);

        player->GetSession()->SendTrainerList(obj);
        return 0;
    }

    /**
     * Sends a guild invitation from the [Player]s [Guild] to the [Player] object specified
     *
     * @param [Player] invitee
     */
    int SendGuildInvite(Forge* F, Player* player)
    {
        Player* plr = F->CHECKOBJ<Player>(2);

        if (Guild* guild = player->GetGuild())
            guild->HandleInviteMember(player->GetSession(), plr->GetName());

        return 0;
    }

    /**
     * Sends an update for the world state to the [Player]
     *
     * @param uint32 field
     * @param uint32 value
     */
    int SendUpdateWorldState(Forge* F, Player* player)
    {
        uint32 field = F->CHECKVAL<uint32>(2);
        uint32 value = F->CHECKVAL<uint32>(3);

        player->SendUpdateWorldState(field, value);
        return 0;
    }

    /**
     * Forces the [Player] to log out
     *
     * @param bool saveToDb = true
     */
    int LogoutPlayer(Forge* F, Player* player)
    {
        bool save = F->CHECKVAL<bool>(2, true);

        player->GetSession()->LogoutPlayer(save);
        return 0;
    }

    /**
     * Forcefully removes the [Player] from a [BattleGround] raid group
     */
    int RemoveFromBattlegroundRaid(Forge* /*E*/, Player* player)
    {
        player->RemoveFromBattlegroundOrBattlefieldRaid();
        return 0;
    }

    /**
     * Unbinds the [Player] from his instances except the one he currently is in.
     *
     * Difficulty is not used on classic.
     *
     * @param uint32 map = true
     * @param uint32 difficulty = 0
     */
    int UnbindInstance(Forge* F, Player* player)
    {
        uint32 map = F->CHECKVAL<uint32>(2);
        uint32 difficulty = F->CHECKVAL<uint32>(3, 0);

        if (difficulty < MAX_DIFFICULTY)
            player->UnbindInstance(map, (Difficulty)difficulty);

        return 0;
    }

    /**
     * Unbinds the [Player] from his instances except the one he currently is in.
     */
    int UnbindAllInstances(Forge* /*E*/, Player* player)
    {
        for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
        {
            Player::BoundInstancesMap& binds = player->GetBoundInstances(Difficulty(i));
            for (Player::BoundInstancesMap::iterator itr = binds.begin(); itr != binds.end();)
            {
                if (itr->first != player->GetMapId())
                    player->UnbindInstance(itr, Difficulty(i));
                else
                    ++itr;
            }
        }
        return 0;
    }

    /**
     * Forces the [Player] to leave a [BattleGround]
     *
     * @param bool teleToEntry = true
     */
    int LeaveBattleground(Forge* F, Player* player)
    {
        bool teleToEntryPoint = F->CHECKVAL<bool>(2, true);

        player->LeaveBattleground(teleToEntryPoint);
        return 0;
    }

    /**
     * Repairs [Item] at specified position.
     *
     * @param uint16 position
     * @param bool cost = true
     * @param float discountMod = 1.0
     */
    int DurabilityRepair(Forge* F, Player* player)
    {
        uint16 position = F->CHECKVAL<uint16>(2);
        bool takeCost = F->CHECKVAL<bool>(3, true);
        float discountMod = F->CHECKVAL<float>(4, 1.0f);

        player->DurabilityRepair(position, takeCost, discountMod);
        return 0;
    }

    /**
     * Repairs all [Item]s.
     *
     * @param bool takeCost = true
     * @param float discountMod = 1.0
     * @param bool guidBank = false
     */
    int DurabilityRepairAll(Forge* F, Player* player)
    {
        bool takeCost = F->CHECKVAL<bool>(2, true);
        float discountMod = F->CHECKVAL<float>(3, 1.0f);
        bool guildBank = F->CHECKVAL<bool>(4, false);

        player->DurabilityRepairAll(takeCost, discountMod, guildBank);
        return 0;
    }

    /**
     * Sets durability loss for an [Item] in the specified slot
     *
     * @param int32 slot
     */
    int DurabilityPointLossForEquipSlot(Forge* F, Player* player)
    {
        int32 slot = F->CHECKVAL<int32>(2);

        if (slot >= EQUIPMENT_SLOT_START && slot < EQUIPMENT_SLOT_END)
            player->DurabilityPointLossForEquipSlot((EquipmentSlots)slot);
        return 0;
    }

    /**
     * Sets durability loss on all [Item]s equipped
     *
     * If inventory is true, sets durability loss for [Item]s in bags
     *
     * @param int32 points
     * @param bool inventory = true
     */
    int DurabilityPointsLossAll(Forge* F, Player* player)
    {
        int32 points = F->CHECKVAL<int32>(2);
        bool inventory = F->CHECKVAL<bool>(3, true);

        player->DurabilityPointsLossAll(points, inventory);
        return 0;
    }

    /**
     * Sets durability loss for the specified [Item]
     *
     * @param [Item] item
     * @param int32 points
     */
    int DurabilityPointsLoss(Forge* F, Player* player)
    {
        Item* item = F->CHECKOBJ<Item>(2);
        int32 points = F->CHECKVAL<int32>(3);

        player->DurabilityPointsLoss(item, points);
        return 0;
    }

    /**
     * Damages specified [Item]
     *
     * @param [Item] item
     * @param double percent
     */
    int DurabilityLoss(Forge* F, Player* player)
    {
        Item* item = F->CHECKOBJ<Item>(2);
        double percent = F->CHECKVAL<double>(3);

        player->DurabilityLoss(item, percent);
        return 0;
    }

    /**
     * Damages all [Item]s equipped. If inventory is true, damages [Item]s in bags
     *
     * @param double percent
     * @param bool inventory = true
     */
    int DurabilityLossAll(Forge* F, Player* player)
    {
        double percent = F->CHECKVAL<double>(2);
        bool inventory = F->CHECKVAL<bool>(3, true);

        player->DurabilityLossAll(percent, inventory);
        return 0;
    }

    /**
     * Kills the [Player]
     */
    int KillPlayer(Forge* /*E*/, Player* player)
    {
        player->KillPlayer();
        return 0;
    }

    /**
     * Forces the [Player] to leave a [Group]
     */
    int RemoveFromGroup(Forge* /*E*/, Player* player)
    {
        if (!player->GetGroup())
            return 0;

        player->RemoveFromGroup();
        return 0;
    }

    /**
     * Returns the [Player]s accumulated talent reset cost
     *
     * @return uint32 resetCost
     */
    int ResetTalentsCost(Forge* F, Player* player)
    {
        F->Push(player->ResetTalentsCost());
        return 1;
    }

    /**
     * Resets the [Player]s talents
     *
     * @param bool noCost = true
     */
    int ResetTalents(Forge* F, Player* player)
    {
        bool no_cost = F->CHECKVAL<bool>(2, true);

        player->ResetTalents(no_cost);
        player->SendTalentsInfoData(false);
        return 0;
    }

    /**
     * Removes the [Spell] from the [Player]
     *
     * @param uint32 entry : entry of a [Spell]
     */
    int RemoveSpell(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        player->RemoveSpell(entry);
        return 0;
    }

    /**
     * Clears the [Player]s combo points
     */
    int ClearComboPoints(Forge* /*E*/, Player* player)
    {
        player->ClearComboPoints();
        return 0;
    }

    /**
     * Adds combo points to the [Player]
     *
     * @param [Unit] target
     * @param int8 count
     */
    int AddComboPoints(Forge* F, Player* player)
    {
        Unit* target = F->CHECKOBJ<Unit>(2);
        int8 count = F->CHECKVAL<int8>(3);

        player->AddComboPoints(target, count);
        return 0;
    }

    /**
     * Gives [Quest] monster talked to credit
     *
     * @param uint32 entry : entry of a [Creature]
     * @param [Creature] creature
     */
    int TalkedToCreature(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);
        Creature* creature = F->CHECKOBJ<Creature>(3);

        player->TalkedToCreature(entry, creature->GET_GUID());
        return 0;
    }

    /**
     * Gives [Quest] monster killed credit
     *
     * @param uint32 entry : entry of a [Creature]
     */
    int KilledMonsterCredit(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        player->KilledMonsterCredit(entry, player->GET_GUID());
        return 0;
    }

    /**
     * Completes a [Quest] if in a [Group]
     *
     * @param uint32 quest : entry of a quest
     * @param [WorldObject] obj
     */
    int GroupEventHappens(Forge* F, Player* player)
    {
        uint32 questId = F->CHECKVAL<uint32>(2);
        WorldObject* obj = F->CHECKOBJ<WorldObject>(3);

        player->GroupEventHappens(questId, obj);
        return 0;
    }

    /**
     * Completes the [Quest] if a [Quest] area is explored, or completes the [Quest]
     *
     * @param uint32 quest : entry of a [Quest]
     */
    int AreaExploredOrEventHappens(Forge* F, Player* player)
    {
        uint32 questId = F->CHECKVAL<uint32>(2);

        player->AreaExploredOrEventHappens(questId);
        return 0;
    }

    /**
     * Sets the given [Quest] entry failed for the [Player].
     *
     * @param uint32 entry : entry of a [Quest]
     */
    int FailQuest(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        player->FailQuest(entry);
        return 0;
    }

    /**
     * Sets the given quest entry incomplete for the [Player].
     *
     * @param uint32 entry : quest entry
     */
    int IncompleteQuest(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        player->IncompleteQuest(entry);
        return 0;
    }

    /**
     * Completes the given quest entry for the [Player] and tries to satisfy all quest requirements.
     *
     * The player should have the quest to complete it.
     *
     * @param uint32 entry : quest entry
     */
    int CompleteQuest(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        Quest const* quest = eObjectMgr->GetQuestTemplate(entry);

        // If player doesn't have the quest
        if (!quest || player->GetQuestStatus(entry) == QUEST_STATUS_NONE)
            return 0;

        // Add quest items for quests that require items
        for (uint8 x = 0; x < QUEST_ITEM_OBJECTIVES_COUNT; ++x)
        {
            uint32 id = quest->RequiredItemId[x];
            uint32 count = quest->RequiredItemCount[x];

            if (!id || !count)
                continue;

            uint32 curItemCount = player->GetItemCount(id, true);

            ItemPosCountVec dest;
            uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, id, count - curItemCount);
            if (msg == EQUIP_ERR_OK)
            {
                Item* item = player->StoreNewItem(dest, id, true);
                player->SendNewItem(item, count - curItemCount, true, false);
            }
        }

        // All creature/GO slain/cast (not required, but otherwise it will display "Creature slain 0/10")
        for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        {
            int32 creature = quest->RequiredNpcOrGo[i];
            uint32 creatureCount = quest->RequiredNpcOrGoCount[i];

            if (creature > 0)
            {
                if (CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(creature))
                    for (uint16 z = 0; z < creatureCount; ++z)
                        player->KilledMonster(creatureInfo, ObjectGuid::Empty);
            }
            else if (creature < 0)
                for (uint16 z = 0; z < creatureCount; ++z)
                    player->KillCreditGO(creature);
        }


        // If the quest requires reputation to complete
        if (uint32 repFaction = quest->GetRepObjectiveFaction())
        {
            uint32 repValue = quest->GetRepObjectiveValue();
            uint32 curRep = player->GetReputationMgr().GetReputation(repFaction);
            if (curRep < repValue)
                if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(repFaction))
                    player->GetReputationMgr().SetReputation(factionEntry, repValue);
        }

        // If the quest requires a SECOND reputation to complete
        if (uint32 repFaction = quest->GetRepObjectiveFaction2())
        {
            uint32 repValue2 = quest->GetRepObjectiveValue2();
            uint32 curRep = player->GetReputationMgr().GetReputation(repFaction);
            if (curRep < repValue2)
                if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(repFaction))
                    player->GetReputationMgr().SetReputation(factionEntry, repValue2);
        }

        // If the quest requires money
        int32 ReqOrRewMoney = quest->GetRewOrReqMoney();
        if (ReqOrRewMoney < 0)
            player->ModifyMoney(-ReqOrRewMoney);

        if (sWorld->getBoolConfig(CONFIG_QUEST_ENABLE_QUEST_TRACKER)) // check if Quest Tracker is enabled
        {
            // prepare Quest Tracker datas
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_QUEST_TRACK_GM_COMPLETE);
            stmt->setUInt32(0, quest->GetQuestId());
            stmt->setUInt32(1, player->GetGUID().GetCounter());

            // add to Quest Tracker
            CharacterDatabase.Execute(stmt);
        }

        player->CompleteQuest(entry);
        return 0;
    }

    /**
     * Tries to add the given quest entry for the [Player].
     *
     * @param uint32 entry : quest entry
     */
    int AddQuest(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        Quest const* quest = eObjectMgr->GetQuestTemplate(entry);

        if (!quest)
            return 0;

        // check item starting quest (it can work incorrectly if added without item in inventory)
        ItemTemplateContainer const& itc = sObjectMgr->GetItemTemplateStore();
        auto itr = std::find_if(std::begin(itc), std::end(itc), [quest](ItemTemplateContainer::value_type const& value)
            {
                return value.second.StartQuest == quest->GetQuestId();
            });

        if (itr != std::end(itc))
            return 0;

        // ok, normal (creature/GO starting) quest
        if (player->CanAddQuest(quest, true))
            player->AddQuestAndCheckCompletion(quest, NULL);

        return 0;
    }

    /**
     * Removes the given quest entry from the [Player].
     *
     * @param uint32 entry : quest entry
     */
    int RemoveQuest(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        Quest const* quest = eObjectMgr->GetQuestTemplate(entry);

        if (!quest)
            return 0;

        // remove all quest entries for 'entry' from quest log
        for (uint8 slot = 0; slot < MAX_QUEST_LOG_SIZE; ++slot)
        {
            uint32 logQuest = player->GetQuestSlotQuestId(slot);
            if (logQuest == entry)
            {
                player->SetQuestSlot(slot, 0);

                // we ignore unequippable quest items in this case, its' still be equipped
                player->TakeQuestSourceItem(logQuest, false);

                if (quest->HasFlag(QUEST_FLAGS_FLAGS_PVP))
                {
                    player->pvpInfo.IsHostile = player->pvpInfo.IsInHostileArea || player->HasPvPForcingQuest();
                    player->UpdatePvPState();
                }
            }
        }

        player->RemoveActiveQuest(entry, false);
        player->RemoveRewardedQuest(entry);
        return 0;
    }

    /**
     * Sends whisper text from the [Player]
     *
     * @param string text
     * @param uint32 lang : language the [Player] will speak
     * @param [Player] receiver : is the [Player] that will receive the whisper, if TrinityCore
     * @param ObjectGuid guid : is the GUID of a [Player] that will receive the whisper, not TrinityCore
     */
    int Whisper(Forge* F, Player* player)
    {
        std::string text = F->CHECKVAL<std::string>(2);
        uint32 lang = F->CHECKVAL<uint32>(3);
        Player* receiver = F->CHECKOBJ<Player>(4);

        player->Whisper(text, (Language)lang, receiver);
        return 0;
    }

    /**
     * Sends a text emote from the [Player]
     *
     * @param string emoteText
     */
    int TextEmote(Forge* F, Player* player)
    {
        std::string text = F->CHECKVAL<std::string>(2);

        player->TextEmote(text);
        return 0;
    }

    /**
     * Sends yell text from the [Player]
     *
     * @param string text : text for the [Player] to yells
     * @param uint32 lang : language the [Player] will speak
     */
    int Yell(Forge* F, Player* player)
    {
        std::string text = F->CHECKVAL<std::string>(2);
        uint32 lang = F->CHECKVAL<uint32>(3);

        player->Yell(text, (Language)lang);
        return 0;
    }

    /**
     * Sends say text from the [Player]
     *
     * @param string text : text for the [Player] to say
     * @param uint32 lang : language the [Player] will speak
     */
    int Say(Forge* F, Player* player)
    {
        std::string text = F->CHECKVAL<std::string>(2);
        uint32 lang = F->CHECKVAL<uint32>(3);

        player->Say(text, (Language)lang);
        return 0;
    }

    /**
     * Gives the [Player] experience
     *
     * @param uint32 xp : experience to give
     * @param [Unit] victim = nil
     */
    int GiveXP(Forge* F, Player* player)
    {
        uint32 xp = F->CHECKVAL<uint32>(2);
        Unit* victim = F->CHECKOBJ<Unit>(3, false);

        player->GiveXP(xp, victim);
        return 0;
    }

    /**
     * Toggle the [Player]s 'Do Not Disturb' flag
     */
    int ToggleDND(Forge* /*E*/, Player* player)
    {
        player->ToggleDND();
        return 0;
    }

    /**
     * Toggle the [Player]s 'Away From Keyboard' flag
     */
    int ToggleAFK(Forge* /*E*/, Player* player)
    {
        player->ToggleAFK();
        return 0;
    }

    /**
     * Equips the given item or item entry to the given slot. Returns the equipped item or nil.
     *
     * @table
     * @columns [EquipSlot, ID]
     * @values [EQUIPMENT_SLOT_HEAD, 0]
     * @values [EQUIPMENT_SLOT_NECK, 1]
     * @values [EQUIPMENT_SLOT_SHOULDERS, 2]
     * @values [EQUIPMENT_SLOT_BODY, 3]
     * @values [EQUIPMENT_SLOT_CHEST, 4]
     * @values [EQUIPMENT_SLOT_WAIST, 5]
     * @values [EQUIPMENT_SLOT_LEGS, 6]
     * @values [EQUIPMENT_SLOT_FEET, 7]
     * @values [EQUIPMENT_SLOT_WRISTS, 8]
     * @values [EQUIPMENT_SLOT_HANDS, 9]
     * @values [EQUIPMENT_SLOT_FINGER1, 10]
     * @values [EQUIPMENT_SLOT_FINGER2, 11]
     * @values [EQUIPMENT_SLOT_TRINKET1, 12]
     * @values [EQUIPMENT_SLOT_TRINKET2, 13]
     * @values [EQUIPMENT_SLOT_BACK, 14]
     * @values [EQUIPMENT_SLOT_MAINHAND, 15]
     * @values [EQUIPMENT_SLOT_OFFHAND, 16]
     * @values [EQUIPMENT_SLOT_RANGED, 17]
     * @values [EQUIPMENT_SLOT_TABARD, 18]
     *
     * @table
     * @columns [BagSlot, ID]
     * @values [INVENTORY_SLOT_BAG_START, 19]
     * @values [INVENTORY_SLOT_BAG_END, 23]
     *
     * @proto equippedItem = (item, slot)
     * @proto equippedItem = (entry, slot)
     * @param [Item] item : item to equip
     * @param uint32 entry : entry of the item to equip
     * @param uint32 slot : equipment slot to equip the item to The slot can be [EquipmentSlots] or [InventorySlots]
     * @return [Item] equippedItem : item or nil if equipping failed
     */
    int EquipItem(Forge* F, Player* player)
    {
        uint16 dest = 0;
        Item* item = F->CHECKOBJ<Item>(2, false);
        uint32 slot = F->CHECKVAL<uint32>(3);

        if (slot >= INVENTORY_SLOT_BAG_END)
            return 1;

        if (!item)
        {
            uint32 entry = F->CHECKVAL<uint32>(2);
            item = Item::CreateItem(entry, 1, player);
            if (!item)
                return 1;

            InventoryResult result = player->CanEquipItem(slot, dest, item, false);
            if (result != EQUIP_ERR_OK)
            {
                delete item;
                return 1;
            }
            player->ItemAddedQuestCheck(entry, 1);
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM, entry, 1);
        }
        else
        {
            InventoryResult result = player->CanEquipItem(slot, dest, item, false);
            if (result != EQUIP_ERR_OK)
                return 1;
            player->RemoveItem(item->GetBagSlot(), item->GetSlot(), true);
        }

        F->Push(player->EquipItem(dest, item, true));
        player->AutoUnequipOffhandIfNeed();
        return 1;
    }

    /**
     * Returns true if the player can equip the given [Item] or item entry to the given slot, false otherwise.
     *
     * @proto canEquip = (item, slot)
     * @proto canEquip = (entry, slot)
     * @param [Item] item : item to equip
     * @param uint32 entry : entry of the item to equip
     * @param uint32 slot : equipment slot to test
     * @return bool canEquip
     */
    int CanEquipItem(Forge* F, Player* player)
    {
        Item* item = F->CHECKOBJ<Item>(2, false);
        uint32 slot = F->CHECKVAL<uint32>(3);
        if (slot >= EQUIPMENT_SLOT_END)
        {
            F->Push(false);
            return 1;
        }

        if (!item)
        {
            uint32 entry = F->CHECKVAL<uint32>(2);
            uint16 dest;
            InventoryResult msg = player->CanEquipNewItem(slot, dest, entry, false);
            if (msg != EQUIP_ERR_OK)
            {
                F->Push(false);
                return 1;
            }
        }
        else
        {
            uint16 dest;
            InventoryResult msg = player->CanEquipItem(slot, dest, item, false);
            if (msg != EQUIP_ERR_OK)
            {
                F->Push(false);
                return 1;
            }
        }
        F->Push(true);
        return 1;
    }

    /**
     * Removes a title by ID from the [Player]s list of known titles
     *
     * @param uint32 titleId
     */
    int UnsetKnownTitle(Forge* F, Player* player)
    {
        uint32 id = F->CHECKVAL<uint32>(2);
        CharTitlesEntry const* t = sCharTitlesStore.LookupEntry(id);
        if (t)
            player->SetTitle(t, true);
        return 0;
    }

    /**
     * Advances all of the [Player]s weapon skills to the maximum amount available
     */
    int AdvanceSkillsToMax(Forge* /*E*/, Player* player)
    {
        player->UpdateWeaponsSkillsToMaxSkillsForLevel();
        return 0;
    }

    /**
     * Advances all of the [Player]s skills to the amount specified
     *
     * @param uint32 skillStep
     */
    int AdvanceAllSkills(Forge* F, Player* player)
    {
        uint32 step = F->CHECKVAL<uint32>(2);

        if (!step)
            return 0;

        for (uint32 i = 0; i < sSkillLineStore.GetNumRows(); ++i)
        {
            if (SkillLineEntry const* entry = sSkillLineStore.LookupEntry(i))
            {
                if (entry->CategoryID == SKILL_CATEGORY_LANGUAGES || entry->CategoryID == SKILL_CATEGORY_GENERIC)
                    continue;

                if (player->HasSkill(entry->ID))
                    player->UpdateSkill(entry->ID, step);
            }
        }

        return 0;
    }

    /**
     * Advances a [Player]s specific skill to the amount specified
     *
     * @param uint32 skillId
     * @param uint32 skillStep
     */
    int AdvanceSkill(Forge* F, Player* player)
    {
        uint32 _skillId = F->CHECKVAL<uint32>(2);
        uint32 _step = F->CHECKVAL<uint32>(3);
        if (_skillId && _step)
        {
            if (player->HasSkill(_skillId))
                player->UpdateSkill(_skillId, _step);
        }
        return 0;
    }

    /**
     * Teleports a [Player] to the location specified
     *
     * @param uint32 mappId
     * @param float xCoord
     * @param float yCoord
     * @param float zCoord
     * @param float orientation
     */
    int Teleport(Forge* F, Player* player)
    {
        uint32 mapId = F->CHECKVAL<uint32>(2);
        float x = F->CHECKVAL<float>(3);
        float y = F->CHECKVAL<float>(4);
        float z = F->CHECKVAL<float>(5);
        float o = F->CHECKVAL<float>(6);

        if (player->IsInFlight())
            player->FinishTaxiFlight();
        else
            player->SaveRecallPosition();

        F->Push(player->TeleportTo(mapId, x, y, z, o));
        return 1;
    }

    /**
     * Adds or detracts from the [Player]s current lifetime kill count
     *
     * @param int32 kills : Positive number to add, negative number to detract
     */
    int AddLifetimeKills(Forge* F, Player* player)
    {
        int32 val = F->CHECKVAL<int32>(2);
        uint32 currentKills = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
        uint32 kills = static_cast<uint32>((static_cast<int32>(currentKills) + val) < 0 ? 0 : currentKills + val);

        player->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS, kills);
        return 0;
    }

    /**
     * Adds the given amount of the specified item entry to the player.
     *
     * @param uint32 entry : entry of the item to add
     * @param uint32 itemCount = 1 : amount of the item to add
     * @return [Item] item : the item that was added or nil
     */
    int AddItem(Forge* F, Player* player)
    {
        uint32 itemId = F->CHECKVAL<uint32>(2);
        uint32 itemCount = F->CHECKVAL<uint32>(3, 1);

        uint32 noSpaceForCount = 0;
        ItemPosCountVec dest;
        InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, itemCount, &noSpaceForCount);
        if (msg != EQUIP_ERR_OK)
            itemCount -= noSpaceForCount;

        if (itemCount == 0 || dest.empty())
            return 1;

        Item* item = player->StoreNewItem(dest, itemId, true, GenerateItemRandomPropertyId(itemId));

        if (item)
            player->SendNewItem(item, itemCount, true, false);

        F->Push(item);
        return 1;
    }

    /**
     * Removes the given amount of the specified [Item] from the player.
     *
     * @proto (item, itemCount)
     * @proto (entry, itemCount)
     * @param [Item] item : item to remove
     * @param uint32 entry : entry of the item to remove
     * @param uint32 itemCount = 1 : amount of the item to remove
     */
    int RemoveItem(Forge* F, Player* player)
    {
        Item* item = F->CHECKOBJ<Item>(2, false);
        uint32 itemCount = F->CHECKVAL<uint32>(3);
        if (!item)
        {
            uint32 itemId = F->CHECKVAL<uint32>(2);
            player->DestroyItemCount(itemId, itemCount, true);
        }
        else
        {
            player->DestroyItemCount(item, itemCount, true);
        }
        return 0;
    }

    /**
     * Removes specified amount of lifetime kills
     *
     * @param uint32 val : kills to remove
     */
    int RemoveLifetimeKills(Forge* F, Player* player)
    {
        uint32 val = F->CHECKVAL<uint32>(2);
        uint32 currentKills = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
        if (val > currentKills)
            val = currentKills;
        player->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS, currentKills - val);
        return 0;
    }

    /**
     * Resets cooldown of the specified spell
     *
     * @param uint32 spellId
     * @param bool update = true
     */
    int ResetSpellCooldown(Forge* F, Player* player)
    {
        uint32 spellId = F->CHECKVAL<uint32>(2);
        bool update = F->CHECKVAL<bool>(3, true);

        player->GetSpellHistory()->ResetCooldown(spellId, update);
        return 0;
    }

    /**
     * Resets cooldown of the specified category
     *
     * @param uint32 category
     * @param bool update = true
     */
    int ResetTypeCooldowns(Forge* F, Player* player)
    {
        uint32 category = F->CHECKVAL<uint32>(2);
        bool update = F->CHECKVAL<bool>(3, true);
        (void)update; // ensure that the variable is referenced in order to pass compiler checks

        player->GetSpellHistory()->ResetCooldowns([category](SpellHistory::CooldownStorageType::iterator itr) -> bool
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first);
            return spellInfo && spellInfo->GetCategory() == category;
        }, update);

        return 0;
    }

    /**
     * Resets all of the [Player]'s cooldowns
     */
    int ResetAllCooldowns(Forge* /*E*/, Player* player)
    {
        player->GetSpellHistory()->ResetAllCooldowns();
        return 0;
    }

    /**
     * Sends a Broadcast Message to the [Player]
     *
     * @param string message
     */
    int SendBroadcastMessage(Forge* F, Player* player)
    {
        const char* message = F->CHECKVAL<const char*>(2);
        if (std::string(message).length() > 0)
            ChatHandler(player->GetSession()).SendSysMessage(message);
        return 0;
    }

    /**
     * Sends an Area Trigger Message to the [Player]
     *
     * @param string message
     */
    int SendAreaTriggerMessage(Forge* F, Player* player)
    {
        std::string msg = F->CHECKVAL<std::string>(2);
        if (msg.length() > 0)
            player->GetSession()->SendAreaTriggerMessage("%s", msg.c_str());
        return 0;
    }

    /**
     * Sends a Notification to the [Player]
     *
     * @param string message
     */
    int SendNotification(Forge* F, Player* player)
    {
        std::string msg = F->CHECKVAL<std::string>(2);
        if (msg.length() > 0)
            player->GetSession()->SendNotification("%s", msg.c_str());
        return 0;
    }

    /**
     * Sends a [WorldPacket] to the [Player]
     *
     * @param [WorldPacket] packet
     * @param bool selfOnly = true
     */
    int SendPacket(Forge* F, Player* player)
    {
        WorldPacket* data = F->CHECKOBJ<WorldPacket>(2);
        bool selfOnly = F->CHECKVAL<bool>(3, true);

        if (selfOnly)
            player->GetSession()->SendPacket(data);
        else
            player->SendMessageToSet(data, true);

        return 0;
    }

    /**
     * Sends addon message to the [Player] receiver
     *
     * @param string prefix
     * @param string message
     * @param [ChatMsg] channel
     * @param [Player] receiver
     *
     */
    int SendAddonMessage(Forge* F, Player* player)
    {
        std::string prefix = F->CHECKVAL<std::string>(2);
        std::string message = F->CHECKVAL<std::string>(3);
        ChatMsg channel = ChatMsg(F->CHECKVAL<uint8>(4));
        Player* receiver = F->CHECKOBJ<Player>(5);
        std::string fullmsg = prefix + "\t" + message;
        WorldPacket data;
        ChatHandler::BuildChatPacket(data, channel, LANG_ADDON, player, receiver, fullmsg);
        receiver->GetSession()->SendPacket(&data);
        return 0;
    }

    /**
     * Kicks the [Player] from the server
     */
    int KickPlayer(Forge* /*E*/, Player* player)
    {
        player->GetSession()->KickPlayer("PlayerMethods::KickPlayer Kick the player");
        return 0;
    }

    /**
     * Adds or subtracts from the [Player]s money in copper
     *
     * @param int32 copperAmt : negative to remove, positive to add
     */
    int ModifyMoney(Forge* F, Player* player)
    {
        using MoneyType = std::tuple_element_t<1, boost::callable_traits::args_t<decltype(&Player::ModifyMoney)>>;

        MoneyType amt = F->CHECKVAL<MoneyType>(2);

        player->ModifyMoney(amt);
        return 1;
    }

    /**
     * Teaches the [Player] the [Spell] specified by entry ID
     *
     * @param uint32 spellId
     */
    int LearnSpell(Forge* F, Player* player)
    {
        uint32 id = F->CHECKVAL<uint32>(2);

        player->LearnSpell(id, false);
        return 0;
    }

    /**
     * Learn the [Player] the talent specified by talent_id and talentRank
     *
     * @param uint32 talent_id
     * @param uint32 talentRank
     */
    int LearnTalent(Forge* F, Player* player)
    {
        uint32 id = F->CHECKVAL<uint32>(2);
        uint32 rank = F->CHECKVAL<uint32>(3);

        player->LearnTalent(id, rank);
        player->SendTalentsInfoData(false);
        return 0;
    }

    /**
     * Remove cooldowns on spells that have less than 10 minutes of cooldown from the [Player], similarly to when you enter an arena.
     */
    int RemoveArenaSpellCooldowns(Forge* /*E*/, Player* player)
    {
        player->RemoveArenaSpellCooldowns();
        return 0;
    }

    /**
     * Resurrects the [Player].
     *
     * @param float healthPercent = 100.0f
     * @param bool ressSickness = false
     */
    int ResurrectPlayer(Forge* F, Player* player)
    {
        float percent = F->CHECKVAL<float>(2, 100.0f);
        bool sickness = F->CHECKVAL<bool>(3, false);
        player->ResurrectPlayer(percent, sickness);
        player->SpawnCorpseBones();
        return 0;
    }

    /**
     * Adds a new item to the gossip menu shown to the [Player] on next call to [Player:GossipSendMenu].
     *
     * sender and intid are numbers which are passed directly to the gossip selection handler. Internally they are partly used for the database gossip handling.<br />
     * code specifies whether to show a box to insert text to. The player inserted text is passed to the gossip selection handler.<br />
     * money specifies an amount of money the player needs to have to click the option. An error message is shown if the player doesn't have enough money.<br />
     * Note that the money amount is only checked client side and is not removed from the player either. You will need to check again in your code before taking action.
     *
     * See also: [Player:GossipSendMenu], [Player:GossipAddQuests], [Player:GossipComplete], [Player:GossipClearMenu]
     *
     * @param uint32 icon : number that specifies used icon
     * @param string msg : label on the gossip item
     * @param uint32 sender : number passed to gossip handlers
     * @param uint32 intid : number passed to gossip handlers
     * @param bool code = false : show text input on click if true
     * @param string popup = nil : if non empty string, a popup with given text shown on click
     * @param uint32 money = 0 : required money in copper
     */
    int GossipMenuAddItem(Forge* F, Player* player)
    {
        uint32 _icon = F->CHECKVAL<uint32>(2);
        const char* msg = F->CHECKVAL<const char*>(3);
        uint32 _sender = F->CHECKVAL<uint32>(4);
        uint32 _intid = F->CHECKVAL<uint32>(5);
        bool _code = F->CHECKVAL<bool>(6, false);
        const char* _promptMsg = F->CHECKVAL<const char*>(7, "");
        uint32 _money = F->CHECKVAL<uint32>(8, 0);

        player->PlayerTalkClass->GetGossipMenu().AddMenuItem(-1, GossipOptionIcon(_icon), msg, _sender, _intid, _promptMsg, _money, _code);
        return 0;
    }

    /**
     * Closes the [Player]s currently open Gossip Menu.
     *
     * See also: [Player:GossipMenuAddItem], [Player:GossipAddQuests], [Player:GossipSendMenu], [Player:GossipClearMenu]
     */
    int GossipComplete(Forge* /*E*/, Player* player)
    {
        player->PlayerTalkClass->SendCloseGossip();
        return 0;
    }

    /**
     * Sends the current gossip items of the player to him as a gossip menu with header text from the given textId.
     *
     * If sender is a [Player] then menu_id is mandatory, otherwise it is not used for anything.
     * menu_id is the ID used to trigger the OnGossipSelect registered for players. See [Global:RegisterPlayerGossipEvent]
     *
     * See also: [Player:GossipMenuAddItem], [Player:GossipAddQuests], [Player:GossipComplete], [Player:GossipClearMenu]
     *
     * @proto (npc_text, sender)
     * @proto (npc_text, sender, menu_id)
     * @param uint32 npc_text : entry ID of a header text in npc_text database table, common default is 100
     * @param [Object] sender : object acting as the source of the sent gossip menu
     * @param uint32 menu_id : if sender is a [Player] then menu_id is mandatory
     */
    int GossipSendMenu(Forge* F, Player* player)
    {
        uint32 npc_text = F->CHECKVAL<uint32>(2);
        Object* sender = F->CHECKOBJ<Object>(3);
        if (sender->GetTypeId() == TYPEID_PLAYER)
        {
            uint32 menu_id = F->CHECKVAL<uint32>(4);
            player->PlayerTalkClass->GetGossipMenu().SetMenuId(menu_id);
        }

        player->PlayerTalkClass->SendGossipMenu(npc_text, sender->GET_GUID());
        return 0;
    }

    /**
     * Clears the [Player]s current gossip item list.
     *
     * See also: [Player:GossipMenuAddItem], [Player:GossipSendMenu], [Player:GossipAddQuests], [Player:GossipComplete]
     *
     *     Note: This is needed when you show a gossip menu without using gossip hello or select hooks which do this automatically.
     *     Usually this is needed when using [Player] is the sender of a Gossip Menu.
     */
    int GossipClearMenu(Forge* /*E*/, Player* player)
    {
        player->PlayerTalkClass->ClearMenus();
        return 0;
    }

    /**
     * Attempts to start the taxi/flying to the given pathID
     *
     * @param uint32 pathId : pathId from DBC or [Global:AddTaxiPath]
     */
    int StartTaxi(Forge* F, Player* player)
    {
        uint32 pathId = F->CHECKVAL<uint32>(2);

        player->ActivateTaxiPathTo(pathId);
        return 0;
    }

    /**
     * Sends POI to the location on your map
     *
     * @param float x
     * @param float y
     * @param uint32 icon : map icon to show
     * @param uint32 flags
     * @param uint32 data
     * @param string iconText
     */
    int GossipSendPOI(Forge* F, Player* player)
    {
        float x = F->CHECKVAL<float>(2);
        float y = F->CHECKVAL<float>(3);
        uint32 icon = F->CHECKVAL<uint32>(4);
        uint32 flags = F->CHECKVAL<uint32>(5);
        uint32 data = F->CHECKVAL<uint32>(6);
        std::string iconText = F->CHECKVAL<std::string>(7);

        WorldPackets::NPC::GossipPOI packet;
        packet.Name = iconText;
        packet.Flags = flags;
        packet.Pos.Pos.Relocate(x, y);
        packet.Icon = icon;
        packet.Importance = data;

        player->SendDirectMessage(packet.Write());
        return 0;
    }

    /**
     * Adds the gossip items to the [Player]'s gossip for the quests the given [WorldObject] can offer to the player.
     *
     * @param [WorldObject] source : a questgiver with quests
     */
    int GossipAddQuests(Forge* F, Player* player)
    {
        WorldObject* source = F->CHECKOBJ<WorldObject>(2);

        if (source->GetTypeId() == TYPEID_UNIT)
        {
            if (source->GetUInt32Value(UNIT_NPC_FLAGS) & UNIT_NPC_FLAG_QUESTGIVER)
                player->PrepareQuestMenu(source->GET_GUID());
        }
        else if (source->GetTypeId() == TYPEID_GAMEOBJECT)
        {
            if (source->ToGameObject()->GetGoType() == GAMEOBJECT_TYPE_QUESTGIVER)
                player->PrepareQuestMenu(source->GET_GUID());
        }
        return 0;
    }

    /**
     * Shows a quest accepting window to the [Player] for the given quest.
     *
     * @param uint32 questId : entry of a quest
     * @param bool activateAccept = true : auto finish the quest
     */
    int SendQuestTemplate(Forge* F, Player* player)
    {
        uint32 questId = F->CHECKVAL<uint32>(2);
        bool activateAccept = F->CHECKVAL<bool>(3, true);

        Quest const* quest = eObjectMgr->GetQuestTemplate(questId);
        if (!quest)
            return 0;

        player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, player->GET_GUID(), activateAccept);
        return 0;
    }

    /**
     * Converts [Player]'s corpse to bones
     */
    int SpawnBones(Forge* /*E*/, Player* player)
    {
        player->SpawnCorpseBones();
        return 0;
    }

    /**
     * Loots [Player]'s bones for insignia
     *
     * @param [Player] looter
     */
    int RemovedInsignia(Forge* F, Player* player)
    {
        Player* looter = F->CHECKOBJ<Player>(2);
        player->RemovedInsignia(looter);
        return 0;
    }

    /**
     * Makes the [Player] invite another player to a group.
     *
     * In multistate, this method is only available in the WORLD state
     *
     * @param [Player] invited : player to invite to group
     * @return bool success : true if the player was invited to a group
     */
    int GroupInvite(Forge* F, Player* player)
    {
        Player* invited = F->CHECKOBJ<Player>(2);

        if (invited->GetGroup() || invited->GetGroupInvite())
        {
            F->Push(false);
            return 1;
        }

        // Get correct existing group if any
        Group* group = player->GetGroup();

        if (group && group->isBGGroup())
            group = player->GetOriginalGroup();

        bool success = false;

        // Try invite if group found
        if (group)
            success = !group->IsFull() && group->AddInvite(invited);
        else
        {
            // Create new group if one not found
            group = new Group;
            success = group->AddLeaderInvite(player) && group->AddInvite(invited);
            if (!success)
                delete group;
        }

        if (success)
        {
            WorldPackets::Party::PartyInvite partyInvite;
            partyInvite.Initialize(player, 0, true);
            invited->SendDirectMessage(partyInvite.Write());
        }

        F->Push(success);
        return 1;
    }

    /**
     * Creates a new [Group] with the creator [Player] as leader.
     *
     * In multistate, this method is only available in the WORLD state
     *
     * @param [Player] invited : player to add to group
     * @return [Group] createdGroup : the created group or nil
     */
    int GroupCreate(Forge* F, Player* player)
    {
        Player* invited = F->CHECKOBJ<Player>(2);

        if (player->GetGroup() || invited->GetGroup())
            return 0;

        if (player->GetGroupInvite())
            player->UninviteFromGroup();
        if (invited->GetGroupInvite())
            invited->UninviteFromGroup();

        // Try create new group
        Group* group = new Group;
        if (!group->AddLeaderInvite(player))
        {
            delete group;
            return 0;
        }

        // Forming a new group, create it
        if (!group->IsCreated())
        {
            group->RemoveInvite(player);
            group->Create(player);
            sGroupMgr->AddGroup(group);
        }

        if (!group->AddMember(invited))
            return 0;

        group->BroadcastGroupUpdate();

        F->Push(group);
        return 1;
    }

    /**
     * Starts a cinematic for the [Player]
     *
     * @param uint32 CinematicSequenceId : entry of a cinematic
     */
    int SendCinematicStart(Forge* F, Player* player)
    {
        uint32 CinematicSequenceId = F->CHECKVAL<uint32>(2);

        player->SendCinematicStart(CinematicSequenceId);
        return 0;
    }

    /**
     * Starts a movie for the [Player]
     *
     * @param uint32 MovieId : entry of a movie
     */
    int SendMovieStart(Forge* F, Player* player)
    {
        uint32 MovieId = F->CHECKVAL<uint32>(2);

        player->SendMovieStart(MovieId);
        return 0;
    }

    /**
     * Binds the [Player] to their current instance.
     */
    int BindToInstance(Forge* /*E*/, Player* player)
    {
        player->BindToInstance();
        return 0;
    }

    /**
     * Adds a talent to the [Player] for the specified spec and learning status.
     *
     * @param uint32 spellId : ID of the spell for the talent
     * @param uint8 spec : The spec to which the talent applies
     * @param bool learning = true : Whether the talent is being learned
     * @return bool success : True if the talent was added, false otherwise
     */
    int AddTalent(Forge* F, Player* player)
    {
        uint32 spellId = F->CHECKVAL<uint32>(2);
        uint8 spec = F->CHECKVAL<uint8>(3);
        bool learning = F->CHECKVAL<bool>(4, true);

        if (spec >= MAX_TALENT_SPECS)
            F->Push(false);
        else
            F->Push(player->AddTalent(spellId, spec, learning));

        return 1;
    }

    /**
     * Grants kill credit for a specific [Craeture] or [GameObject].
     *
     * @param uint32 entryId : the ID of the [Creature] or [GameObject] to award credit for.
     */
    int KillGOCredit(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);
        ObjectGuid guid = F->CHECKVAL<ObjectGuid>(3, ObjectGuid::Empty);

        player->KillCreditGO(entry, guid);
        return 0;
    }

    /**
     * Grants a player kill credit.
     */
    int KilledPlayerCredit(Forge* /*E*/, Player* player)
    {
        player->KilledPlayerCredit();
        return 0;
    }

    /**
     * Removes a quest from the rewarded quests for the [Player].
     *
     * @param uint32 questId : the ID of the quest to remove.
     */
    int RemoveRewardedQuest(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        player->RemoveRewardedQuest(entry);
        return 0;
    }

    /**
     * Removes an active quest from the [Player].
     *
     * @param uint32 questId : the ID of the quest to remove.
     */
    int RemoveActiveQuest(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);

        player->RemoveActiveQuest(entry);
        return 0;
    }

    /**
     * Summons a pet for the [Player].
     *
     * @table
     * @columns [Summon Type, ID]
     * @values [SUMMON_PET, 0]
     * @values [HUNTER_PET, 1]
     *
     * @param uint32 entryId : the ID of the pet to summon.
     * @param float x
     * @param float y
     * @param float z
     * @param float o
     * @param uint32 petType
     * @param uint32 despawnTime
     */
    int SummonPet(Forge* F, Player* player)
    {
        uint32 entry = F->CHECKVAL<uint32>(2);
        float x = F->CHECKVAL<float>(3);
        float y = F->CHECKVAL<float>(4);
        float z = F->CHECKVAL<float>(5);
        float o = F->CHECKVAL<float>(6);
        uint32 petType = F->CHECKVAL<uint32>(7);
        uint32 despwtime = F->CHECKVAL<uint32>(8);

        if (petType >= MAX_PET_TYPE)
            return 0;

        player->SummonPet(entry, x, y, z, o, (PetType)petType, despwtime);
        return 0;
    }

    /**
     * Removes the [Player]'s active pet.
     *
     * @table
     * @columns [Save mode, ID, Comment]
     * @values [PET_SAVE_AS_DELETED, -1, "Not saved"]
     * @values [PET_SAVE_AS_CURRENT, 0, "In current slot (with the player)"]
     * @values [PET_SAVE_FIRST_STABLE_SLOT, 1, ""]
     * @values [PET_SAVE_SECOND_STABLE_SLOT, 2, ""]
     * @values [PET_SAVE_THIRD_STABLE_SLOT, 3, ""]
     * @values [PET_SAVE_LAST_STABLE_SLOT, 4, ""]
     *
     * @param int saveMode = -1
     * @param bool returnReagent = false
     *
     */
    int RemovePet(Forge* F, Player* player)
    {
        int mode = F->CHECKVAL<int>(2, PET_SAVE_AS_DELETED);
        bool returnreagent = F->CHECKVAL<bool>(3, false);

        if (!player->GetPet())
            return 0;

        player->RemovePet(player->GetPet(), (PetSaveMode)mode, returnreagent);
        return 0;
    }

    /**
     * Runs a command as the [Player].
     *
     * @param string command : the command to run
     */
    int RunCommand(Forge* F, Player* player)
    {
        const char* command = F->CHECKVAL<const char*>(2);
        if (std::string(command).length() > 0)
            ChatHandler(player->GetSession())._ParseCommands(command);
        return 0;
    }

    ForgeRegister<Player> PlayerMethods[] =
    {
        // Getters
        { "GetSelection", &LuaPlayer::GetSelection },
        { "GetGMRank", &LuaPlayer::GetGMRank },
        { "GetGuildId", &LuaPlayer::GetGuildId },
        { "GetCoinage", &LuaPlayer::GetCoinage },
        { "GetTeam", &LuaPlayer::GetTeam },
        { "GetItemCount", &LuaPlayer::GetItemCount },
        { "GetGroup", &LuaPlayer::GetGroup },
        { "GetGuild", &LuaPlayer::GetGuild },
        { "GetAccountId", &LuaPlayer::GetAccountId },
        { "GetAccountName", &LuaPlayer::GetAccountName },
        { "GetArenaPoints", &LuaPlayer::GetArenaPoints },
        { "GetHonorPoints", &LuaPlayer::GetHonorPoints },
        { "GetLifetimeKills", &LuaPlayer::GetLifetimeKills },
        { "GetPlayerIP", &LuaPlayer::GetPlayerIP },
        { "GetLevelPlayedTime", &LuaPlayer::GetLevelPlayedTime },
        { "GetTotalPlayedTime", &LuaPlayer::GetTotalPlayedTime },
        { "GetItemByPos", &LuaPlayer::GetItemByPos },
        { "GetItemByEntry", &LuaPlayer::GetItemByEntry },
        { "GetItemByGUID", &LuaPlayer::GetItemByGUID },
        { "GetMailItem", &LuaPlayer::GetMailItem },
        { "GetReputation", &LuaPlayer::GetReputation },
        { "GetEquippedItemBySlot", &LuaPlayer::GetEquippedItemBySlot },
        { "GetQuestLevel", &LuaPlayer::GetQuestLevel },
        { "GetChatTag", &LuaPlayer::GetChatTag },
        { "GetRestBonus", &LuaPlayer::GetRestBonus },
        { "GetPhaseMaskForSpawn", &LuaPlayer::GetPhaseMaskForSpawn },
        { "GetReqKillOrCastCurrentCount", &LuaPlayer::GetReqKillOrCastCurrentCount },
        { "GetQuestStatus", &LuaPlayer::GetQuestStatus },
        { "GetInGameTime", &LuaPlayer::GetInGameTime },
        { "GetComboPoints", &LuaPlayer::GetComboPoints },
        { "GetComboTarget", &LuaPlayer::GetComboTarget },
        { "GetGuildName", &LuaPlayer::GetGuildName },
        { "GetFreeTalentPoints", &LuaPlayer::GetFreeTalentPoints },
        { "GetActiveSpec", &LuaPlayer::GetActiveSpec },
        { "GetSpecsCount", &LuaPlayer::GetSpecsCount },
        { "GetSpellCooldownDelay", &LuaPlayer::GetSpellCooldownDelay },
        { "GetGuildRank", &LuaPlayer::GetGuildRank },
        { "GetDifficulty", &LuaPlayer::GetDifficulty },
        { "GetHealthBonusFromStamina", &LuaPlayer::GetHealthBonusFromStamina },
        { "GetManaBonusFromIntellect", &LuaPlayer::GetManaBonusFromIntellect },
        { "GetMaxSkillValue", &LuaPlayer::GetMaxSkillValue },
        { "GetPureMaxSkillValue", &LuaPlayer::GetPureMaxSkillValue },
        { "GetSkillValue", &LuaPlayer::GetSkillValue },
        { "GetBaseSkillValue", &LuaPlayer::GetBaseSkillValue },
        { "GetPureSkillValue", &LuaPlayer::GetPureSkillValue },
        { "GetSkillPermBonusValue", &LuaPlayer::GetSkillPermBonusValue },
        { "GetSkillTempBonusValue", &LuaPlayer::GetSkillTempBonusValue },
        { "GetReputationRank", &LuaPlayer::GetReputationRank },
        { "GetDrunkValue", &LuaPlayer::GetDrunkValue },
        { "GetBattlegroundId", &LuaPlayer::GetBattlegroundId },
        { "GetBattlegroundTypeId", &LuaPlayer::GetBattlegroundTypeId },
        { "GetXPRestBonus", &LuaPlayer::GetXPRestBonus },
        { "GetGroupInvite", &LuaPlayer::GetGroupInvite },
        { "GetSubGroup", &LuaPlayer::GetSubGroup },
        { "GetNextRandomRaidMember", &LuaPlayer::GetNextRandomRaidMember },
        { "GetOriginalGroup", &LuaPlayer::GetOriginalGroup },
        { "GetOriginalSubGroup", &LuaPlayer::GetOriginalSubGroup },
        { "GetChampioningFaction", &LuaPlayer::GetChampioningFaction },
        { "GetLatency", &LuaPlayer::GetLatency },
        { "GetRecruiterId", &LuaPlayer::GetRecruiterId },
        { "GetDbLocaleIndex", &LuaPlayer::GetDbLocaleIndex },
        { "GetDbcLocale", &LuaPlayer::GetDbcLocale },
        { "GetCorpse", &LuaPlayer::GetCorpse },
        { "GetGossipTextId", &LuaPlayer::GetGossipTextId },
        { "GetQuestRewardStatus", &LuaPlayer::GetQuestRewardStatus },
        { "GetShieldBlockValue", &LuaPlayer::GetShieldBlockValue },
        { "GetMailCount", &LuaPlayer::GetMailCount },
        { "GetXP", &LuaPlayer::GetXP },
        { "GetXPForNextLevel", &LuaPlayer::GetXPForNextLevel },

        // Setters
        { "AdvanceSkillsToMax", &LuaPlayer::AdvanceSkillsToMax },
        { "AdvanceSkill", &LuaPlayer::AdvanceSkill },
        { "AdvanceAllSkills", &LuaPlayer::AdvanceAllSkills },
        { "AddLifetimeKills", &LuaPlayer::AddLifetimeKills },
        { "SetCoinage", &LuaPlayer::SetCoinage },
        { "SetKnownTitle", &LuaPlayer::SetKnownTitle },
        { "UnsetKnownTitle", &LuaPlayer::UnsetKnownTitle },
        { "SetBindPoint", &LuaPlayer::SetBindPoint },
        { "SetArenaPoints", &LuaPlayer::SetArenaPoints },
        { "SetHonorPoints", &LuaPlayer::SetHonorPoints },
        { "SetLifetimeKills", &LuaPlayer::SetLifetimeKills },
        { "SetGameMaster", &LuaPlayer::SetGameMaster },
        { "SetGMChat", &LuaPlayer::SetGMChat },
        { "SetTaxiCheat", &LuaPlayer::SetTaxiCheat },
        { "SetGMVisible", &LuaPlayer::SetGMVisible },
        { "SetPvPDeath", &LuaPlayer::SetPvPDeath },
        { "SetAcceptWhispers", &LuaPlayer::SetAcceptWhispers },
        { "SetRestBonus", &LuaPlayer::SetRestBonus },
        { "SetQuestStatus", &LuaPlayer::SetQuestStatus },
        { "SetReputation", &LuaPlayer::SetReputation },
        { "SetFreeTalentPoints", &LuaPlayer::SetFreeTalentPoints },
        { "SetGuildRank", &LuaPlayer::SetGuildRank },
        { "SetMovement", &LuaPlayer::SetMovement },
        { "SetSkill", &LuaPlayer::SetSkill },
        { "SetFactionForRace", &LuaPlayer::SetFactionForRace },
        { "SetDrunkValue", &LuaPlayer::SetDrunkValue },
        { "SetAtLoginFlag", &LuaPlayer::SetAtLoginFlag },
        { "SetPlayerLock", &LuaPlayer::SetPlayerLock },
        { "SetGender", &LuaPlayer::SetGender },
        { "SetSheath", &LuaPlayer::SetSheath },
        { "SetFFA", &LuaPlayer::SetFFA },

        // Boolean
        { "IsInGroup", &LuaPlayer::IsInGroup },
        { "IsInGuild", &LuaPlayer::IsInGuild },
        { "IsGM", &LuaPlayer::IsGM },
        { "IsImmuneToDamage", &LuaPlayer::IsImmuneToDamage },
        { "IsAlliance", &LuaPlayer::IsAlliance },
        { "IsHorde", &LuaPlayer::IsHorde },
        { "HasTitle", &LuaPlayer::HasTitle },
        { "HasItem", &LuaPlayer::HasItem },
        { "Teleport", &LuaPlayer::Teleport },
        { "AddItem", &LuaPlayer::AddItem },
        { "IsInArenaTeam", &LuaPlayer::IsInArenaTeam },
        { "CanCompleteQuest", &LuaPlayer::CanCompleteQuest },
        { "CanEquipItem", &LuaPlayer::CanEquipItem },
        { "IsFalling", &LuaPlayer::IsFalling },
        { "ToggleAFK", &LuaPlayer::ToggleAFK },
        { "ToggleDND", &LuaPlayer::ToggleDND },
        { "IsAFK", &LuaPlayer::IsAFK },
        { "IsDND", &LuaPlayer::IsDND },
        { "IsAcceptingWhispers", &LuaPlayer::IsAcceptingWhispers },
        { "IsGMChat", &LuaPlayer::IsGMChat },
        { "IsTaxiCheater", &LuaPlayer::IsTaxiCheater },
        { "IsGMVisible", &LuaPlayer::IsGMVisible },
        { "HasQuest", &LuaPlayer::HasQuest },
        { "InBattlegroundQueue", &LuaPlayer::InBattlegroundQueue },
        { "IsImmuneToEnvironmentalDamage", &LuaPlayer::IsImmuneToEnvironmentalDamage },
        { "CanSpeak", &LuaPlayer::CanSpeak },
        { "HasAtLoginFlag", &LuaPlayer::HasAtLoginFlag },
        { "InRandomLfgDungeon", &LuaPlayer::InRandomLfgDungeon },
        { "HasPendingBind", &LuaPlayer::HasPendingBind },
        { "HasAchieved", &LuaPlayer::HasAchieved },
        { "SetAchievement", &LuaPlayer::SetAchievement },
        { "CanUninviteFromGroup", &LuaPlayer::CanUninviteFromGroup },
        { "IsRested", &LuaPlayer::IsRested },
        { "IsNeverVisible", &LuaPlayer::IsNeverVisible },
        { "IsVisibleForPlayer", &LuaPlayer::IsVisibleForPlayer },
        { "IsUsingLfg", &LuaPlayer::IsUsingLfg },
        { "HasQuestForItem", &LuaPlayer::HasQuestForItem },
        { "HasQuestForGO", &LuaPlayer::HasQuestForGO },
        { "CanShareQuest", &LuaPlayer::CanShareQuest },
        { "HasReceivedQuestReward", &LuaPlayer::HasReceivedQuestReward },
        { "HasTalent", &LuaPlayer::HasTalent },
        { "IsInSameGroupWith", &LuaPlayer::IsInSameGroupWith },
        { "IsInSameRaidWith", &LuaPlayer::IsInSameRaidWith },
        { "IsGroupVisibleFor", &LuaPlayer::IsGroupVisibleFor },
        { "HasSkill", &LuaPlayer::HasSkill },
        { "IsHonorOrXPTarget", &LuaPlayer::IsHonorOrXPTarget },
        { "CanParry", &LuaPlayer::CanParry },
        { "CanBlock", &LuaPlayer::CanBlock },
        { "CanTitanGrip", &LuaPlayer::CanTitanGrip },
        { "InBattleground", &LuaPlayer::InBattleground },
        { "InArena", &LuaPlayer::InArena },
        { "IsOutdoorPvPActive", &LuaPlayer::IsOutdoorPvPActive },
        { "IsARecruiter", &LuaPlayer::IsARecruiter },
        { "CanUseItem", &LuaPlayer::CanUseItem },
        { "HasSpell", &LuaPlayer::HasSpell },
        { "HasSpellCooldown", &LuaPlayer::HasSpellCooldown },
        { "IsInWater", &LuaPlayer::IsInWater },
        { "CanFly", &LuaPlayer::CanFly },
        { "IsMoving", &LuaPlayer::IsMoving },
        { "IsFlying", &LuaPlayer::IsFlying },
        { "CanCompleteRepeatableQuest", &LuaPlayer::CanCompleteRepeatableQuest },
        { "CanRewardQuest", &LuaPlayer::CanRewardQuest },
        { "HasRecruited", &LuaPlayer::HasRecruited },
        { "IsRecruited", &LuaPlayer::IsRecruited },

        // Gossip
        { "GossipMenuAddItem", &LuaPlayer::GossipMenuAddItem },
        { "GossipMenuAddItemData", METHOD_REG_NONE }, // not implemented
        { "GossipSendMenu", &LuaPlayer::GossipSendMenu },
        { "GossipComplete", &LuaPlayer::GossipComplete },
        { "GossipClearMenu", &LuaPlayer::GossipClearMenu },

        // Other
        { "SendBroadcastMessage", &LuaPlayer::SendBroadcastMessage },
        { "SendAreaTriggerMessage", &LuaPlayer::SendAreaTriggerMessage },
        { "SendNotification", &LuaPlayer::SendNotification },
        { "SendPacket", &LuaPlayer::SendPacket },
        { "SendAddonMessage", &LuaPlayer::SendAddonMessage },
        { "ModifyMoney", &LuaPlayer::ModifyMoney },
        { "LearnSpell", &LuaPlayer::LearnSpell },
        { "LearnTalent", &LuaPlayer::LearnTalent },
        { "RemoveArenaSpellCooldowns", &LuaPlayer::RemoveArenaSpellCooldowns },
        { "RemoveItem", &LuaPlayer::RemoveItem },
        { "RemoveLifetimeKills", &LuaPlayer::RemoveLifetimeKills },
        { "ResurrectPlayer", &LuaPlayer::ResurrectPlayer },
        { "EquipItem", &LuaPlayer::EquipItem },
        { "ResetSpellCooldown", &LuaPlayer::ResetSpellCooldown },
        { "ResetTypeCooldowns", &LuaPlayer::ResetTypeCooldowns },
        { "ResetAllCooldowns", &LuaPlayer::ResetAllCooldowns },
        { "GiveXP", &LuaPlayer::GiveXP },
        { "RemovePet", &LuaPlayer::RemovePet },
        { "SummonPet", &LuaPlayer::SummonPet },
        { "Say", &LuaPlayer::Say },
        { "Yell", &LuaPlayer::Yell },
        { "TextEmote", &LuaPlayer::TextEmote },
        { "Whisper", &LuaPlayer::Whisper },
        { "CompleteQuest", &LuaPlayer::CompleteQuest },
        { "IncompleteQuest", &LuaPlayer::IncompleteQuest },
        { "FailQuest", &LuaPlayer::FailQuest },
        { "AddQuest", &LuaPlayer::AddQuest },
        { "RemoveQuest", &LuaPlayer::RemoveQuest },
        { "RemoveActiveQuest", &LuaPlayer::RemoveActiveQuest },
        { "RemoveRewardedQuest", &LuaPlayer::RemoveRewardedQuest },
        { "AreaExploredOrEventHappens", &LuaPlayer::AreaExploredOrEventHappens },
        { "GroupEventHappens", &LuaPlayer::GroupEventHappens },
        { "KilledMonsterCredit", &LuaPlayer::KilledMonsterCredit },
        { "KilledPlayerCredit", &LuaPlayer::KilledPlayerCredit },
        { "KillGOCredit", &LuaPlayer::KillGOCredit },
        { "TalkedToCreature", &LuaPlayer::TalkedToCreature },
        { "ResetPetTalents", &LuaPlayer::ResetPetTalents },
        { "AddComboPoints", &LuaPlayer::AddComboPoints },
        { "ClearComboPoints", &LuaPlayer::ClearComboPoints },
        { "RemoveSpell", &LuaPlayer::RemoveSpell },
        { "ResetTalents", &LuaPlayer::ResetTalents },
        { "ResetTalentsCost", &LuaPlayer::ResetTalentsCost },
        { "AddTalent", &LuaPlayer::AddTalent },
        { "RemoveFromGroup", &LuaPlayer::RemoveFromGroup },
        { "KillPlayer", &LuaPlayer::KillPlayer },
        { "DurabilityLossAll", &LuaPlayer::DurabilityLossAll },
        { "DurabilityLoss", &LuaPlayer::DurabilityLoss },
        { "DurabilityPointsLoss", &LuaPlayer::DurabilityPointsLoss },
        { "DurabilityPointsLossAll", &LuaPlayer::DurabilityPointsLossAll },
        { "DurabilityPointLossForEquipSlot", &LuaPlayer::DurabilityPointLossForEquipSlot },
        { "DurabilityRepairAll", &LuaPlayer::DurabilityRepairAll },
        { "DurabilityRepair", &LuaPlayer::DurabilityRepair },
        { "ModifyHonorPoints", &LuaPlayer::ModifyHonorPoints },
        { "ModifyArenaPoints", &LuaPlayer::ModifyArenaPoints },
        { "LeaveBattleground", &LuaPlayer::LeaveBattleground },
        { "BindToInstance", &LuaPlayer::BindToInstance },
        { "UnbindInstance", &LuaPlayer::UnbindInstance },
        { "UnbindAllInstances", &LuaPlayer::UnbindAllInstances },
        { "RemoveFromBattlegroundRaid", &LuaPlayer::RemoveFromBattlegroundRaid },
        { "ResetAchievements", &LuaPlayer::ResetAchievements },
        { "KickPlayer", &LuaPlayer::KickPlayer },
        { "LogoutPlayer", &LuaPlayer::LogoutPlayer },
        { "SendTrainerList", &LuaPlayer::SendTrainerList },
        { "SendListInventory", &LuaPlayer::SendListInventory },
        { "SendShowBank", &LuaPlayer::SendShowBank },
        { "SendTabardVendorActivate", &LuaPlayer::SendTabardVendorActivate },
        { "SendSpiritResurrect", &LuaPlayer::SendSpiritResurrect },
        { "SendTaxiMenu", &LuaPlayer::SendTaxiMenu },
        { "SendUpdateWorldState", &LuaPlayer::SendUpdateWorldState },
        { "RewardQuest", &LuaPlayer::RewardQuest },
        { "SendAuctionMenu", &LuaPlayer::SendAuctionMenu },
        { "SendShowMailBox", &LuaPlayer::SendShowMailBox },
        { "StartTaxi", &LuaPlayer::StartTaxi },
        { "GossipSendPOI", &LuaPlayer::GossipSendPOI },
        { "GossipAddQuests", &LuaPlayer::GossipAddQuests },
        { "SendQuestTemplate", &LuaPlayer::SendQuestTemplate },
        { "SpawnBones", &LuaPlayer::SpawnBones },
        { "RemovedInsignia", &LuaPlayer::RemovedInsignia },
        { "SendGuildInvite", &LuaPlayer::SendGuildInvite },
        { "Mute", &LuaPlayer::Mute },
        { "SummonPlayer", &LuaPlayer::SummonPlayer },
        { "SaveToDB", &LuaPlayer::SaveToDB },
        { "GroupInvite", &LuaPlayer::GroupInvite, METHOD_REG_WORLD }, // World state method only in multistate
        { "GroupCreate", &LuaPlayer::GroupCreate, METHOD_REG_WORLD }, // World state method only in multistate
        { "SendCinematicStart", &LuaPlayer::SendCinematicStart },
        { "SendMovieStart", &LuaPlayer::SendMovieStart },
        { "RunCommand", &LuaPlayer::RunCommand },

        // Not implemented methods
        { "GetHonorStoredKills", METHOD_REG_NONE }, // classic only
        { "GetRankPoints", METHOD_REG_NONE }, // classic only
        { "GetHonorLastWeekStandingPos", METHOD_REG_NONE }, // classic only

        { "SetHonorStoredKills", METHOD_REG_NONE }, // classic only
        { "SetRankPoints", METHOD_REG_NONE }, // classic only
        { "SetHonorLastWeekStandingPos", METHOD_REG_NONE }, // classic only

        { "CanFlyInZone", METHOD_REG_NONE }, // not implemented

        { "UpdateHonor", METHOD_REG_NONE }, // classic only
        { "ResetHonor", METHOD_REG_NONE }, // classic only
        { "ClearHonorInfo", METHOD_REG_NONE }, // classic only
        { "GainSpellComboPoints", METHOD_REG_NONE } // not implemented
    };
};
#endif
