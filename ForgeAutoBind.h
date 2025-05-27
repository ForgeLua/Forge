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

#ifndef FORGE_AUTO_BIND_H
#define FORGE_AUTO_BIND_H

#include "LuaEngine.h"
#include <type_traits>
#include <tuple>
#include <utility>
#include <cstdint>

namespace ForgeAutoBind {

    // ===== TYPE TRAITS FOR AUTOMATIC FUNCTION DETECTION =====
    template<typename T>
    struct function_traits;

    template<typename R, typename C, typename... Args>
    struct function_traits<R(C::*)(Args...)> {
        using return_type = R;
        using class_type = C;
        using args_tuple = std::tuple<Args...>;
        static constexpr size_t arity = sizeof...(Args);
        static constexpr bool is_const = false;
    };

    template<typename R, typename C, typename... Args>
    struct function_traits<R(C::*)(Args...) const> {
        using return_type = R;
        using class_type = C;
        using args_tuple = std::tuple<Args...>;
        static constexpr size_t arity = sizeof...(Args);
        static constexpr bool is_const = true;
    };

    // ===== AUTOMATIC PUSH/CHECK SYSTEM FOR FORGE =====
    template<typename T>
    struct forge_type_handler {
        static T get(Forge* F, int index) {
            return F->CHECKVAL<T>(index);
        }

        static void push(Forge* F, const T& value) {
            F->Push(value);
        }
    };

    template<typename T>
    struct forge_type_handler<T*> {
        static T* get(Forge* F, int index) {
            return F->CHECKOBJ<T>(index);
        }

        static void push(Forge* F, T* value) {
            F->Push(value);
        }
    };

    // ===== WRAPPERS FOR METHODS =====

    // Wrapper for getters (no arguments, return value only)
    template<typename R, typename C>
    int auto_getter(Forge* F, C* obj, R(C::*method)() const) {
        R result = (obj->*method)();
        forge_type_handler<R>::push(F, result);
        return 1; // Return count
    }

    // Wrapper for setters (one argument, no return value)
    template<typename C, typename Arg>
    int auto_setter(Forge* F, C* obj, void(C::*method)(Arg)) {
        Arg arg = forge_type_handler<Arg>::get(F, 2);
        (obj->*method)(arg);
        return 0; // No return value
    }

    // Wrapper for calls (no arguments, no return value)
    template<typename C>
    int auto_call(Forge* F, C* obj, void(C::*method)()) {
        (obj->*method)();
        return 0; // No return value
    }

    // Wrapper for setters with variadic arguments
    template<typename C, typename... Args>
    int auto_setter(Forge* F, C* obj, void(C::*method)(Args...)) {
        auto args = std::make_tuple(forge_type_handler<Args>::get(F, 2 + sizeof...(Args) - sizeof...(Args))...);

        std::apply([obj, method](Args... args) { (obj->*method)(args...); }, args);

        return 0; // No return value
    }

    // Wrapper for methods with arguments and no return value
    template<typename C, typename... Args>
    int auto_void_method(Forge* F, C* obj, void(C::*method)(Args...)) {
        auto args = std::make_tuple(forge_type_handler<Args>::get(F, 2)...);

        std::apply([obj, method](Args... args) { (obj->*method)(args...); }, args);
        return 0; // No return value
    }

} // namespace ForgeAutoBind

// ===== MACROS =====

#define FORGE_AUTO_GETTER(ClassName, MethodName) \
    int MethodName(Forge* F, ClassName* obj) { \
        return ForgeAutoBind::auto_getter(F, obj, &ClassName::MethodName); \
    }

#define FORGE_AUTO_SETTER(ClassName, MethodName, ...) \
    int MethodName(Forge* F, ClassName* obj) { \
        return ForgeAutoBind::auto_setter(F, obj, &ClassName::MethodName); \
    }

#define FORGE_AUTO_CALL(ClassName, MethodName) \
    int MethodName(Forge* F, ClassName* obj) { \
        return ForgeAutoBind::auto_call(F, obj, &ClassName::MethodName); \
    }

#define FORGE_AUTO_METHOD(ClassName, MethodName, ...) \
    int MethodName(Forge* F, ClassName* obj) { \
        return ForgeAutoBind::auto_method(F, obj, &ClassName::MethodName); \
    }

#define FORGE_AUTO_VOID_METHOD(ClassName, MethodName, ...) \
    int MethodName(Forge* F, ClassName* obj) { \
        return ForgeAutoBind::auto_void_method(F, obj, &ClassName::MethodName); \
    }

#endif // FORGE_AUTO_BIND_H