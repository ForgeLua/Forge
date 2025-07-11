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
#include "LuaEngine.h"
#include "BindingMap.h"
#include "ForgeCompat.h"
#include "ForgeConfig.h"
#include "ForgeEventMgr.h"
#include "ForgeIncludes.h"
#include "ForgeLoader.h"
#include "ForgeTemplate.h"
#include "ForgeUtility.h"
#include "ForgeCreatureAI.h"
#include "ForgeInstanceAI.h"

extern "C"
{
// Base lua libraries
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

// Additional lua libraries
};

extern void RegisterMethods(Forge* F);

void Forge::_ReloadForge()
{
    // Remove all timed events
    eventMgr->SetStates(LUAEVENT_STATE_ERASE);

    // Close lua
    CloseLua();

    // Open new lua and libraries
    OpenLua();

    // Run scripts from loaded paths
    RunScripts();

    reload = false;
}

Forge::Forge(Map* map, bool compatMode) :
event_level(0),
push_counter(0),
boundMap(map),
compatibilityMode(compatMode),

L(NULL),
eventMgr(NULL),

ServerEventBindings(NULL),
PlayerEventBindings(NULL),
GuildEventBindings(NULL),
GroupEventBindings(NULL),
VehicleEventBindings(NULL),
BGEventBindings(NULL),

PacketEventBindings(NULL),
CreatureEventBindings(NULL),
CreatureGossipBindings(NULL),
GameObjectEventBindings(NULL),
GameObjectGossipBindings(NULL),
SpellEventBindings(NULL),
ItemEventBindings(NULL),
ItemGossipBindings(NULL),
PlayerGossipBindings(NULL),
MapEventBindings(NULL),
InstanceEventBindings(NULL),

CreatureUniqueBindings(NULL)
{
    OpenLua();
    eventMgr = new EventMgr(this);

    // if the script cache is ready, run scripts, otherwise flag state for reload
    if (sForgeLoader->GetCacheState() == SCRIPT_CACHE_READY)
        RunScripts();
    else
        reload = true;
}

Forge::~Forge()
{
    CloseLua();
    delete eventMgr;
    eventMgr = NULL;
}

void Forge::CloseLua()
{
    OnLuaStateClose();

    DestroyBindStores();

    // Must close lua state after deleting stores and mgr
    if (L)
        lua_close(L);
    L = NULL;

    instanceDataRefs.clear();
    continentDataRefs.clear();
}

static int PrecompiledLoader(lua_State* L)
{
    const char* modname = lua_tostring(L, 1);
    if (modname == NULL)
        return 0;

    const std::vector<LuaScript>& scripts = sForgeLoader->GetLuaScripts();

    auto it = std::find_if(scripts.begin(), scripts.end(), [modname](const LuaScript& script) { return script.filename == modname; });
    if (it == scripts.end()) {
        lua_pushfstring(L, "\n\tno precompiled script '%s' found", modname);
        return 1;
    }
    if (luaL_loadbuffer(L, reinterpret_cast<const char*>(&it->bytecode[0]), it->bytecode.size(), it->filename.c_str()))
    {
        // Stack: modname, errmsg
        return lua_error(L);
    }
    // Stack: modname, filefunction
    lua_pushstring(L, it->filepath.c_str());
    // Stack: modname, filefunction, modpath
    return 2;
}

void Forge::OpenLua()
{
    L = luaL_newstate();

    lua_pushlightuserdata(L, this);
    lua_setfield(L, LUA_REGISTRYINDEX, FORGE_STATE_PTR);

    CreateBindStores();

    // open base lua libraries
    luaL_openlibs(L);

    // Register methods and functions
    RegisterMethods(this);

    // get require paths
    const std::string& requirepath = sForgeLoader->GetRequirePath();
    const std::string& requirecpath = sForgeLoader->GetRequireCPath();

    // Set lua require folder paths (scripts folder structure)
    lua_getglobal(L, "package");
    lua_pushstring(L, requirepath.c_str());
    lua_setfield(L, -2, "path");
    lua_pushstring(L, requirecpath.c_str());
    lua_setfield(L, -2, "cpath");
    // Set package.loaders loader for precompiled scripts
    lua_getfield(L, -1, "loaders");
    if (lua_isnil(L, -1)) {
        // Lua 5.2+ uses searchers instead of loaders
        lua_pop(L, 1);
        lua_getfield(L, -1, "searchers");
    }
    // insert the new loader to the loaders table by shifting other elements down by one
    const int newLoaderIndex = 1;
    for (int i = lua_rawlen(L, -1); i >= newLoaderIndex; --i) {
        lua_rawgeti(L, -1, i);
        lua_rawseti(L, -2, i + 1);
    }
    lua_pushcfunction(L, &PrecompiledLoader);
    lua_rawseti(L, -2, newLoaderIndex);
    lua_pop(L, 2); // pop loaders/searchers table, pop package table
}

