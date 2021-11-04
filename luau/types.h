/* build.h  -  Luau library  -  Public Domain  -  2021 Mattias Jansson
 *
 * This library provides a cross-platform lua library in C11 for games and applications
 * based on our foundation library. The latest source code is always available at
 *
 * https://github.com/mjansson/luau_lib
 *
 * This library is put in the public domain; you can redistribute it and/or modify it without
 * any restrictions.
 *
 * The Luau library is released under the MIT license. For more information about Luau, see
 * https://luau-lang.org/
 */

#pragma once

/*! \file types.h
    Data types used by Luau scripting library */

#include <foundation/platform.h>
#include <foundation/types.h>

#include <luau/build.h>

#if defined(LUAU_COMPILE) && LUAU_COMPILE
#ifdef __cplusplus
#define LUAU_EXTERN extern "C"
#define LUAU_API extern "C"
#else
#define LUAU_EXTERN extern
#define LUAU_API extern
#endif
#else
#ifdef __cplusplus
#define LUAU_EXTERN extern "C"
#define LUAU_API extern "C"
#else
#define LUAU_EXTERN extern
#define LUAU_API extern
#endif
#endif

typedef struct lua_State luau_State;

typedef struct luau_config_t luau_config_t;
typedef struct luau_t luau_t;

struct luau_config_t {
	unsigned int unused;
};

struct luau_t {
    luau_State* state;
};
