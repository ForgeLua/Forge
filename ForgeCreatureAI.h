/*
 * Copyright (C) 2010 - 2024 Forge Lua Engine <https://forgeluaengine.github.io/>
 * This program is free software licensed under GPL version 3
 * Please see the included DOCS/LICENSE.md for more information
 */

#ifndef _FORGE_CREATURE_AI_H
#define _FORGE_CREATURE_AI_H

#include "LuaEngine.h"

struct ScriptedAI;
typedef ScriptedAI NativeScriptedAI;

struct ForgeCreatureAI : NativeScriptedAI
{
    // used to delay the spawn hook triggering on AI creation
    bool justSpawned;
    // used to delay movementinform hook (WP hook)
    std::vector< std::pair<uint32, uint32> > movepoints;

    ForgeCreatureAI(Creature* creature) : NativeScriptedAI(creature), justSpawned(true)
    {
    }
    ~ForgeCreatureAI() { }

    //Called at World update tick
    void UpdateAI(uint32 diff) override
    {
        if (!movepoints.empty())
        {
            for (auto& point : movepoints)
            {
                if (!me->GetForge()->MovementInform(me, point.first, point.second))
                    NativeScriptedAI::MovementInform(point.first, point.second);
            }
            movepoints.clear();
        }

        if (!me->GetForge()->UpdateAI(me, diff))
        {
            if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
                NativeScriptedAI::UpdateAI(diff);
        }
    }

    // Called for reaction when initially engaged - this will always happen _after_ JustEnteredCombat
    // Called at creature aggro either by MoveInLOS or Attack Start
    void JustEngagedWith(Unit* target) override
    {
        if (!me->GetForge()->EnterCombat(me, target))
            NativeScriptedAI::JustEngagedWith(target);
    }

    // Called at any Damage from any attacker (before damage apply)
    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType damageType, SpellInfo const* spellInfo) override
    {
        if (!me->GetForge()->DamageTaken(me, attacker, damage))
        {
            NativeScriptedAI::DamageTaken(attacker, damage, damageType, spellInfo);
        }
    }

    //Called at creature death
    void JustDied(Unit* killer) override
    {
        if (!me->GetForge()->JustDied(me, killer))
            NativeScriptedAI::JustDied(killer);
    }

    //Called at creature killing another unit
    void KilledUnit(Unit* victim) override
    {
        if (!me->GetForge()->KilledUnit(me, victim))
            NativeScriptedAI::KilledUnit(victim);
    }

    // Called when the creature summon successfully other creature
    void JustSummoned(Creature* summon) override
    {
        if (!me->GetForge()->JustSummoned(me, summon))
            NativeScriptedAI::JustSummoned(summon);
    }

    // Called when a summoned creature is despawned
    void SummonedCreatureDespawn(Creature* summon) override
    {
        if (!me->GetForge()->SummonedCreatureDespawn(me, summon))
            NativeScriptedAI::SummonedCreatureDespawn(summon);
    }

    //Called at waypoint reached or PointMovement end
    void MovementInform(uint32 type, uint32 id) override
    {
        // delayed since hook triggers before actually reaching the point
        // and starting new movement would bug
        movepoints.push_back(std::make_pair(type, id));
    }

    // Called before EnterCombat even before the creature is in combat.
    void AttackStart(Unit* target) override
    {
        if (!me->GetForge()->AttackStart(me, target))
            NativeScriptedAI::AttackStart(target);
    }

    // Called for reaction at stopping attack at no attackers or targets
    void EnterEvadeMode(EvadeReason /*why*/) override
    {
        if (!me->GetForge()->EnterEvadeMode(me))
            NativeScriptedAI::EnterEvadeMode();
    }

    // Called when creature appears in the world (spawn, respawn, grid load etc...)
    void JustAppeared() override
    {
        if (!me->GetForge()->JustRespawned(me))
            NativeScriptedAI::JustAppeared();
    }

    // Called at reaching home after evade
    void JustReachedHome() override
    {
        if (!me->GetForge()->JustReachedHome(me))
            NativeScriptedAI::JustReachedHome();
    }

    // Called at text emote receive from player
    void ReceiveEmote(Player* player, uint32 emoteId) override
    {
        if (!me->GetForge()->ReceiveEmote(me, player, emoteId))
            NativeScriptedAI::ReceiveEmote(player, emoteId);
    }

    // called when the corpse of this creature gets removed
    void CorpseRemoved(uint32& respawnDelay) override
    {
        if (!me->GetForge()->CorpseRemoved(me, respawnDelay))
            NativeScriptedAI::CorpseRemoved(respawnDelay);
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (!me->GetForge()->MoveInLineOfSight(me, who))
            NativeScriptedAI::MoveInLineOfSight(who);
    }

    // Called when hit by a spell
    void SpellHit(WorldObject* caster, SpellInfo const* spell) override
    {
        if (!me->GetForge()->SpellHit(me, caster, spell))
            NativeScriptedAI::SpellHit(caster, spell);
    }

    // Called when spell hits a target
    void SpellHitTarget(WorldObject* target, SpellInfo const* spell) override
    {
        if (!me->GetForge()->SpellHitTarget(me, target, spell))
            NativeScriptedAI::SpellHitTarget(target, spell);
    }

    // Called when the creature is summoned successfully by other creature
    void IsSummonedBy(WorldObject* summoner) override
    {
        if (!summoner->ToUnit() || !me->GetForge()->OnSummoned(me, summoner->ToUnit()))
            NativeScriptedAI::IsSummonedBy(summoner);
    }

    void SummonedCreatureDies(Creature* summon, Unit* killer) override
    {
        if (!me->GetForge()->SummonedCreatureDies(me, summon, killer))
            NativeScriptedAI::SummonedCreatureDies(summon, killer);
    }

    // Called when owner takes damage
    void OwnerAttackedBy(Unit* attacker) override
    {
        if (!me->GetForge()->OwnerAttackedBy(me, attacker))
            NativeScriptedAI::OwnerAttackedBy(attacker);
    }

    // Called when owner attacks something
    void OwnerAttacked(Unit* target) override
    {
        if (!me->GetForge()->OwnerAttacked(me, target))
            NativeScriptedAI::OwnerAttacked(target);
    }
};

#endif
