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

#ifndef _LUA_ENGINE_H
#define _LUA_ENGINE_H

#include "Common.h"
#include "ForgeUtility.h"
#include "Hooks.h"

#include "DBCEnums.h"
#include "Group.h"
#include "Item.h"
#include "Map.h"
#include "SharedDefines.h"
#include "Weather.h"
#include "World.h"

#include <mutex>
#include <memory>

extern "C"
{
#include "lua.h"
};

class AuctionHouseObject;
class Channel;
class Corpse;
class Creature;
class CreatureAI;
class ForgeInstanceAI;
class GameObject;
class Group;
class Guild;
class Item;
class Pet;
class Player;
class Quest;
class Spell;
class SpellCastTargets;
class Unit;
class Weather;
class WorldPacket;
struct AreaTriggerEntry;
struct AuctionEntry;

class Battleground;
class GameObjectAI;
class InstanceScript;
class TempSummon;
class Vehicle;
struct ItemTemplate;
typedef Battleground BattleGround;
typedef BattlegroundTypeId BattleGroundTypeId;
typedef InstanceScript InstanceData;

struct lua_State;
class EventMgr;
class ForgeObject;
template<typename T> class ForgeTemplate;

template<typename K> class BindingMap;
template<typename T> struct EventKey;
template<typename T> struct EntryKey;
template<typename T> struct UniqueObjectKey;

struct LuaScript
{
    std::string fileext;
    std::string filename;
    std::string filepath;
    std::string modulepath;
    BytecodeBuffer bytecode;
    int32 mapId;
};

enum MethodRegisterState
{
    METHOD_REG_NONE = 0,
    METHOD_REG_MAP,
    METHOD_REG_WORLD,
    METHOD_REG_ALL
};

#define FORGE_STATE_PTR "Forge State Ptr"

#define FORGE_GAME_API TC_GAME_API

class FORGE_GAME_API Forge
{
public:
    typedef std::list<LuaScript> ScriptList;
    typedef std::recursive_mutex LockType;

    void ReloadForge() { reload = true; }
    bool ExecuteCall(int params, int res);

private:

    // Indicates that the lua state should be reloaded
    bool reload = false;

    // A counter for lua event stacks that occur (see event_level).
    // This is used to determine whether an object belongs to the current call stack or not.
    // 0 is reserved for always belonging to the call stack
    // 1 is reserved for a non valid callstackid
    uint64 callstackid = 2;
    // A counter for the amount of nested events. When the event_level
    // reaches 0 we are about to return back to C++. At this point the
    // objects used during the event stack are invalidated.
    uint32 event_level;
    // When a hook pushes arguments to be passed to event handlers,
    //  this is used to keep track of how many arguments were pushed.
    uint8 push_counter;

    Map* const boundMap;

    // Whether or not Forge is in compatibility mode. Used in some method wrappers.
    bool compatibilityMode;

    // Map from instance ID -> Lua table ref
    std::unordered_map<uint32, int> instanceDataRefs;
    // Map from map ID -> Lua table ref
    std::unordered_map<uint32, int> continentDataRefs;

    void OpenLua();
    void CloseLua();
    void DestroyBindStores();
    void CreateBindStores();
    void InvalidateObjects();

    // Use ReloadForge() to make forge reload
    // This is called on world update to reload forge
    void _ReloadForge();

    // Some helpers for hooks to call event handlers.
    // The bodies of the templates are in HookHelpers.h, so if you want to use them you need to #include "HookHelpers.h".
    template<typename K1, typename K2> int SetupStack(BindingMap<K1>* bindings1, BindingMap<K2>* bindings2, const K1& key1, const K2& key2, int number_of_arguments);
                                       int CallOneFunction(int number_of_functions, int number_of_arguments, int number_of_results);
                                       void CleanUpStack(int number_of_arguments);
    template<typename T>               void ReplaceArgument(T value, uint8 index);
    template<typename K1, typename K2> void CallAllFunctions(BindingMap<K1>* bindings1, BindingMap<K2>* bindings2, const K1& key1, const K2& key2);
    template<typename K1, typename K2> bool CallAllFunctionsBool(BindingMap<K1>* bindings1, BindingMap<K2>* bindings2, const K1& key1, const K2& key2, bool default_value = false);

