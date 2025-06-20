/*
* Copyright (C) 2010 - 2024 Forge Lua Engine <https://forgeluaengine.github.io/>
* Copyright (C) 2022 - 2022 Hour of Twilight <https://www.houroftwilight.net/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "ForgeCompat.h"
#include "ForgeConfig.h"
#include "ForgeLoader.h"
#include "ForgeUtility.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <charconv>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#if defined FORGE_WINDOWS
#include <Windows.h>
#endif

#include "MapManager.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

void ForgeUpdateListener::handleFileAction(efsw::WatchID /*watchid*/, std::string const& dir, std::string const& filename, efsw::Action /*action*/, std::string /*oldFilename*/)
{
    auto const path = fs::absolute(filename, dir);
    if (!path.has_extension())
        return;

    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });

    if (ext != ".lua" && ext != ".ext" && ext != ".moon")
        return;

    sForgeLoader->ReloadForgeForMap(RELOAD_ALL_STATES);
}

ForgeLoader::ForgeLoader() : m_cacheState(SCRIPT_CACHE_NONE)
{
    lua_scriptWatcher = -1;
}

ForgeLoader* ForgeLoader::instance()
{
    static ForgeLoader instance;
    return &instance;
}

ForgeLoader::~ForgeLoader()
{
    // join any previously created reload thread so it can exit cleanly
    if (m_reloadThread.joinable())
        m_reloadThread.join();

    if (lua_scriptWatcher >= 0)
    {
        lua_fileWatcher.removeWatch(lua_scriptWatcher);
        lua_scriptWatcher = -1;
    }
}

void ForgeLoader::ReloadScriptCache()
{
    // if the internal cache state is anything other than ready, we return
    if (m_cacheState != SCRIPT_CACHE_READY)
    {
        FORGE_LOG_DEBUG("[Forge]: Script cache not ready, skipping reload");
        return;
    }

    // try to join any previous thread before starting a new one, just in case
    if (m_reloadThread.joinable())
        m_reloadThread.join();

    // set the internal cache state to reinit
    m_cacheState = SCRIPT_CACHE_REINIT;

    // create new thread to load scripts asynchronously
    m_reloadThread = std::thread(&ForgeLoader::LoadScripts, this);
    FORGE_LOG_DEBUG("[Forge]: Script cache reload thread started");
}

void ForgeLoader::LoadScripts()
{
    // only reload the cache if it is either in a reinit state or not loaded at all
    if (m_cacheState != SCRIPT_CACHE_REINIT && m_cacheState != SCRIPT_CACHE_NONE)
        return;

    // set the cache state to loading
    m_cacheState = SCRIPT_CACHE_LOADING;

    uint32 oldMSTime = ForgeUtil::GetCurrTime();

    std::string lua_folderpath = sForgeConfig->GetConfig(CONFIG_FORGE_SCRIPT_PATH);
    const std::string& lua_path_extra = sForgeConfig->GetConfig(CONFIG_FORGE_REQUIRE_PATH_EXTRA);
    const std::string& lua_cpath_extra = sForgeConfig->GetConfig(CONFIG_FORGE_REQUIRE_CPATH_EXTRA);
    
#if !defined FORGE_WINDOWS
    if (lua_folderpath[0] == '~')
        if (const char* home = getenv("HOME"))
            lua_folderpath.replace(0, 1, home);
#endif

    FORGE_LOG_INFO("[Forge]: Searching for scripts in `%s`", lua_folderpath.c_str());

    // open a new temporary Lua state to compile bytecode in
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // clear all cache variables
    m_requirePath.clear();
    m_requirecPath.clear();

    // read and compile all scripts
    ReadFiles(L, lua_folderpath);

    // close temporary Lua state
    lua_close(L);

    // combine lists of Lua scripts and extensions
    CombineLists();

    // append our custom require paths and cpaths if the config variables are not empty
    if (!lua_path_extra.empty())
        m_requirePath += lua_path_extra;

    if (!lua_cpath_extra.empty())
        m_requirecPath += lua_cpath_extra;

    // Erase last ;
    if (!m_requirePath.empty())
        m_requirePath.erase(m_requirePath.end() - 1);

    if (!m_requirecPath.empty())
        m_requirecPath.erase(m_requirecPath.end() - 1);

    FORGE_LOG_INFO("[Forge]: Loaded and precompiled %u scripts in %u ms", uint32(m_scriptCache.size()), ForgeUtil::GetTimeDiff(oldMSTime));

    // set the cache state to ready
    m_cacheState = SCRIPT_CACHE_READY;
}

