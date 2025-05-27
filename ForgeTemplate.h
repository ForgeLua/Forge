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

#ifndef _FORGE_TEMPLATE_H
#define _FORGE_TEMPLATE_H

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "LuaEngine.h"
#include "ForgeUtility.h"
#include "ForgeCompat.h"
#if !defined FORGE_CMANGOS
#include "SharedDefines.h"
#else
#include "Globals/SharedDefines.h"
#include "Util/UniqueTrackablePtr.h"
#endif

#if defined FORGE_TRINITY
#include "UniqueTrackablePtr.h"
#endif

class ForgeObject
{
public:
    ForgeObject(Forge* F, char const* tname) : F(F), type_name(tname)
    {
    }

    virtual ~ForgeObject()
    {
    }

    // Get wrapped object pointer
    virtual void* GetObjIfValid() const = 0;
    // Returns pointer to the wrapped object's type name
    const char* GetTypeName() const { return type_name; }
#if !defined TRACKABLE_PTR_NAMESPACE
    // Invalidates the pointer if it should be invalidated
    virtual void Invalidate() = 0;
#endif

protected:
    Forge* F;
    const char* type_name;
};

#if defined TRACKABLE_PTR_NAMESPACE
template <typename T>
struct ForgeConstrainedObjectRef
{
    TRACKABLE_PTR_NAMESPACE unique_weak_ptr<T> Obj;
    Map const* BoundMap = nullptr;
};

ForgeConstrainedObjectRef<Aura> GetWeakPtrFor(Aura const* obj);
ForgeConstrainedObjectRef<BattleGround> GetWeakPtrFor(BattleGround const* obj);
ForgeConstrainedObjectRef<Group> GetWeakPtrFor(Group const* obj);
ForgeConstrainedObjectRef<Guild> GetWeakPtrFor(Guild const* obj);
ForgeConstrainedObjectRef<Map> GetWeakPtrFor(Map const* obj);
ForgeConstrainedObjectRef<Object> GetWeakPtrForObjectImpl(Object const* obj);
ForgeConstrainedObjectRef<Quest> GetWeakPtrFor(Quest const* obj);
ForgeConstrainedObjectRef<Spell> GetWeakPtrFor(Spell const* obj);
#if FORGE_EXPANSION >= EXP_WOTLK
ForgeConstrainedObjectRef<Vehicle> GetWeakPtrFor(Vehicle const* obj);
#endif

template <typename T>
ForgeConstrainedObjectRef<T> GetWeakPtrFor(T const* obj)
{
    ForgeConstrainedObjectRef<Object> ref = GetWeakPtrForObjectImpl(obj);
    return { TRACKABLE_PTR_NAMESPACE static_pointer_cast<T>(ref.Obj), ref.BoundMap };
}

#endif

template <typename T>
class ForgeObjectImpl : public ForgeObject
{
public:
#if defined TRACKABLE_PTR_NAMESPACE
    ForgeObjectImpl(Forge* F, T const* obj, char const* tname) : ForgeObject(F, tname), _obj(GetWeakPtrFor(obj))
    {
    }

    void* GetObjIfValid() const override
    {
        if (TRACKABLE_PTR_NAMESPACE unique_strong_ref_ptr<T> obj = _obj.Obj.lock())
            if (!F->GetBoundMap() || !_obj.BoundMap || F->GetBoundMap() == _obj.BoundMap)
                return obj.get();

        return nullptr;
    }
#else
    ForgeObjectImpl(Forge* F, T* obj, char const* tname) : ForgeObject(F, tname), _obj(obj), callstackid(F->GetCallstackId())
    {
    }

    void* GetObjIfValid() const override
    {
        if (callstackid == F->GetCallstackId())
            return _obj;

        return nullptr;
    }

    void Invalidate() override { callstackid = 1; }
#endif

private:
#if defined TRACKABLE_PTR_NAMESPACE
    ForgeConstrainedObjectRef<T> _obj;
#else
    void* _obj;
    uint64 callstackid;
#endif
};

template <typename T>
class ForgeObjectValueImpl : public ForgeObject
{
public:
    ForgeObjectValueImpl(Forge* F, T const* obj, char const* tname) : ForgeObject(F, tname), _obj(*obj /*always a copy, what gets passed here might be pointing to something not owned by us*/)
    {
    }

    void* GetObjIfValid() const override { return const_cast<T*>(&_obj); }

#if !defined TRACKABLE_PTR_NAMESPACE
    void Invalidate() override { }
#endif

private:
    T _obj;
};