    // Same as above but for only one binding instead of two.
    // `key` is passed twice because there's no NULL for references, but it's not actually used if `bindings2` is NULL.
    template<typename K> int SetupStack(BindingMap<K>* bindings, const K& key, int number_of_arguments)
    {
        return SetupStack<K, K>(bindings, NULL, key, key, number_of_arguments);
    }
    template<typename K> void CallAllFunctions(BindingMap<K>* bindings, const K& key)
    {
        CallAllFunctions<K, K>(bindings, NULL, key, key);
    }
    template<typename K> bool CallAllFunctionsBool(BindingMap<K>* bindings, const K& key, bool default_value = false)
    {
        return CallAllFunctionsBool<K, K>(bindings, NULL, key, key, default_value);
    }

    // Non-static pushes, to be used in hooks.
    // They up the pushed value counter for hook helper functions.
    void HookPush()                                 { Push(); ++push_counter; }
    void HookPush(const long long value)            { Push(value); ++push_counter; }
    void HookPush(const unsigned long long value)   { Push(value); ++push_counter; }
    void HookPush(const long value)                 { Push(value); ++push_counter; }
    void HookPush(const unsigned long value)        { Push(value); ++push_counter; }
    void HookPush(const int value)                  { Push(value); ++push_counter; }
    void HookPush(const unsigned int value)         { Push(value); ++push_counter; }
    void HookPush(const bool value)                 { Push(value); ++push_counter; }
    void HookPush(const float value)                { Push(value); ++push_counter; }
    void HookPush(const double value)               { Push(value); ++push_counter; }
    void HookPush(const std::string& value)         { Push(value); ++push_counter; }
    void HookPush(const char* value)                { Push(value); ++push_counter; }
    void HookPush(ObjectGuid const value)           { Push(value); ++push_counter; }
    template<typename T>
    void HookPush(T const* ptr)                     { Push(ptr); ++push_counter; }

public:

    lua_State* L;
    EventMgr* eventMgr;

    QueryCallbackProcessor queryProcessor;
    QueryCallbackProcessor& GetQueryProcessor() { return queryProcessor; }

    BindingMap< EventKey<Hooks::ServerEvents> >*     ServerEventBindings;
    BindingMap< EventKey<Hooks::PlayerEvents> >*     PlayerEventBindings;
    BindingMap< EventKey<Hooks::GuildEvents> >*      GuildEventBindings;
    BindingMap< EventKey<Hooks::GroupEvents> >*      GroupEventBindings;
    BindingMap< EventKey<Hooks::VehicleEvents> >*    VehicleEventBindings;
    BindingMap< EventKey<Hooks::BGEvents> >*         BGEventBindings;

    BindingMap< EntryKey<Hooks::PacketEvents> >*     PacketEventBindings;
    BindingMap< EntryKey<Hooks::CreatureEvents> >*   CreatureEventBindings;
    BindingMap< EntryKey<Hooks::GossipEvents> >*     CreatureGossipBindings;
    BindingMap< EntryKey<Hooks::GameObjectEvents> >* GameObjectEventBindings;
    BindingMap< EntryKey<Hooks::GossipEvents> >*     GameObjectGossipBindings;
    BindingMap< EntryKey<Hooks::SpellEvents> >*      SpellEventBindings;
    BindingMap< EntryKey<Hooks::ItemEvents> >*       ItemEventBindings;
    BindingMap< EntryKey<Hooks::GossipEvents> >*     ItemGossipBindings;
    BindingMap< EntryKey<Hooks::GossipEvents> >*     PlayerGossipBindings;
    BindingMap< EntryKey<Hooks::InstanceEvents> >*   MapEventBindings;
    BindingMap< EntryKey<Hooks::InstanceEvents> >*   InstanceEventBindings;

    BindingMap< UniqueObjectKey<Hooks::CreatureEvents> >* CreatureUniqueBindings;

    static int StackTrace(lua_State* _L);
    static void Report(lua_State* _L);

    // Never returns nullptr
    static Forge* GetForge(lua_State* L)
    {
        lua_pushstring(L, FORGE_STATE_PTR);
        lua_rawget(L, LUA_REGISTRYINDEX);
        ASSERT(lua_islightuserdata(L, -1));
        Forge* F = static_cast<Forge*>(lua_touserdata(L, -1));
        lua_pop(L, 1);
        ASSERT(F);
        return F;
    }

