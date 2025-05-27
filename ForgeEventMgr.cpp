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

#include "ForgeEventMgr.h"
#include "LuaEngine.h"
#if !defined FORGE_CMANGOS
#include "Object.h"
#else
#include "Entities/Object.h"
#endif

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
};

ForgeEventProcessor::ForgeEventProcessor(Forge* _F, WorldObject* _obj) : m_time(0), obj(_obj), F(_F)
{
    if (obj)
        F->eventMgr->processors.insert(this);
}

ForgeEventProcessor::~ForgeEventProcessor()
{
    {
        RemoveEvents_internal();
    }

    if (obj)
        F->eventMgr->processors.erase(this);
}

void ForgeEventProcessor::Update(uint32 diff)
{
    m_time += diff;
    for (EventList::iterator it = eventList.begin(); it != eventList.end() && it->first <= m_time; it = eventList.begin())
    {
        LuaEvent* luaEvent = it->second;
        eventList.erase(it);

        if (luaEvent->state != LUAEVENT_STATE_ERASE)
            eventMap.erase(luaEvent->funcRef);

        if (luaEvent->state == LUAEVENT_STATE_RUN)
        {
            uint32 delay = luaEvent->delay;
            bool remove = luaEvent->repeats == 1;
            if (!remove)
                AddEvent(luaEvent); // Reschedule before calling incase RemoveEvents used

            // Call the timed event
            F->OnTimedEvent(luaEvent->funcRef, delay, luaEvent->repeats ? luaEvent->repeats-- : luaEvent->repeats, obj);

            if (!remove)
                continue;
        }

        // Event should be deleted (executed last time or set to be aborted)
        RemoveEvent(luaEvent);
    }
}

void ForgeEventProcessor::SetStates(LuaEventState state)
{
    for (EventList::iterator it = eventList.begin(); it != eventList.end(); ++it)
        it->second->SetState(state);
    if (state == LUAEVENT_STATE_ERASE)
        eventMap.clear();
}

void ForgeEventProcessor::RemoveEvents_internal()
{
    for (EventList::iterator it = eventList.begin(); it != eventList.end(); ++it)
        RemoveEvent(it->second);

    eventList.clear();
    eventMap.clear();
}

void ForgeEventProcessor::SetState(int eventId, LuaEventState state)
{
    if (eventMap.find(eventId) != eventMap.end())
        eventMap[eventId]->SetState(state);
    if (state == LUAEVENT_STATE_ERASE)
        eventMap.erase(eventId);
}

void ForgeEventProcessor::AddEvent(LuaEvent* luaEvent)
{
    luaEvent->GenerateDelay();
    eventList.insert(std::pair<uint64, LuaEvent*>(m_time + luaEvent->delay, luaEvent));
    eventMap[luaEvent->funcRef] = luaEvent;
}

void ForgeEventProcessor::AddEvent(int funcRef, uint32 min, uint32 max, uint32 repeats)
{
    AddEvent(new LuaEvent(funcRef, min, max, repeats));
}

void ForgeEventProcessor::RemoveEvent(LuaEvent* luaEvent)
{
    // Unreference if should and if Forge was not yet uninitialized and if the lua state still exists
    if (luaEvent->state != LUAEVENT_STATE_ERASE && F->HasLuaState())
    {
        // Free lua function ref
        luaL_unref(F->L, LUA_REGISTRYINDEX, luaEvent->funcRef);
    }
    delete luaEvent;
}

EventMgr::EventMgr(Forge* _F) : globalProcessor(new ForgeEventProcessor(_F, NULL)), F(_F)
{
}

EventMgr::~EventMgr()
{
    {
        if (!processors.empty())
            for (ProcessorSet::const_iterator it = processors.begin(); it != processors.end(); ++it) // loop processors
                (*it)->RemoveEvents_internal();
        globalProcessor->RemoveEvents_internal();
    }
    delete globalProcessor;
    globalProcessor = NULL;
}

void EventMgr::SetStates(LuaEventState state)
{
    if (!processors.empty())
        for (ProcessorSet::const_iterator it = processors.begin(); it != processors.end(); ++it) // loop processors
            (*it)->SetStates(state);
    globalProcessor->SetStates(state);
}

void EventMgr::SetState(int eventId, LuaEventState state)
{
    if (!processors.empty())
        for (ProcessorSet::const_iterator it = processors.begin(); it != processors.end(); ++it) // loop processors
            (*it)->SetState(eventId, state);
    globalProcessor->SetState(eventId, state);
}
