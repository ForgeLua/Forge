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

#define START_HOOK_SERVER(EVENT) \
    auto key = EventKey<ServerEvents>(EVENT);\
    if (!ServerEventBindings->HasBindingsFor(key))\
        return;

#define START_HOOK_PACKET(EVENT, OPCODE) \
    auto key = EntryKey<PacketEvents>(EVENT, OPCODE);\
    if (!PacketEventBindings->HasBindingsFor(key))\
        return;

bool Forge::OnPacketSend(WorldSession* session, const WorldPacket& packet)
{
    bool result = true;
    Player* player = NULL;
    if (session)
        player = session->GetPlayer();
    OnPacketSendAny(player, packet, result);
    OnPacketSendOne(player, packet, result);
    return result;
}
void Forge::OnPacketSendAny(Player* player, const WorldPacket& packet, bool& result)
{
    START_HOOK_SERVER(SERVER_EVENT_ON_PACKET_SEND);
    HookPush(&packet); // pushing pointer to local is fine, a copy of value will be stored, not pointer itself
    HookPush(player);
    int n = SetupStack(ServerEventBindings, key, 2);

    while (n > 0)
    {
        int r = CallOneFunction(n--, 2, 1);

        if (lua_isboolean(L, r + 0) && !lua_toboolean(L, r + 0))
            result = false;

        lua_pop(L, 1);
    }

    CleanUpStack(2);
}

void Forge::OnPacketSendOne(Player* player, const WorldPacket& packet, bool& result)
{
    START_HOOK_PACKET(PACKET_EVENT_ON_PACKET_SEND, packet.GetOpcode());
    HookPush(&packet); // pushing pointer to local is fine, a copy of value will be stored, not pointer itself
    HookPush(player);
    int n = SetupStack(PacketEventBindings, key, 2);

    while (n > 0)
    {
        int r = CallOneFunction(n--, 2, 1);

        if (lua_isboolean(L, r + 0) && !lua_toboolean(L, r + 0))
            result = false;

        lua_pop(L, 1);
    }

    CleanUpStack(2);
}

bool Forge::OnPacketReceive(WorldSession* session, WorldPacket& packet)
{
    bool result = true;
    Player* player = NULL;
    if (session)
        player = session->GetPlayer();
    OnPacketReceiveAny(player, packet, result);
    OnPacketReceiveOne(player, packet, result);
    return result;
}

void Forge::OnPacketReceiveAny(Player* player, WorldPacket& packet, bool& result)
{
    START_HOOK_SERVER(SERVER_EVENT_ON_PACKET_RECEIVE);
    HookPush(&packet); // pushing pointer to local is fine, a copy of value will be stored, not pointer itself
    HookPush(player);
    int n = SetupStack(ServerEventBindings, key, 2);

    while (n > 0)
    {
        int r = CallOneFunction(n--, 2, 2);

        if (lua_isboolean(L, r + 0) && !lua_toboolean(L, r + 0))
            result = false;

        if (lua_isuserdata(L, r + 1))
            if (WorldPacket* data = CHECKOBJ<WorldPacket>(r + 1, false))
            {
#if defined FORGE_TRINITY || defined FORGE_VMANGOS
                packet = std::move(*data);
#else
                packet = *data;
#endif
            }

        lua_pop(L, 2);
    }

    CleanUpStack(2);
}

void Forge::OnPacketReceiveOne(Player* player, WorldPacket& packet, bool& result)
{
    START_HOOK_PACKET(PACKET_EVENT_ON_PACKET_RECEIVE, packet.GetOpcode());
    HookPush(&packet); // pushing pointer to local is fine, a copy of value will be stored, not pointer itself
    HookPush(player);
    int n = SetupStack(PacketEventBindings, key, 2);

    while (n > 0)
    {
        int r = CallOneFunction(n--, 2, 2);

        if (lua_isboolean(L, r + 0) && !lua_toboolean(L, r + 0))
            result = false;

        if (lua_isuserdata(L, r + 1))
            if (WorldPacket* data = CHECKOBJ<WorldPacket>(r + 1, false))
            {
#if defined FORGE_TRINITY || defined FORGE_VMANGOS
                packet = std::move(*data);
#else
                packet = *data;
#endif
            }

        lua_pop(L, 2);
    }

    CleanUpStack(2);
}
