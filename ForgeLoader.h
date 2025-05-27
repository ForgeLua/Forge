/*
* Copyright (C) 2010 - 2024 Forge Lua Engine <https://forgeluaengine.github.io/>
* Copyright (C) 2022 - 2022 Hour of Twilight <https://www.houroftwilight.net/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef _FORGELOADER_H
#define _FORGELOADER_H

#include "LuaEngine.h"

#include <efsw/efsw.hpp>

extern "C"
{
#include "lua.h"
};

enum ForgeReloadActions
{
    RELOAD_CACHE_ONLY   = -3,
    RELOAD_ALL_STATES   = -2,
    RELOAD_GLOBAL_STATE = -1
};

enum ForgeScriptCacheState
{
    SCRIPT_CACHE_NONE = 0,
    SCRIPT_CACHE_REINIT = 1,
    SCRIPT_CACHE_LOADING = 2,
    SCRIPT_CACHE_READY = 3
};

struct LuaScript;

class ForgeLoader
{
private:
    ForgeLoader();
    ~ForgeLoader();

public:
    ForgeLoader(ForgeLoader const&) = delete;
    ForgeLoader(ForgeLoader&&) = delete;

    ForgeLoader& operator= (ForgeLoader const&) = delete;
    ForgeLoader& operator= (ForgeLoader&&) = delete;
    static ForgeLoader* instance();

    void LoadScripts();
    void ReloadForgeForMap(int mapId);

    uint8 GetCacheState() const { return m_cacheState; }
    const std::vector<LuaScript>& GetLuaScripts() const { return m_scriptCache; }
    const std::string& GetRequirePath() const { return m_requirePath; }
    const std::string& GetRequireCPath() const { return m_requirecPath; }

    // efsw file watcher
    void InitializeFileWatcher();
    efsw::FileWatcher lua_fileWatcher;
    efsw::WatchID lua_scriptWatcher;

private:
    void ReloadScriptCache();
    void ReadFiles(lua_State* L, std::string path);
    void CombineLists();
    void ProcessScript(lua_State* L, std::string filename, const std::string& fullpath, int32 mapId);
    bool CompileScript(lua_State* L, LuaScript& script);
    static int LoadBytecodeChunk(lua_State* L, uint8* bytes, size_t len, BytecodeBuffer* buffer);

    std::atomic<uint8> m_cacheState;
    std::vector<LuaScript> m_scriptCache;
    std::string m_requirePath;
    std::string m_requirecPath;
    std::list<LuaScript> m_scripts;
    std::list<LuaScript> m_extensions;
    std::thread m_reloadThread;
};

/// File watcher responsible for watching lua scripts
class ForgeUpdateListener : public efsw::FileWatchListener
{
public:
    ForgeUpdateListener() { }
    virtual ~ForgeUpdateListener() { }

    void handleFileAction(efsw::WatchID /*watchid*/, std::string const& dir,
        std::string const& filename, efsw::Action /*action*/, std::string oldFilename = "") final override;
};

static ForgeUpdateListener forgeUpdateListener;

#define sForgeLoader ForgeLoader::instance()

#endif