    // can be used by anything, including methods.
    void Push(); // nil
    void Push(const long long);
    void Push(const unsigned long long);
    void Push(const long);
    void Push(const unsigned long);
    void Push(const int);
    void Push(const unsigned int);
    void Push(const bool);
    void Push(const float);
    void Push(const double);
    void Push(const std::string&);
    void Push(const char*);
    void Push(Object const* obj);
    void Push(WorldObject const* obj);
    void Push(Unit const* unit);
    void Push(Pet const* pet);
    void Push(TempSummon const* summon);
    void Push(ObjectGuid const guid);
    template<typename T>
    void Push(T const* ptr)
    {
        ForgeTemplate<T>::Push(this, ptr);
    }

    /*
     * Returns `true` if Forge has instance data for `map`.
     */
    bool HasInstanceData(Map const* map);

    /*
     * Use the top element of the stack as the instance data table for `map`,
     *   then pops it off the stack.
     */
    void CreateInstanceData(Map const* map);

    /*
     * Retrieve the instance data for the `Map` scripted by `ai` and push it
     *   onto the stack.
     *
     * An `ForgeInstanceAI` is needed because the instance data might
     *   not exist (i.e. Forge has been reloaded).
     *
     * In that case, the AI is "reloaded" (new instance data table is created
     *   and loaded with the last known save state, and `Load`/`Initialize`
     *   hooks are called).
     */
    void PushInstanceData(ForgeInstanceAI* ai, bool incrementCounter = true);

    void RunScripts();
    bool HasLuaState() const { return L != NULL; }
    uint64 GetCallstackId() const { return callstackid; }
    int Register(uint8 reg, uint32 entry, ObjectGuid guid, uint32 instanceId, uint32 event_id, int functionRef, uint32 shots);
    void UpdateForge(uint32 diff);

    // Checks
    template<typename T> T CHECKVAL(int narg);
    template<typename T> T CHECKVAL(int narg, T def)
    {
        return lua_isnoneornil(L, narg) ? def : CHECKVAL<T>(narg);
    }
    template<typename T> T* CHECKOBJ(int narg, bool error = true)
    {
        return ForgeTemplate<T>::Check(this, narg, error);
    }
    ForgeObject* CHECKTYPE(int narg, const char* tname, bool error = true);

    CreatureAI* GetAI(Creature* creature);
    InstanceData* GetInstanceData(Map* map);
    void FreeInstanceId(uint32 instanceId);

    Map* GetBoundMap() const { return boundMap; }

    int32 GetBoundMapId() const
    {
        if(const Map * map = GetBoundMap())
            return map->GetId();

        return -1;
    }

    uint32 GetBoundInstanceId() const
    {
        if(const Map * map = GetBoundMap())
            return map->GetInstanceId();

        return 0;
    }

    bool GetCompatibilityMode() const { return compatibilityMode; }

    Forge(Map * map, bool compatMode = false);
    ~Forge();

    // Prevent copy
    Forge(Forge const&) = delete;
    Forge& operator=(const Forge&) = delete;

    /* Custom */
    void OnTimedEvent(int funcRef, uint32 delay, uint32 calls, WorldObject* obj);
    bool OnCommand(Player* player, const char* text);
    void OnWorldUpdate(uint32 diff);
    void OnLootItem(Player* pPlayer, Item* pItem, uint32 count, ObjectGuid guid);
    void OnLootMoney(Player* pPlayer, uint32 amount);
    void OnFirstLogin(Player* pPlayer);
    void OnEquip(Player* pPlayer, Item* pItem, uint8 bag, uint8 slot);
    void OnRepop(Player* pPlayer);
    void OnResurrect(Player* pPlayer);
    void OnQuestAbandon(Player* pPlayer, uint32 questId);
    void OnQuestStatusChanged(Player* pPlayer, uint32 questId, uint8 status);
    void OnLearnTalents(Player* pPlayer, uint32 talentId, uint32 talentRank, uint32 spellid);
    void OnSkillChange(Player* pPlayer, uint32 skillId, uint32 skillValue);
    void OnLearnSpell(Player* pPlayer, uint32 spellid);
    InventoryResult OnCanUseItem(const Player* pPlayer, uint32 itemEntry);
    void OnLuaStateClose();
    void OnLuaStateOpen();
    bool OnAddonMessage(Player* sender, uint32 type, std::string& msg, Player* receiver, Guild* guild, Group* group, Channel* channel);
    bool OnTradeInit(Player* trader, Player* tradee);
    bool OnTradeAccept(Player* trader, Player* tradee);
    bool OnSendMail(Player* sender, ObjectGuid recipientGuid);
    void OnDiscoverArea(Player* player, uint32 area);
    void OnPlayerItemMove(Player* pPlayer, uint8 sourceBag, uint8 sourceSlot, uint8 destBag, uint8 destSlot, Item* itemSource, Item* itemDest);
    void OnPlayerUnEquipItem(Player* pPlayer, Item* pItem, uint8 slot);
    void OnPlayerItemBuy(Player* pPlayer, Item* pItem, Creature* vendor, ItemTemplate const* item_template, uint32 count);