void Forge::CreateBindStores()
{
    DestroyBindStores();

    ServerEventBindings      = new BindingMap< EventKey<Hooks::ServerEvents> >(L);
    PlayerEventBindings      = new BindingMap< EventKey<Hooks::PlayerEvents> >(L);
    GuildEventBindings       = new BindingMap< EventKey<Hooks::GuildEvents> >(L);
    GroupEventBindings       = new BindingMap< EventKey<Hooks::GroupEvents> >(L);
    VehicleEventBindings     = new BindingMap< EventKey<Hooks::VehicleEvents> >(L);
    BGEventBindings          = new BindingMap< EventKey<Hooks::BGEvents> >(L);

    PacketEventBindings      = new BindingMap< EntryKey<Hooks::PacketEvents> >(L);
    CreatureEventBindings    = new BindingMap< EntryKey<Hooks::CreatureEvents> >(L);
    CreatureGossipBindings   = new BindingMap< EntryKey<Hooks::GossipEvents> >(L);
    GameObjectEventBindings  = new BindingMap< EntryKey<Hooks::GameObjectEvents> >(L);
    GameObjectGossipBindings = new BindingMap< EntryKey<Hooks::GossipEvents> >(L);
    SpellEventBindings       = new BindingMap< EntryKey<Hooks::SpellEvents> >(L);
    ItemEventBindings        = new BindingMap< EntryKey<Hooks::ItemEvents> >(L);
    ItemGossipBindings       = new BindingMap< EntryKey<Hooks::GossipEvents> >(L);
    PlayerGossipBindings     = new BindingMap< EntryKey<Hooks::GossipEvents> >(L);
    MapEventBindings         = new BindingMap< EntryKey<Hooks::InstanceEvents> >(L);
    InstanceEventBindings    = new BindingMap< EntryKey<Hooks::InstanceEvents> >(L);

    CreatureUniqueBindings   = new BindingMap< UniqueObjectKey<Hooks::CreatureEvents> >(L);
}

void Forge::DestroyBindStores()
{
    delete ServerEventBindings;
    delete PlayerEventBindings;
    delete GuildEventBindings;
    delete GroupEventBindings;
    delete VehicleEventBindings;

    delete PacketEventBindings;
    delete CreatureEventBindings;
    delete CreatureGossipBindings;
    delete GameObjectEventBindings;
    delete GameObjectGossipBindings;
    delete SpellEventBindings;
    delete ItemEventBindings;
    delete ItemGossipBindings;
    delete PlayerGossipBindings;
    delete BGEventBindings;
    delete MapEventBindings;
    delete InstanceEventBindings;

    delete CreatureUniqueBindings;

    ServerEventBindings = NULL;
    PlayerEventBindings = NULL;
    GuildEventBindings = NULL;
    GroupEventBindings = NULL;
    VehicleEventBindings = NULL;

    PacketEventBindings = NULL;
    CreatureEventBindings = NULL;
    CreatureGossipBindings = NULL;
    GameObjectEventBindings = NULL;
    GameObjectGossipBindings = NULL;
    SpellEventBindings = NULL;
    ItemEventBindings = NULL;
    ItemGossipBindings = NULL;
    PlayerGossipBindings = NULL;
    BGEventBindings = NULL;
    MapEventBindings = NULL;
    InstanceEventBindings = NULL;

    CreatureUniqueBindings = NULL;
}

void Forge::RunScripts()
{
    int32 const boundMapId = GetBoundMapId();
    uint32 const boundInstanceId = GetBoundInstanceId();
    FORGE_LOG_DEBUG("[Forge]: Running scripts for state: %i, instance: %u", boundMapId, boundInstanceId);

    uint32 oldMSTime = ForgeUtil::GetCurrTime();
    uint32 count = 0;

    std::unordered_map<std::string, std::string> loaded; // filename, path

    lua_getglobal(L, "require");
    // Stack: require

    const std::vector<LuaScript>& scripts = sForgeLoader->GetLuaScripts();

    for (auto it = scripts.begin(); it != scripts.end(); ++it)
    {
        // Check that no duplicate names exist
        if (loaded.find(it->filename) != loaded.end())
        {
            FORGE_LOG_ERROR("[Forge]: Error loading `%s`. File with same name already loaded from `%s`, rename either file", it->filepath.c_str(), loaded[it->filename].c_str());
            continue;
        }
        loaded[it->filename] = it->filepath;

        // We call require on the filename to load the script
        // A custom loader is used to load the script from the combined_scripts table
        // The loader is set up in Forge::OpenLua
        lua_pushvalue(L, -1); // Stack: require, require
        lua_pushstring(L, it->filename.c_str()); // Stack: require, require, filename
        if (ExecuteCall(1, 0))
        {
            // Successfully called require on the script
            FORGE_LOG_DEBUG("[Forge]: Successfully loaded `%s`", it->filepath.c_str());
            ++count;
            continue;
        }
        // Stack: require
    }
    // Stack: require
    lua_pop(L, 1);
    FORGE_LOG_INFO("[Forge]: Executed %u Lua scripts in %u ms for map: %i, instance: %u", count, ForgeUtil::GetTimeDiff(oldMSTime), boundMapId, boundInstanceId);

    OnLuaStateOpen();
}