#define MAKE_FORGE_OBJECT_VALUE_IMPL(type) \
template <> \
class ForgeObjectImpl<type> : public ForgeObjectValueImpl<type> \
{ \
public: \
    using ForgeObjectValueImpl::ForgeObjectValueImpl; \
}

MAKE_FORGE_OBJECT_VALUE_IMPL(long long);
MAKE_FORGE_OBJECT_VALUE_IMPL(unsigned long long);
MAKE_FORGE_OBJECT_VALUE_IMPL(ObjectGuid);
MAKE_FORGE_OBJECT_VALUE_IMPL(WorldPacket);
MAKE_FORGE_OBJECT_VALUE_IMPL(ForgeQuery);

template<typename T = void>
struct ForgeRegister
{
    const char* name;
    typename std::conditional<std::is_same_v<T, void>, int(*)(Forge*), int(*)(Forge*, T*)>::type mfunc;
    MethodRegisterState regState;

    // constructor for non-globals (with T*)
    ForgeRegister(const char* name, int(*func)(Forge*, T*), MethodRegisterState state = METHOD_REG_ALL)
        : name(name), mfunc(func), regState(state) {}

    // constructor for globals (without T*)
    ForgeRegister(const char* name, int(*func)(Forge*), MethodRegisterState state = METHOD_REG_ALL)
        : name(name), mfunc(func), regState(state) {}

    // constructor for nullptr functions and METHOD_REG_NONE (unimplemented methods)
    ForgeRegister(const char* name, MethodRegisterState state = METHOD_REG_NONE)
        : name(name), mfunc(nullptr), regState(state) {}
};

template<typename T = void>
class ForgeTemplate
{
public:
    static const char* tname;

    // name will be used as type name
    // If gc is true, lua will handle the memory management for object pushed
    // gc should be used if pushing for example WorldPacket,
    // that will only be needed on lua side and will not be managed by TC/mangos/<core>
    static void Register(Forge* F, const char* name)
    {
        ASSERT(F);
        ASSERT(name);

        lua_State* L = F->L;

        // check that metatable isn't already there
        lua_getglobal(L, name);
        ASSERT(lua_isnoneornil(L, -1));

        // pop nil
        lua_pop(L, 1);

        tname = name;

        // create metatable for userdata of this type
        luaL_newmetatable(L, tname);
        int metatable = lua_gettop(L);

        // push methodtable to stack to be accessed and modified by users
        lua_pushvalue(L, metatable);
        lua_setglobal(L, tname);

        // tostring
        lua_pushcfunction(L, ToString);
        lua_setfield(L, metatable, "__tostring");

        // garbage collecting
        lua_pushcfunction(L, CollectGarbage);
        lua_setfield(L, metatable, "__gc");

        // TODO: Safe to remove this?
        // make methods accessible through metatable
        lua_pushvalue(L, metatable);
        lua_setfield(L, metatable, "__index");

        // make new indexes saved to methods
        lua_pushcfunction(L, Add);
        lua_setfield(L, metatable, "__add");

        // make new indexes saved to methods
        lua_pushcfunction(L, Subtract);
        lua_setfield(L, metatable, "__sub");

        // make new indexes saved to methods
        lua_pushcfunction(L, Multiply);
        lua_setfield(L, metatable, "__mul");

        // make new indexes saved to methods
        lua_pushcfunction(L, Divide);
        lua_setfield(L, metatable, "__div");

        // make new indexes saved to methods
        lua_pushcfunction(L, Mod);
        lua_setfield(L, metatable, "__mod");

        // make new indexes saved to methods
        lua_pushcfunction(L, Pow);
        lua_setfield(L, metatable, "__pow");

        // make new indexes saved to methods
        lua_pushcfunction(L, UnaryMinus);
        lua_setfield(L, metatable, "__unm");

        // make new indexes saved to methods
        lua_pushcfunction(L, Concat);
        lua_setfield(L, metatable, "__concat");

        // make new indexes saved to methods
        lua_pushcfunction(L, Length);
        lua_setfield(L, metatable, "__len");

        // make new indexes saved to methods
        lua_pushcfunction(L, Equal);
        lua_setfield(L, metatable, "__eq");

        // make new indexes saved to methods
        lua_pushcfunction(L, Less);
        lua_setfield(L, metatable, "__lt");

        // make new indexes saved to methods
        lua_pushcfunction(L, LessOrEqual);
        lua_setfield(L, metatable, "__le");

        // make new indexes saved to methods
        lua_pushcfunction(L, Call);
        lua_setfield(L, metatable, "__call");

        // special method to get the object type
        lua_pushcfunction(L, GetType);
        lua_setfield(L, metatable, "GetObjectType");

        // pop metatable
        lua_pop(L, 1);
    }

