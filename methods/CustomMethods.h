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

// This file is used for custom Lua methods, without needing to edit the existing method header files.
// This can also be used to override default methods without needing to edit existing methods.
// It follows the same structure as any other method header, except you can use RegisterCustomFunction
// to register multiple method tables in a single file.

#include "ForgeTemplate.h"
#include "ForgeIncludes.h"

#ifndef CUSTOMMETHODS_H
#define CUSTOMMETHODS_H

namespace LuaCustom
{
    // Define a custom method that returns the players name
    int CustomPlayerMethod(Forge* F, Player* player)
    {
        F->Push(player->GetName());
        return 1;
    }

    // Create a custom player method registry
    ForgeRegister<Player> CustomPlayerMethods[] =
    {
        // Add the custom player method to the registry
        { "CustomPlayerMethod", &LuaCustom::CustomPlayerMethod },
    };

    inline void RegisterCustomMethods([[maybe_unused]] Forge* F)
    {
        // Append all the custom Player methods to the Player object
        ForgeTemplate<Player>::SetMethods(F, CustomPlayerMethods);
    };
};

#endif
