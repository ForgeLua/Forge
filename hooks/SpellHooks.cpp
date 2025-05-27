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

#include "Hooks.h"
#include "HookHelpers.h"
#include "LuaEngine.h"
#include "BindingMap.h"
#include "ForgeIncludes.h"
#include "ForgeTemplate.h"

using namespace Hooks;

#define START_HOOK(EVENT, SPELL) \
    auto key = EntryKey<SpellEvents>(EVENT, SPELL->m_spellInfo->Id);\
    if (!SpellEventBindings->HasBindingsFor(key))\
        return;

#define START_HOOK_WITH_RETVAL(EVENT, SPELL, RETVAL) \
    auto key = EntryKey<SpellEvents>(EVENT, SPELL->m_spellInfo->Id);\
    if (!SpellEventBindings->HasBindingsFor(key))\
        return RETVAL;

void Forge::OnSpellCast(Spell* pSpell, bool skipCheck)
{
    START_HOOK(SPELL_EVENT_ON_CAST, pSpell);
    HookPush(pSpell);
    HookPush(skipCheck);
    CallAllFunctions(SpellEventBindings, key);
}