    /* Item */
    void OnDummyEffect(WorldObject* pCaster, uint32 spellId, SpellEffIndex effIndex, Item* pTarget);
    bool OnQuestAccept(Player* pPlayer, Item* pItem, Quest const* pQuest);
    bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& targets);
    bool OnItemUse(Player* pPlayer, Item* pItem, SpellCastTargets const& targets);
    bool OnItemGossip(Player* pPlayer, Item* pItem, SpellCastTargets const& targets);
    bool OnExpire(Player* pPlayer, ItemTemplate const* pProto);
    bool OnRemove(Player* pPlayer, Item* item);
    void OnAdd(Player* pPlayer, Item* item);
    void HandleGossipSelectOption(Player* pPlayer, Item* item, uint32 sender, uint32 action, const std::string& code);
    void OnItemEquip(Player* pPlayer, Item* pItem, uint8 slot);
    void OnItemUnEquip(Player* pPlayer, Item* pItem, uint8 slot);

    /* Creature */
    void OnDummyEffect(WorldObject* pCaster, uint32 spellId, SpellEffIndex effIndex, Creature* pTarget);
    bool OnGossipHello(Player* pPlayer, Creature* pCreature);
    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action);
    bool OnGossipSelectCode(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action, const char* code);
    bool OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const* pQuest);
    bool OnQuestReward(Player* pPlayer, Creature* pCreature, Quest const* pQuest, uint32 opt);
    void GetDialogStatus(const Player* pPlayer, const Creature* pCreature);

    bool OnSummoned(Creature* creature, Unit* summoner);
    bool UpdateAI(Creature* me, const uint32 diff);
    bool EnterCombat(Creature* me, Unit* target);
    bool DamageTaken(Creature* me, Unit* attacker, uint32& damage);
    bool JustDied(Creature* me, Unit* killer);
    bool KilledUnit(Creature* me, Unit* victim);
    bool JustSummoned(Creature* me, Creature* summon);
    bool SummonedCreatureDespawn(Creature* me, Creature* summon);
    bool MovementInform(Creature* me, uint32 type, uint32 id);
    bool AttackStart(Creature* me, Unit* target);
    bool EnterEvadeMode(Creature* me);
    bool JustRespawned(Creature* me);
    bool JustReachedHome(Creature* me);
    bool ReceiveEmote(Creature* me, Player* player, uint32 emoteId);
    bool CorpseRemoved(Creature* me, uint32& respawnDelay);
    bool MoveInLineOfSight(Creature* me, Unit* who);
    bool SpellHit(Creature* me, WorldObject* caster, SpellInfo const* spell);
    bool SpellHitTarget(Creature* me, WorldObject* target, SpellInfo const* spell);
    bool SummonedCreatureDies(Creature* me, Creature* summon, Unit* killer);
    bool OwnerAttackedBy(Creature* me, Unit* attacker);
    bool OwnerAttacked(Creature* me, Unit* target);
    void On_Reset(Creature* me);

    /* GameObject */
    void OnDummyEffect(WorldObject* pCaster, uint32 spellId, SpellEffIndex effIndex, GameObject* pTarget);
    bool OnGameObjectUse(Player* pPlayer, GameObject* pGameObject);
    bool OnGossipHello(Player* pPlayer, GameObject* pGameObject);
    bool OnGossipSelect(Player* pPlayer, GameObject* pGameObject, uint32 sender, uint32 action);
    bool OnGossipSelectCode(Player* pPlayer, GameObject* pGameObject, uint32 sender, uint32 action, const char* code);
    bool OnQuestAccept(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest);
    bool OnQuestReward(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest, uint32 opt);
    void GetDialogStatus(const Player* pPlayer, const GameObject* pGameObject);
    void OnDestroyed(GameObject* pGameObject, WorldObject* attacker);
    void OnDamaged(GameObject* pGameObject, WorldObject* attacker);
    void OnLootStateChanged(GameObject* pGameObject, uint32 state);
    void OnGameObjectStateChanged(GameObject* pGameObject, uint32 state);
    void UpdateAI(GameObject* pGameObject, uint32 diff);
    void OnSpawn(GameObject* gameobject);

    /* Packet */
    bool OnPacketSend(WorldSession* session, const WorldPacket& packet);
    void OnPacketSendAny(Player* player, const WorldPacket& packet, bool& result);
    void OnPacketSendOne(Player* player, const WorldPacket& packet, bool& result);
    bool OnPacketReceive(WorldSession* session, WorldPacket& packet);
    void OnPacketReceiveAny(Player* player, WorldPacket& packet, bool& result);
    void OnPacketReceiveOne(Player* player, WorldPacket& packet, bool& result);

    /* Player */
    void OnPlayerEnterCombat(Player* pPlayer, Unit* pEnemy);
    void OnPlayerLeaveCombat(Player* pPlayer);
    void OnPVPKill(Player* pKiller, Player* pKilled);
    void OnCreatureKill(Player* pKiller, Creature* pKilled);
    void OnPlayerKilledByCreature(Creature* pKiller, Player* pKilled);
    void OnPlayerKilledByEnvironment(Player* pKilled, uint8 damageType);
    void OnLevelChanged(Player* pPlayer, uint8 oldLevel);
    void OnFreeTalentPointsChanged(Player* pPlayer, uint32 newPoints);
    void OnTalentsReset(Player* pPlayer, bool noCost);
    void OnMoneyChanged(Player* pPlayer, int32& amount);
    void OnGiveXP(Player* pPlayer, uint32& amount, Unit* pVictim);
    void OnReputationChange(Player* pPlayer, uint32 factionID, int32& standing, bool incremental);
    void OnDuelRequest(Player* pTarget, Player* pChallenger);
    void OnDuelStart(Player* pStarter, Player* pChallenger);
    void OnDuelEnd(Player* pWinner, Player* pLoser, DuelCompleteType type);
    bool OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg);
    bool OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg, Group* pGroup);
    bool OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg, Guild* pGuild);
    bool OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg, Channel* pChannel);
    bool OnChat(Player* pPlayer, uint32 type, uint32 lang, std::string& msg, Player* pReceiver);
    void OnEmote(Player* pPlayer, uint32 emote);
    void OnTextEmote(Player* pPlayer, uint32 textEmote, uint32 emoteNum, ObjectGuid guid);
    void OnSpellCast(Player* pPlayer, Spell* pSpell, bool skipCheck);
    void OnLogin(Player* pPlayer);
    void OnLogout(Player* pPlayer);
    void OnCreate(Player* pPlayer);
    void OnDelete(uint32 guid);
    void OnSave(Player* pPlayer);
    void OnBindToInstance(Player* pPlayer, Difficulty difficulty, uint32 mapid, bool permanent);
    void OnUpdateZone(Player* pPlayer, uint32 newZone, uint32 newArea);
    void OnUpdateArea(Player* pPlayer, uint32 oldArea, uint32 newArea);
    void OnMapChanged(Player* pPlayer);
    void HandleGossipSelectOption(Player* pPlayer, uint32 menuId, uint32 sender, uint32 action, const std::string& code);
    void OnAchievementComplete(Player* pPlayer, uint32 achievementId);

    /* Vehicle */
    void OnInstall(Vehicle* vehicle);
    void OnUninstall(Vehicle* vehicle);
    void OnInstallAccessory(Vehicle* vehicle, Creature* accessory);
    void OnAddPassenger(Vehicle* vehicle, Unit* passenger, int8 seatId);
    void OnRemovePassenger(Vehicle* vehicle, Unit* passenger);

    /* AreaTrigger */
    bool OnAreaTrigger(Player* pPlayer, AreaTriggerEntry const* pTrigger);

    /* Weather */
    void OnChange(Weather* weather, uint32 zone, WeatherState state, float grade);

    /* Auction House */
    void OnAdd(AuctionHouseObject* ah, AuctionEntry* entry);
    void OnRemove(AuctionHouseObject* ah, AuctionEntry* entry);
    void OnSuccessful(AuctionHouseObject* ah, AuctionEntry* entry);
    void OnExpire(AuctionHouseObject* ah, AuctionEntry* entry);

    /* Guild */
    void OnAddMember(Guild* guild, Player* player, uint32 plRank);
    void OnRemoveMember(Guild* guild, Player* player, bool isDisbanding);
    void OnMOTDChanged(Guild* guild, const std::string& newMotd);
    void OnInfoChanged(Guild* guild, const std::string& newInfo);
    void OnCreate(Guild* guild, Player* leader, const std::string& name);
    void OnDisband(Guild* guild);
    void OnMemberWitdrawMoney(Guild* guild, Player* player, uint32& amount, bool isRepair);
    void OnMemberDepositMoney(Guild* guild, Player* player, uint32& amount);
    void OnItemMove(Guild* guild, Player* player, Item* pItem, bool isSrcBank, uint8 srcContainer, uint8 srcSlotId, bool isDestBank, uint8 destContainer, uint8 destSlotId);
    void OnEvent(Guild* guild, uint8 eventType, uint32 playerGuid1, uint32 playerGuid2, uint8 newRank);
    void OnBankEvent(Guild* guild, uint8 eventType, uint8 tabId, uint32 playerGuid, uint32 itemOrMoney, uint16 itemStackCount, uint8 destTabId);

    /* Group */
    void OnAddMember(Group* group, ObjectGuid guid);
    void OnInviteMember(Group* group, ObjectGuid guid);
    void OnRemoveMember(Group* group, ObjectGuid guid, uint8 method);
    void OnChangeLeader(Group* group, ObjectGuid newLeaderGuid, ObjectGuid oldLeaderGuid);
    void OnDisband(Group* group);
    void OnCreate(Group* group, ObjectGuid leaderGuid, GroupType groupType);
    bool OnMemberAccept(Group* group, Player* player);

    /* Map */
    void OnCreate(Map* map);
    void OnDestroy(Map* map);
    void OnPlayerEnter(Map* map, Player* player);
    void OnPlayerLeave(Map* map, Player* player);
    void OnUpdate(Map* map, uint32 diff);
    void OnAddToWorld(Creature* creature);
    void OnRemoveFromWorld(Creature* creature);
    void OnAddToWorld(GameObject* gameobject);
    void OnRemoveFromWorld(GameObject* gameobject);
    void OnRemove(Creature* creature);
    void OnRemove(GameObject* gameobject);

    /* Instance */
    void OnInitialize(ForgeInstanceAI* ai);
    void OnLoad(ForgeInstanceAI* ai);
    void OnUpdateInstance(ForgeInstanceAI* ai, uint32 diff);
    void OnPlayerEnterInstance(ForgeInstanceAI* ai, Player* player);
    void OnCreatureCreate(ForgeInstanceAI* ai, Creature* creature);
    void OnGameObjectCreate(ForgeInstanceAI* ai, GameObject* gameobject);
    bool OnCheckEncounterInProgress(ForgeInstanceAI* ai);

    /* World */
    void OnOpenStateChange(bool open);
    void OnConfigLoad(bool reload);
    void OnShutdownInitiate(ShutdownExitCode code, ShutdownMask mask);
    void OnShutdownCancel();
    void OnStartup();
    void OnShutdown();
    void OnGameEventStart(uint32 eventid);
    void OnGameEventStop(uint32 eventid);

    /* Battle Ground */
    void OnBGStart(BattleGround* bg, BattleGroundTypeId bgId, uint32 instanceId);
    void OnBGEnd(BattleGround* bg, BattleGroundTypeId bgId, uint32 instanceId, Team winner);
    void OnBGCreate(BattleGround* bg, BattleGroundTypeId bgId, uint32 instanceId);
    void OnBGDestroy(BattleGround* bg, BattleGroundTypeId bgId, uint32 instanceId);

    /* Spell */
    void OnSpellCast(Spell* pSpell, bool skipCheck);
};
template<> Unit* Forge::CHECKOBJ<Unit>(int narg, bool error);
template<> Object* Forge::CHECKOBJ<Object>(int narg, bool error);
template<> WorldObject* Forge::CHECKOBJ<WorldObject>(int narg, bool error);
template<> ForgeObject* Forge::CHECKOBJ<ForgeObject>(int narg, bool error);

#endif