    template<typename C, size_t N>
    static void SetMethods(Forge* F, ForgeRegister<C> const (&methodTable)[N])
    {
        ASSERT(F);
        ASSERT(methodTable);

        lua_State* L = F->L;

        // determine if the method table functions are global or non-global
        constexpr bool isGlobal = std::is_same_v<C, void>;

        if constexpr (isGlobal)
        {
            lua_pushglobaltable(L);
        }
        else
        {
            ASSERT(tname);

            // get metatable
            lua_pushstring(L, tname);
            lua_rawget(L, LUA_REGISTRYINDEX);
            ASSERT(lua_istable(L, -1));
        }

        // load all core-specific methods
        for (std::size_t i = 0; i < N; i++)
        {
            const auto& method = methodTable + i;

            // push the method name to the Lua stack
            lua_pushstring(L, method->name);

            // if the method should not be registered, push a closure to error output function
            if (method->regState == METHOD_REG_NONE)
            {
                lua_pushstring(L, method->name);
                lua_pushcclosure(L, MethodUnimpl, 1);
                lua_rawset(L, -3);
                continue;
            }

            // if we're in multistate mode, we need to check whether a method is flagged as a world or a map specific method
            if (!F->GetCompatibilityMode() && method->regState != METHOD_REG_ALL)
            {
                int32 mapId = F->GetBoundMapId();

                // if the method should not be registered, push a closure to error output function
                if ((mapId == -1 && method->regState == METHOD_REG_MAP) ||
                    (mapId != -1 && method->regState == METHOD_REG_WORLD))
                {
                    lua_pushstring(L, method->name);
                    lua_pushinteger(L, mapId);
                    lua_pushcclosure(L, MethodWrongState, 2);
                    lua_rawset(L, -3);
                    continue;
                }
            }

            // push a closure to the thunk with the method pointer as light user data
            lua_pushlightuserdata(L, (void*)method);
            lua_pushcclosure(L, thunk, 1);
            lua_rawset(L, -3);
        }

        lua_pop(L, 1);
    }

    static int Push(Forge* F, T const* obj)
    {
        lua_State* L = F->L;
        if (!obj)
        {
            lua_pushnil(L);
            return 1;
        }

        typedef ForgeObjectImpl<T> ForgeObjectType;

        // Create new userdata
        ForgeObjectType* forgeObject = static_cast<ForgeObjectType*>(lua_newuserdata(L, sizeof(ForgeObjectType)));
        if (!forgeObject)
        {
            FORGE_LOG_ERROR("%s could not create new userdata", tname);
            lua_pushnil(L);
            return 1;
        }
        new (forgeObject) ForgeObjectType(F, const_cast<T*>(obj), tname);

        // Set metatable for it
        lua_pushstring(L, tname);
        lua_rawget(L, LUA_REGISTRYINDEX);
        if (!lua_istable(L, -1))
        {
            FORGE_LOG_ERROR("%s missing metatable", tname);
            lua_pop(L, 2);
            lua_pushnil(L);
            return 1;
        }
        lua_setmetatable(L, -2);
        return 1;
    }

    static T* Check(Forge* F, int narg, bool error = true)
    {
        lua_State* L = F->L;

        ForgeObject* forgeObj = F->CHECKTYPE(narg, tname, error);
        if (!forgeObj)
            return NULL;

        void* obj = forgeObj->GetObjIfValid();
        if (!obj)
        {
            char buff[256];
            snprintf(buff, 256, "%s expected, got pointer to nonexisting (invalidated) object (%s). Check your code.", tname, luaL_typename(L, narg));
            if (error)
            {
                luaL_argerror(L, narg, buff);
            }
            else
            {
                FORGE_LOG_ERROR("%s", buff);
            }
            return NULL;
        }
        return static_cast<T*>(obj);
    }

    static int GetType(lua_State* L)
    {
        lua_pushstring(L, tname);
        return 1;
    }