int ForgeLoader::LoadBytecodeChunk(lua_State* /*L*/, uint8* bytes, size_t len, BytecodeBuffer* buffer)
{
    buffer->insert(buffer->end(), bytes, bytes + len);

    return 0;
}

// Finds lua script files from given path (including subdirectories) and pushes them to scripts
void ForgeLoader::ReadFiles(lua_State* L, std::string path)
{
    std::string lua_folderpath = sForgeConfig->GetConfig(CONFIG_FORGE_SCRIPT_PATH);

    FORGE_LOG_DEBUG("[Forge]: ReadFiles from path `%s`", path.c_str());

    fs::path someDir(path);
    fs::directory_iterator end_iter;

    if (fs::exists(someDir) && fs::is_directory(someDir) && !fs::is_empty(someDir))
    {
        m_requirePath +=
            path + "/?.lua;" +
            path + "/?.ext;" +
            path + "/?.moon;";

        m_requirecPath +=
            path + "/?.dll;" +
            path + "/?.so;";

        for (fs::directory_iterator dir_iter(someDir); dir_iter != end_iter; ++dir_iter)
        {
            std::string fullpath = dir_iter->path().generic_string();
            // Check if file is hidden
#if defined FORGE_WINDOWS
            DWORD dwAttrib = GetFileAttributes(fullpath.c_str());
            if (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_HIDDEN))
                continue;
#else
            std::string name = dir_iter->path().filename().generic_string().c_str();
            if (name[0] == '.')
                continue;
#endif

            // load subfolder
            if (fs::is_directory(dir_iter->status()))
            {
                ReadFiles(L, fullpath);
                continue;
            }

            if (fs::is_regular_file(dir_iter->status()))
            {
                // default to all maps
                int32 mapId = -1;

                // strip base folder path and trailing slash from fullpath
                std::string subfolder = dir_iter->path().generic_string();
                subfolder = subfolder.erase(0, lua_folderpath.size() + 1);

                // convert subfolder name to an integer
                auto [ptr, ec] = std::from_chars(subfolder.data(), subfolder.data() + subfolder.size(), mapId);

                // default to all maps on invalid map id or map id less than -1
                if (ec == std::errc::invalid_argument || ec == std::errc::result_out_of_range || mapId < -1)
                    mapId = -1;

                // was file, try add
                std::string filename = dir_iter->path().filename().generic_string();
                size_t filesize = fs::file_size(dir_iter->path());
                ProcessScript(L, filename, filesize, fullpath, mapId);
            }
        }
    }
}

bool ForgeLoader::CompileScript(lua_State* L, LuaScript& script)
{
    // Attempt to load the file
    int err = 0;
    if (script.fileext == ".moon")
    {
        std::string str = "return require('moonscript').loadfile([[" + script.filepath+ "]])";
        err = luaL_dostring(L, str.c_str());
    } else
        err = luaL_loadfile(L, script.filepath.c_str());

    // If something bad happened, try to find an error.
    if (err != 0)
    {
        FORGE_LOG_ERROR("[Forge]: CompileScript failed to load the Lua script `%s`.", script.filename.c_str());
        Forge::Report(L);
        return false;
    }
    FORGE_LOG_DEBUG("[Forge]: CompileScript loaded Lua script `%s`", script.filename.c_str());

    // Everything's OK so far, the script has been loaded, now we need to start dumping it to bytecode.
    err = lua_dump(L, (lua_Writer)LoadBytecodeChunk, &script.bytecode);
    if (err || script.bytecode.empty())
    {
        FORGE_LOG_ERROR("[Forge]: CompileScript failed to dump the Lua script `%s` to bytecode.", script.filename.c_str());
        Forge::Report(L);
        return false;
    }
    FORGE_LOG_DEBUG("[Forge]: CompileScript dumped Lua script `%s` to bytecode.", script.filename.c_str());

    // pop the loaded function from the stack
    lua_pop(L, 1);
    return true;
}

