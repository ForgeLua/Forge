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

#include "ForgeAutoBind.h"

#ifndef AURAMETHODS_H
#define AURAMETHODS_H

/***
 * The persistent effect of a [Spell] that remains on a [Unit] after the [Spell]
 *   has been cast.
 *
 * As an example, if you cast a damage-over-time spell on a target, an [Aura] is
 *   put on the target that deals damage continuously.
 *
 * [Aura]s on your player are displayed in-game as a series of icons to the left
 *   of the mini-map.
 *
 * Inherits all methods from: none
 */
namespace LuaAura
{
    /**
     * Returns the [Unit] that casted the [Spell] that caused this [Aura] to be applied.
     *
     * @return [Unit] caster
     */
    FORGE_AUTO_GETTER(Aura, GetCaster)

    /**
     * Returns the GUID of the [Unit] that casted the [Spell] that caused this [Aura] to be applied.
     *
     * @return string caster_guid : the GUID of the Unit as a decimal string
     */
    FORGE_AUTO_GETTER(Aura, GetCasterGUID)

    /**
     * Returns the level of the [Unit] that casted the [Spell] that caused this [Aura] to be applied.
     *
     * @return uint32 caster_level
     */
    int GetCasterLevel(Forge* F, Aura* aura)
    {
        F->Push(aura->GetCaster()->GetLevel());
        return 1;
    }

    /**
     * Returns the amount of time left until the [Aura] expires.
     *
     * @return int32 duration : amount of time left in milliseconds
     */
    FORGE_AUTO_GETTER(Aura, GetDuration)

    /**
     * Returns the ID of the [Spell] that caused this [Aura] to be applied.
     *
     * @return uint32 aura_id
     */
    FORGE_AUTO_GETTER(Aura, GetId)

    /**
     * Returns the amount of time this [Aura] lasts when applied.
     *
     * To determine how much time has passed since this Aura was applied,
     *   subtract the result of [Aura]:GetDuration from the result of this method.
     *
     * @return int32 max_duration : the maximum duration of the Aura, in milliseconds
     */
    FORGE_AUTO_GETTER(Aura, GetMaxDuration)

    /**
     * Returns the number of times the [Aura] has "stacked".
     *
     * This is the same as the number displayed on the [Aura]'s icon in-game.
     *
     * @return uint32 stack_amount
     */
    FORGE_AUTO_GETTER(Aura, GetStackAmount)

    /**
     * Returns the [Unit] that the [Aura] has been applied to.
     *
     * @return [Unit] owner
     */
    FORGE_AUTO_GETTER(Aura, GetOwner)

    /**
     * Change the amount of time before the [Aura] expires.
     *
     * @param int32 duration : the new duration of the Aura, in milliseconds
     */
    FORGE_AUTO_SETTER(Aura, SetDuration, int32)

    /**
     * Change the maximum amount of time before the [Aura] expires.
     *
     * This does not affect the current duration of the [Aura], but if the [Aura]
     *   is reset to the maximum duration, it will instead change to `duration`.
     *
     * @param int32 duration : the new maximum duration of the Aura, in milliseconds
     */
    FORGE_AUTO_SETTER(Aura, SetMaxDuration, int32)

    /**
     * Change the amount of times the [Aura] has "stacked" on the [Unit].
     *
     * If `amount` is greater than or equal to the current number of stacks,
     *   then the [Aura] has its duration reset to the maximum duration.
     *
     * @param uint8 amount
     */
    FORGE_AUTO_SETTER(Aura, SetStackAmount, uint8)

    /**
     * Remove this [Aura] from the [Unit] it is applied to.
     */
    FORGE_AUTO_VOID_METHOD(Aura, Remove, uint8)

    ForgeRegister<Aura> AuraMethods[] =
    {
        // Getters
        { "GetCaster",      &LuaAura::GetCaster },
        { "GetCasterGUID",  &LuaAura::GetCasterGUID },
        { "GetCasterLevel", &LuaAura::GetCasterLevel },
        { "GetDuration",    &LuaAura::GetDuration },
        { "GetMaxDuration", &LuaAura::GetMaxDuration },
        { "GetAuraId",      &LuaAura::GetId },
        { "GetStackAmount", &LuaAura::GetStackAmount },
        { "GetOwner",       &LuaAura::GetOwner },

        // Setters
        { "SetDuration",    &LuaAura::SetDuration },
        { "SetMaxDuration", &LuaAura::SetMaxDuration },
        { "SetStackAmount", &LuaAura::SetStackAmount },

        // Other
        { "Remove",         &LuaAura::Remove }
    };
};
#endif