void Forge::InvalidateObjects()
{
    ++callstackid;
    ASSERT(callstackid && "Callstackid overflow");
}

void Forge::Report(lua_State* _L)
{
    const char* msg = lua_tostring(_L, -1);
    FORGE_LOG_ERROR("%s", msg);
    lua_pop(_L, 1);
}

// Borrowed from http://stackoverflow.com/questions/12256455/print-stacktrace-from-c-code-with-embedded-lua
int Forge::StackTrace(lua_State* _L)
{
    // Stack: errmsg
    if (!lua_isstring(_L, -1))  /* 'message' not a string? */
        return 1;  /* keep it intact */
    // Stack: errmsg, debug
    lua_getglobal(_L, "debug");
    if (!lua_istable(_L, -1))
    {
        lua_pop(_L, 1);
        return 1;
    }
    // Stack: errmsg, debug, traceback
    lua_getfield(_L, -1, "traceback");
    if (!lua_isfunction(_L, -1))
    {
        lua_pop(_L, 2);
        return 1;
    }
    lua_pushvalue(_L, -3);  /* pass error message */
    lua_pushinteger(_L, 1);  /* skip this function and traceback */
    // Stack: errmsg, debug, traceback, errmsg, 2
    lua_call(_L, 2, 1);  /* call debug.traceback */

    // dirty stack?
    // Stack: errmsg, debug, tracemsg
    return 1;
}

bool Forge::ExecuteCall(int params, int res)
{
    int top = lua_gettop(L);
    int base = top - params;

    // Expected: function, [parameters]
    ASSERT(base > 0);

    // Check function type
    if (!lua_isfunction(L, base))
    {
        FORGE_LOG_ERROR("[Forge]: Cannot execute call: registered value is %s, not a function.", luaL_tolstring(L, base, NULL));
        ASSERT(false); // stack probably corrupt
    }

    bool usetrace = sForgeConfig->GetConfig(CONFIG_FORGE_TRACEBACK);
    if (usetrace)
    {
        lua_pushcfunction(L, &StackTrace);
        // Stack: function, [parameters], traceback
        lua_insert(L, base);
        // Stack: traceback, function, [parameters]
    }

    // Objects are invalidated when event_level hits 0
    ++event_level;
    int result = lua_pcall(L, params, res, usetrace ? base : 0);
    --event_level;

    if (usetrace)
    {
        // Stack: traceback, [results or errmsg]
        lua_remove(L, base);
    }
    // Stack: [results or errmsg]

    // lua_pcall returns 0 on success.
    // On error print the error and push nils for expected amount of returned values
    if (result)
    {
        // Stack: errmsg
        Report(L);

        // Force garbage collect
        lua_gc(L, LUA_GCCOLLECT, 0);

        // Push nils for expected amount of results
        for (int i = 0; i < res; ++i)
            lua_pushnil(L);
        // Stack: [nils]
        return false;
    }

    // Stack: [results]
    return true;
}

