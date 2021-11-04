/* luau.h  -  Luau library  -  Public Domain  -  2021 Mattias Jansson
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

/*! \file luau.h
    Luau scripting environment */

#include <foundation/platform.h>

#include <luau/build.h>
#include <luau/types.h>

LUAU_API int
lua_module_initialize(const luau_config_t config);

LUAU_API void
luau_module_finalize(void);

LUAU_API bool
luau_module_is_initialized(void);

LUAU_API version_t
luau_module_version(void);

LUAU_API luau_config_t
luau_module_config(void);

/*! Parse config declarations from JSON buffer
\param path Path
\param path_size Length of path
\param buffer Data buffer
\param size Size of data buffer
\param tokens JSON tokens
\param tokens_count Number of JSON tokens */
LUAU_API void
luau_module_parse_config(const char* path, size_t path_size, const char* buffer, size_t size, const json_token_t* tokens,
                        size_t tokens_count);