void ForgeLoader::ProcessScript(lua_State* L, std::string filename, const size_t& filesize, const std::string& fullpath, int32 mapId)
{
    FORGE_LOG_DEBUG("[Forge]: ProcessScript checking file `%s`", fullpath.c_str());

    // split file name
    std::size_t extDot = filename.find_last_of('.');
    if (extDot == std::string::npos)
        return;
    std::string ext = filename.substr(extDot);
    filename = filename.substr(0, extDot);

    // check extension and add path to scripts to load
    if (ext != ".lua" && ext != ".ext" && ext != ".moon")
        return;
    bool extension = ext == ".ext";

    LuaScript script;
    script.fileext = ext;
    script.filename = filename;
    script.filepath = fullpath;
    script.modulepath = fullpath.substr(0, fullpath.length() - filename.length() - ext.length());
    script.bytecode.reserve(filesize);
    script.mapId = mapId;

    // if compilation fails, we don't add the script 
    if (!CompileScript(L, script))
        return;

    if (extension)
        m_extensions.push_back(script);
    else
        m_scripts.push_back(script);

    FORGE_LOG_DEBUG("[Forge]: ProcessScript processed `%s` successfully", fullpath.c_str());
}

void ForgeLoader::InitializeFileWatcher()
{
    std::string lua_folderpath = sForgeConfig->GetConfig(CONFIG_FORGE_SCRIPT_PATH);

    lua_scriptWatcher = lua_fileWatcher.addWatch(lua_folderpath, &forgeUpdateListener, true);
    if (lua_scriptWatcher >= 0)
    {
        FORGE_LOG_INFO("[Forge]: Script reloader is listening on `%s`.", lua_folderpath.c_str());
    }
    else
    {
        FORGE_LOG_INFO("[Forge]: Failed to initialize the script reloader on `%s`.", lua_folderpath.c_str());
    }

    lua_fileWatcher.watch();
}

static bool ScriptPathComparator(const LuaScript& first, const LuaScript& second)
{
    return first.filepath < second.filepath;
}

void ForgeLoader::CombineLists()
{
    m_extensions.sort(ScriptPathComparator);
    m_scripts.sort(ScriptPathComparator);

    m_scriptCache.clear();
    m_scriptCache.reserve(m_extensions.size() + m_scripts.size());

    std::move(m_extensions.begin(), m_extensions.end(), std::back_inserter(m_scriptCache));
    std::move(m_scripts.begin(), m_scripts.end(), std::back_inserter(m_scriptCache));

    m_extensions.clear();
    m_scripts.clear();
}

void ForgeLoader::ReloadForgeForMap(int mapId)
{
    // reload the script cache asynchronously
    ReloadScriptCache();

    // If a mapid is provided but does not match any map or reserved id then only script storage is loaded
    if (mapId != RELOAD_CACHE_ONLY)
    {
        if (mapId == RELOAD_GLOBAL_STATE || mapId == RELOAD_ALL_STATES)
            if (Forge* f = sWorld->GetForge())
                f->ReloadForge();

        sMapMgr->DoForAllMaps([&](Map* map)
            {
                if (mapId == RELOAD_ALL_STATES || mapId == static_cast<int>(map->GetId()))
                    if (Forge* f = map->GetForge())
                        f->ReloadForge();
            }
        );
    }
}