void Forge::Push()
{
    lua_pushnil(L);
}
void Forge::Push(const long long l)
{
    // pushing pointer to local is fine, a copy of value will be stored, not pointer itself
    ForgeTemplate<long long>::Push(this, &l);
}
void Forge::Push(const unsigned long long l)
{
    // pushing pointer to local is fine, a copy of value will be stored, not pointer itself
    ForgeTemplate<unsigned long long>::Push(this, &l);
}
void Forge::Push(const long l)
{
    Push(static_cast<long long>(l));
}
void Forge::Push(const unsigned long l)
{
    Push(static_cast<unsigned long long>(l));
}
void Forge::Push(const int i)
{
    lua_pushinteger(L, i);
}
void Forge::Push(const unsigned int u)
{
    lua_pushunsigned(L, u);
}
void Forge::Push(const double d)
{
    lua_pushnumber(L, d);
}
void Forge::Push(const float f)
{
    lua_pushnumber(L, f);
}
void Forge::Push(const bool b)
{
    lua_pushboolean(L, b);
}
void Forge::Push(const std::string& str)
{
    lua_pushstring(L, str.c_str());
}
void Forge::Push(const char* str)
{
    lua_pushstring(L, str);
}
void Forge::Push(Pet const* pet)
{
    Push<Creature>(pet);
}
void Forge::Push(TempSummon const* summon)
{
    Push<Creature>(summon);
}
void Forge::Push(Unit const* unit)
{
    if (!unit)
    {
        Push();
        return;
    }
    switch (unit->GetTypeId())
    {
        case TYPEID_UNIT:
            Push(unit->ToCreature());
            break;
        case TYPEID_PLAYER:
            Push(unit->ToPlayer());
            break;
        default:
            ForgeTemplate<Unit>::Push(this, unit);
    }
}
void Forge::Push(WorldObject const* obj)
{
    if (!obj)
    {
        Push();
        return;
    }
    switch (obj->GetTypeId())
    {
        case TYPEID_UNIT:
            Push(obj->ToCreature());
            break;
        case TYPEID_PLAYER:
            Push(obj->ToPlayer());
            break;
        case TYPEID_GAMEOBJECT:
            Push(obj->ToGameObject());
            break;
        case TYPEID_CORPSE:
            Push(obj->ToCorpse());
            break;
        default:
            ForgeTemplate<WorldObject>::Push(this, obj);
    }
}
void Forge::Push(Object const* obj)
{
    if (!obj)
    {
        Push();
        return;
    }
    switch (obj->GetTypeId())
    {
        case TYPEID_UNIT:
            Push(obj->ToCreature());
            break;
        case TYPEID_PLAYER:
            Push(obj->ToPlayer());
            break;
        case TYPEID_GAMEOBJECT:
            Push(obj->ToGameObject());
            break;
        case TYPEID_CORPSE:
            Push(obj->ToCorpse());
            break;
        default:
            ForgeTemplate<Object>::Push(this, obj);
    }
}
void Forge::Push(ObjectGuid const guid)
{
    // pushing pointer to local is fine, a copy of value will be stored, not pointer itself
    ForgeTemplate<ObjectGuid>::Push(this, &guid);
}

static int CheckIntegerRange(lua_State* luastate, int narg, int min, int max)
{
    double value = luaL_checknumber(luastate, narg);
    char error_buffer[64];

    if (value > max)
    {
        snprintf(error_buffer, 64, "value must be less than or equal to %i", max);
        return luaL_argerror(luastate, narg, error_buffer);
    }

    if (value < min)
    {
        snprintf(error_buffer, 64, "value must be greater than or equal to %i", min);
        return luaL_argerror(luastate, narg, error_buffer);
    }

    return static_cast<int>(value);
}

static unsigned int CheckUnsignedRange(lua_State* luastate, int narg, unsigned int max)
{
    double value = luaL_checknumber(luastate, narg);

    if (value < 0)
        return luaL_argerror(luastate, narg, "value must be greater than or equal to 0");

    if (value > max)
    {
        char error_buffer[64];
        snprintf(error_buffer, 64, "value must be less than or equal to %u", max);
        return luaL_argerror(luastate, narg, error_buffer);
    }

    return static_cast<unsigned int>(value);
}

template<>
bool Forge::CHECKVAL<bool>(int narg)
{
    return lua_toboolean(L, narg) != 0;
}

template<>
float Forge::CHECKVAL<float>(int narg)
{
    return static_cast<float>(luaL_checknumber(L, narg));
}

template<>
double Forge::CHECKVAL<double>(int narg)
{
    return luaL_checknumber(L, narg);
}

template<>
signed char Forge::CHECKVAL<signed char>(int narg)
{
    return CheckIntegerRange(L, narg, SCHAR_MIN, SCHAR_MAX);
}

template<>
unsigned char Forge::CHECKVAL<unsigned char>(int narg)
{
    return CheckUnsignedRange(L, narg, UCHAR_MAX);
}

template<>
short Forge::CHECKVAL<short>(int narg)
{
    return CheckIntegerRange(L, narg, SHRT_MIN, SHRT_MAX);
}

template<>
unsigned short Forge::CHECKVAL<unsigned short>(int narg)
{
    return CheckUnsignedRange(L, narg, USHRT_MAX);
}

template<>
int Forge::CHECKVAL<int>(int narg)
{
    return CheckIntegerRange(L, narg, INT_MIN, INT_MAX);
}