    static int thunk(lua_State* L)
    {
        ForgeRegister<T>* l = static_cast<ForgeRegister<T>*>(lua_touserdata(L, lua_upvalueindex(1)));
        Forge* F = Forge::GetForge(L);

        // determine if the method table functions are global or non-global
        constexpr bool isGlobal = std::is_same_v<T, void>;

        // we only check self if the method is not a global
        T* obj;
        if constexpr (!isGlobal)
        {
            obj = F->CHECKOBJ<T>(1);
            if (!obj)
                return 0;
        }

        int top = lua_gettop(L);

        int expected = 0;
        if constexpr (isGlobal)
            expected = l->mfunc(F);      // global method
        else
            expected = l->mfunc(F, obj); // non-global method

        int args = lua_gettop(L) - top;
        if (args < 0 || args > expected)
        {
            FORGE_LOG_ERROR("[Forge]: %s returned unexpected amount of arguments %i out of %i. Report to devs", l->name, args, expected);
            ASSERT(false);
        }
        lua_settop(L, top + expected);
        return expected;
    }

    // Metamethods ("virtual")

    // Remember special cases like ForgeTemplate<Vehicle>::CollectGarbage
    static int CollectGarbage(lua_State* L)
    {
        Forge* F = Forge::GetForge(L);

        // Get object pointer (and check type, no error)
        ForgeObject* obj = F->CHECKOBJ<ForgeObject>(1, false);
        obj->~ForgeObject();
        return 0;
    }

    static int ToString(lua_State* L)
    {
        Forge* F = Forge::GetForge(L);

        T* obj = F->CHECKOBJ<T>(1, true); // get self
        lua_pushfstring(L, "%s: %p", tname, obj);
        return 1;
    }

    static int ArithmeticError(lua_State* L) { return luaL_error(L, "attempt to perform arithmetic on a %s value", tname); }
    static int CompareError(lua_State* L) { return luaL_error(L, "attempt to compare %s", tname); }
    static int Add(lua_State* L) { return ArithmeticError(L); }
    static int Subtract(lua_State* L) { return ArithmeticError(L); }
    static int Multiply(lua_State* L) { return ArithmeticError(L); }
    static int Divide(lua_State* L) { return ArithmeticError(L); }
    static int Mod(lua_State* L) { return ArithmeticError(L); }
    static int Pow(lua_State* L) { return ArithmeticError(L); }
    static int UnaryMinus(lua_State* L) { return ArithmeticError(L); }
    static int Concat(lua_State* L) { return luaL_error(L, "attempt to concatenate a %s value", tname); }
    static int Length(lua_State* L) { return luaL_error(L, "attempt to get length of a %s value", tname); }
    static int Equal(lua_State* L) { Forge* F = Forge::GetForge(L); F->Push(F->CHECKOBJ<T>(1) == F->CHECKOBJ<T>(2)); return 1; }
    static int Less(lua_State* L) { return CompareError(L); }
    static int LessOrEqual(lua_State* L) { return CompareError(L); }
    static int Call(lua_State* L) { return luaL_error(L, "attempt to call a %s value", tname); }

    static int MethodWrongState(lua_State* L) { luaL_error(L, "attempt to call method '%s' that does not exist for state: %d", lua_tostring(L, lua_upvalueindex(1)), lua_tointeger(L, lua_upvalueindex(2))); return 0; }
    static int MethodUnimpl(lua_State* L) { luaL_error(L, "attempt to call method '%s' that is not implemented for this emulator", lua_tostring(L, lua_upvalueindex(1))); return 0; }
};

template<typename T> const char* ForgeTemplate<T>::tname = NULL;

template <typename T>
class ForgeTemplateHelper
{
public:
    static int PerformOp(lua_State* L, std::function<T(T, T)> op)
    {
        Forge* F = Forge::GetForge(L);
        T val1 = F->CHECKVAL<T>(1);
        T val2 = F->CHECKVAL<T>(2);
        F->Push(op(val1, val2));
        return 1;
    }

    static int PerformOp(lua_State* L, std::function<T(T)> op)
    {
        Forge* F = Forge::GetForge(L);

        T val = F->CHECKVAL<T>(1);
        F->Push(op(val));
        return 1;
    }

    static int ToString(lua_State* L)
    {
        Forge* F = Forge::GetForge(L);

        T val = F->CHECKVAL<T>(1);
        std::ostringstream ss;
        ss << val;
        F->Push(ss.str());
        return 1;
    }

    static int Pow(lua_State* L)
    {
        Forge* F = Forge::GetForge(L);

        T val1 = F->CHECKVAL<T>(1);
        T val2 = F->CHECKVAL<T>(2);
        F->Push(static_cast<T>(powl(static_cast<long double>(val1), static_cast<long double>(val2))));
        return 1;
    }
};

#endif