template<>
unsigned int Forge::CHECKVAL<unsigned int>(int narg)
{
    return CheckUnsignedRange(L, narg, UINT_MAX);
}

template<>
const char* Forge::CHECKVAL<const char*>(int narg)
{
    return luaL_checkstring(L, narg);
}

template<>
std::string Forge::CHECKVAL<std::string>(int narg)
{
    return luaL_checkstring(L, narg);
}

template<>
long long Forge::CHECKVAL<long long>(int narg)
{
    if (lua_isnumber(L, narg))
        return static_cast<long long>(CHECKVAL<double>(narg));
    return *(Forge::CHECKOBJ<long long>(narg, true));
}

template<>
unsigned long long Forge::CHECKVAL<unsigned long long>(int narg)
{
    if (lua_isnumber(L, narg))
        return static_cast<unsigned long long>(CHECKVAL<uint32>(narg));
    return *(Forge::CHECKOBJ<unsigned long long>(narg, true));
}

template<>
long Forge::CHECKVAL<long>(int narg)
{
    return static_cast<long>(CHECKVAL<long long>(narg));
}

template<>
unsigned long Forge::CHECKVAL<unsigned long>(int narg)
{
    return static_cast<unsigned long>(CHECKVAL<unsigned long long>(narg));
}

template<>
ObjectGuid Forge::CHECKVAL<ObjectGuid>(int narg)
{
    ObjectGuid* guid = CHECKOBJ<ObjectGuid>(narg, true);
    return guid ? *guid : ObjectGuid();
}

template<>
AuraRemoveMode Forge::CHECKVAL<AuraRemoveMode>(int index) {
    uint8 value = static_cast<uint8>(CHECKVAL<uint8>(index));
    return static_cast<AuraRemoveMode>(value);
}

template<> Object* Forge::CHECKOBJ<Object>(int narg, bool error)
{
    Object* obj = CHECKOBJ<WorldObject>(narg, false);
    if (!obj)
        obj = CHECKOBJ<Item>(narg, false);
    if (!obj)
        obj = ForgeTemplate<Object>::Check(this, narg, error);
    return obj;
}
template<> WorldObject* Forge::CHECKOBJ<WorldObject>(int narg, bool error)
{
    WorldObject* obj = CHECKOBJ<Unit>(narg, false);
    if (!obj)
        obj = CHECKOBJ<GameObject>(narg, false);
    if (!obj)
        obj = CHECKOBJ<Corpse>(narg, false);
    if (!obj)
        obj = ForgeTemplate<WorldObject>::Check(this, narg, error);
    return obj;
}
template<> Unit* Forge::CHECKOBJ<Unit>(int narg, bool error)
{
    Unit* obj = CHECKOBJ<Player>(narg, false);
    if (!obj)
        obj = CHECKOBJ<Creature>(narg, false);
    if (!obj)
        obj = ForgeTemplate<Unit>::Check(this, narg, error);
    return obj;
}

template<> ForgeObject* Forge::CHECKOBJ<ForgeObject>(int narg, bool error)
{
    return CHECKTYPE(narg, NULL, error);
}

ForgeObject* Forge::CHECKTYPE(int narg, const char* tname, bool error)
{
    if (lua_islightuserdata(L, narg))
    {
        if (error)
            luaL_argerror(L, narg, "bad argument : userdata expected, got lightuserdata");
        return NULL;
    }

    ForgeObject* forgeObject = static_cast<ForgeObject*>(lua_touserdata(L, narg));

    if (!forgeObject || (tname && forgeObject->GetTypeName() != tname))
    {
        if (error)
        {
            char buff[256];
            snprintf(buff, 256, "bad argument : %s expected, got %s", tname ? tname : "ForgeObject", forgeObject ? forgeObject->GetTypeName() : luaL_typename(L, narg));
            luaL_argerror(L, narg, buff);
        }
        return NULL;
    }
    return forgeObject;
}

template<typename K>
static int cancelBinding(lua_State* L)
{
    Forge* F = Forge::GetForge(L);

    uint64 bindingID = F->CHECKVAL<uint64>(lua_upvalueindex(1));

    BindingMap<K>* bindings = (BindingMap<K>*)lua_touserdata(L, lua_upvalueindex(2));
    ASSERT(bindings != NULL);

    bindings->Remove(bindingID);

    return 0;
}

template<typename K>
static void createCancelCallback(Forge* f, uint64 bindingID, BindingMap<K>* bindings)
{
    f->Push(bindingID);
    lua_pushlightuserdata(f->L, bindings);
    // Stack: bindingID, bindings

    lua_pushcclosure(f->L, &cancelBinding<K>, 2);
    // Stack: cancel_callback
}

// Saves the function reference ID given to the register type's store for given entry under the given event
int Forge::Register(uint8 regtype, uint32 entry, ObjectGuid guid, uint32 instanceId, uint32 event_id, int functionRef, uint32 shots)
{
    uint64 bindingID;

    switch (regtype)
    {
        case Hooks::REGTYPE_SERVER:
            if (event_id < Hooks::SERVER_EVENT_COUNT)
            {
                auto key = EventKey<Hooks::ServerEvents>((Hooks::ServerEvents)event_id);
                bindingID = ServerEventBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, ServerEventBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_PLAYER:
            if (event_id < Hooks::PLAYER_EVENT_COUNT)
            {
                auto key = EventKey<Hooks::PlayerEvents>((Hooks::PlayerEvents)event_id);
                bindingID = PlayerEventBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, PlayerEventBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_GUILD:
            if (event_id < Hooks::GUILD_EVENT_COUNT)
            {
                auto key = EventKey<Hooks::GuildEvents>((Hooks::GuildEvents)event_id);
                bindingID = GuildEventBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, GuildEventBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_GROUP:
            if (event_id < Hooks::GROUP_EVENT_COUNT)
            {
                auto key = EventKey<Hooks::GroupEvents>((Hooks::GroupEvents)event_id);
                bindingID = GroupEventBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, GroupEventBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_VEHICLE:
            if (event_id < Hooks::VEHICLE_EVENT_COUNT)
            {
                auto key = EventKey<Hooks::VehicleEvents>((Hooks::VehicleEvents)event_id);
                bindingID = VehicleEventBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, VehicleEventBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_BG:
            if (event_id < Hooks::BG_EVENT_COUNT)
            {
                auto key = EventKey<Hooks::BGEvents>((Hooks::BGEvents)event_id);
                bindingID = BGEventBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, BGEventBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_PACKET:
            if (event_id < Hooks::PACKET_EVENT_COUNT)
            {
                if (entry >= NUM_MSG_TYPES)
                {
                    luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                    luaL_error(L, "Couldn't find a creature with (ID: %d)!", entry);
                    return 0; // Stack: (empty)
                }

                auto key = EntryKey<Hooks::PacketEvents>((Hooks::PacketEvents)event_id, entry);
                bindingID = PacketEventBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, PacketEventBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_CREATURE:
            if (event_id < Hooks::CREATURE_EVENT_COUNT)
            {
                if (entry != 0)
                {
                    if (!eObjectMgr->GetCreatureTemplate(entry))
                    {
                        luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                        luaL_error(L, "Couldn't find a creature with (ID: %d)!", entry);
                        return 0; // Stack: (empty)
                    }

                    auto key = EntryKey<Hooks::CreatureEvents>((Hooks::CreatureEvents)event_id, entry);
                    bindingID = CreatureEventBindings->Insert(key, functionRef, shots);
                    createCancelCallback(this, bindingID, CreatureEventBindings);
                }
                else
                {
                    if (guid.IsEmpty())
                    {
                        luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                        luaL_error(L, "guid was 0!");
                        return 0; // Stack: (empty)
                    }

                    auto key = UniqueObjectKey<Hooks::CreatureEvents>((Hooks::CreatureEvents)event_id, guid, instanceId);
                    bindingID = CreatureUniqueBindings->Insert(key, functionRef, shots);
                    createCancelCallback(this, bindingID, CreatureUniqueBindings);
                }
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_CREATURE_GOSSIP:
            if (event_id < Hooks::GOSSIP_EVENT_COUNT)
            {
                if (!eObjectMgr->GetCreatureTemplate(entry))
                {
                    luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                    luaL_error(L, "Couldn't find a creature with (ID: %d)!", entry);
                    return 0; // Stack: (empty)
                }

                auto key = EntryKey<Hooks::GossipEvents>((Hooks::GossipEvents)event_id, entry);
                bindingID = CreatureGossipBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, CreatureGossipBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_GAMEOBJECT:
            if (event_id < Hooks::GAMEOBJECT_EVENT_COUNT)
            {
                if (!eObjectMgr->GetGameObjectTemplate(entry))
                {
                    luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                    luaL_error(L, "Couldn't find a gameobject with (ID: %d)!", entry);
                    return 0; // Stack: (empty)
                }

                auto key = EntryKey<Hooks::GameObjectEvents>((Hooks::GameObjectEvents)event_id, entry);
                bindingID = GameObjectEventBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, GameObjectEventBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_GAMEOBJECT_GOSSIP:
            if (event_id < Hooks::GOSSIP_EVENT_COUNT)
            {
                if (!eObjectMgr->GetGameObjectTemplate(entry))
                {
                    luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                    luaL_error(L, "Couldn't find a gameobject with (ID: %d)!", entry);
                    return 0; // Stack: (empty)
                }

                auto key = EntryKey<Hooks::GossipEvents>((Hooks::GossipEvents)event_id, entry);
                bindingID = GameObjectGossipBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, GameObjectGossipBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_SPELL:
            if (event_id < Hooks::SPELL_EVENT_COUNT)
            {
                auto key = EntryKey<Hooks::SpellEvents>((Hooks::SpellEvents)event_id, entry);
                bindingID = SpellEventBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, SpellEventBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_ITEM:
            if (event_id < Hooks::ITEM_EVENT_COUNT)
            {
                if (!eObjectMgr->GetItemTemplate(entry))
                {
                    luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                    luaL_error(L, "Couldn't find a item with (ID: %d)!", entry);
                    return 0; // Stack: (empty)
                }

                auto key = EntryKey<Hooks::ItemEvents>((Hooks::ItemEvents)event_id, entry);
                bindingID = ItemEventBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, ItemEventBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_ITEM_GOSSIP:
            if (event_id < Hooks::GOSSIP_EVENT_COUNT)
            {
                if (!eObjectMgr->GetItemTemplate(entry))
                {
                    luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
                    luaL_error(L, "Couldn't find a item with (ID: %d)!", entry);
                    return 0; // Stack: (empty)
                }

                auto key = EntryKey<Hooks::GossipEvents>((Hooks::GossipEvents)event_id, entry);
                bindingID = ItemGossipBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, ItemGossipBindings);
                return 1; // Stack: callback
            }
            break;

        case Hooks::REGTYPE_PLAYER_GOSSIP:
            if (event_id < Hooks::GOSSIP_EVENT_COUNT)
            {
                auto key = EntryKey<Hooks::GossipEvents>((Hooks::GossipEvents)event_id, entry);
                bindingID = PlayerGossipBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, PlayerGossipBindings);
                return 1; // Stack: callback
            }
            break;
        case Hooks::REGTYPE_MAP:
            if (event_id < Hooks::INSTANCE_EVENT_COUNT)
            {
                auto key = EntryKey<Hooks::InstanceEvents>((Hooks::InstanceEvents)event_id, entry);
                bindingID = MapEventBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, MapEventBindings);
                return 1; // Stack: callback
            }
            break;
        case Hooks::REGTYPE_INSTANCE:
            if (event_id < Hooks::INSTANCE_EVENT_COUNT)
            {
                auto key = EntryKey<Hooks::InstanceEvents>((Hooks::InstanceEvents)event_id, entry);
                bindingID = InstanceEventBindings->Insert(key, functionRef, shots);
                createCancelCallback(this, bindingID, InstanceEventBindings);
                return 1; // Stack: callback
            }
            break;
    }
    luaL_unref(L, LUA_REGISTRYINDEX, functionRef);
    std::ostringstream oss;
    oss << "regtype " << static_cast<uint32>(regtype) << ", event " << event_id << ", entry " << entry << ", guid " <<
        guid.ToHexString()
        << ", instance " << instanceId;
    luaL_error(L, "Unknown event type (%s)", oss.str().c_str());
    return 0;
}

void Forge::UpdateForge(uint32 diff)
{
    if (reload && sForgeLoader->GetCacheState() == SCRIPT_CACHE_READY)
        if(!GetQueryProcessor().HasPendingCallbacks())
            _ReloadForge();

    eventMgr->globalProcessor->Update(diff);
    GetQueryProcessor().ProcessReadyCallbacks();
}

/*
 * Cleans up the stack, effectively undoing all Push calls and the Setup call.
 */
void Forge::CleanUpStack(int number_of_arguments)
{
    // Stack: event_id, [arguments]

    lua_pop(L, number_of_arguments + 1); // Add 1 because the caller doesn't know about `event_id`.
    // Stack: (empty)

    if (event_level == 0)
        InvalidateObjects();
}

/*
 * Call a single event handler that was put on the stack with `Setup` and removes it from the stack.
 *
 * The caller is responsible for keeping track of how many times this should be called.
 */
int Forge::CallOneFunction(int number_of_functions, int number_of_arguments, int number_of_results)
{
    ++number_of_arguments; // Caller doesn't know about `event_id`.
    ASSERT(number_of_functions > 0 && number_of_arguments > 0 && number_of_results >= 0);
    // Stack: event_id, [arguments], [functions]

    int functions_top        = lua_gettop(L);
    int first_function_index = functions_top - number_of_functions + 1;
    int arguments_top        = first_function_index - 1;
    int first_argument_index = arguments_top - number_of_arguments + 1;

    // Copy the arguments from the bottom of the stack to the top.
    for (int argument_index = first_argument_index; argument_index <= arguments_top; ++argument_index)
    {
        lua_pushvalue(L, argument_index);
    }
    // Stack: event_id, [arguments], [functions], event_id, [arguments]

    ExecuteCall(number_of_arguments, number_of_results);
    --functions_top;
    // Stack: event_id, [arguments], [functions - 1], [results]

    return functions_top + 1; // Return the location of the first result (if any exist).
}

CreatureAI* Forge::GetAI(Creature* creature)
{
    for (int i = 1; i < Hooks::CREATURE_EVENT_COUNT; ++i)
    {
        Hooks::CreatureEvents event_id = (Hooks::CreatureEvents)i;

        auto entryKey = EntryKey<Hooks::CreatureEvents>(event_id, creature->GetEntry());
        auto uniqueKey = UniqueObjectKey<Hooks::CreatureEvents>(event_id, creature->GET_GUID(), creature->GetInstanceId());

        if (CreatureEventBindings->HasBindingsFor(entryKey) ||
            CreatureUniqueBindings->HasBindingsFor(uniqueKey))
            return new ForgeCreatureAI(creature);
    }

    return NULL;
}

InstanceData* Forge::GetInstanceData(Map* map)
{
    for (int i = 1; i < Hooks::INSTANCE_EVENT_COUNT; ++i)
    {
        Hooks::InstanceEvents event_id = (Hooks::InstanceEvents)i;

        auto key = EntryKey<Hooks::InstanceEvents>(event_id, map->GetId());

        if (MapEventBindings->HasBindingsFor(key) ||
            InstanceEventBindings->HasBindingsFor(key))
            return new ForgeInstanceAI(map);
    }

    return NULL;
}

bool Forge::HasInstanceData(Map const* map)
{
    if (!map->Instanceable())
        return continentDataRefs.find(map->GetId()) != continentDataRefs.end();
    else
        return instanceDataRefs.find(map->GetInstanceId()) != instanceDataRefs.end();
}

void Forge::CreateInstanceData(Map const* map)
{
    ASSERT(lua_istable(L, -1));
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);

    if (!map->Instanceable())
    {
        uint32 mapId = map->GetId();

        // If there's another table that was already stored for the map, unref it.
        auto mapRef = continentDataRefs.find(mapId);
        if (mapRef != continentDataRefs.end())
        {
            luaL_unref(L, LUA_REGISTRYINDEX, mapRef->second);
        }

        continentDataRefs[mapId] = ref;
    }
    else
    {
        uint32 instanceId = map->GetInstanceId();

        // If there's another table that was already stored for the instance, unref it.
        auto instRef = instanceDataRefs.find(instanceId);
        if (instRef != instanceDataRefs.end())
        {
            luaL_unref(L, LUA_REGISTRYINDEX, instRef->second);
        }

        instanceDataRefs[instanceId] = ref;
    }
}

/*
 * Unrefs the instanceId related events and data
 * Does all required actions for when an instance is freed.
 */
void Forge::FreeInstanceId(uint32 instanceId)
{
    for (int i = 1; i < Hooks::INSTANCE_EVENT_COUNT; ++i)
    {
        auto key = EntryKey<Hooks::InstanceEvents>((Hooks::InstanceEvents)i, instanceId);

        if (MapEventBindings->HasBindingsFor(key))
            MapEventBindings->Clear(key);

        if (InstanceEventBindings->HasBindingsFor(key))
            InstanceEventBindings->Clear(key);

        if (instanceDataRefs.find(instanceId) != instanceDataRefs.end())
        {
            luaL_unref(L, LUA_REGISTRYINDEX, instanceDataRefs[instanceId]);
            instanceDataRefs.erase(instanceId);
        }
    }
}

void Forge::PushInstanceData(ForgeInstanceAI* ai, bool incrementCounter)
{
    // Check if the instance data is missing (i.e. someone reloaded Forge).
    if (!HasInstanceData(ai->instance))
        ai->Reload();

    // Get the instance data table from the registry.
    if (!ai->instance->Instanceable())
        lua_rawgeti(L, LUA_REGISTRYINDEX, continentDataRefs[ai->instance->GetId()]);
    else
        lua_rawgeti(L, LUA_REGISTRYINDEX, instanceDataRefs[ai->instance->GetInstanceId()]);

    ASSERT(lua_istable(L, -1));

    if (incrementCounter)
        ++push_counter;
}
