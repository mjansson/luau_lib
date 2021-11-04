/* luau.c  -  Luau library  -  Public Domain  -  2021 Mattias Jansson
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

#include <luau/luau.h>

#include <foundation/foundation.h>

static bool luau_module_initialized;
static luau_config_t luau_config;

int
luau_module_initialize(const luau_config_t config) {
	FOUNDATION_UNUSED(config);

	if (luau_module_initialized)
		return 0;

	luau_module_initialized = true;

	return 0;
}

void
luau_module_finalize(void) {
	if (!luau_module_initialized)
		return;

	luau_module_initialized = false;
}

bool
luau_module_is_initialized(void) {
	return luau_module_initialized;
}

luau_config_t
luau_module_config(void) {
	return luau_config;
}

void
luau_module_parse_config(const char* path, size_t path_size, const char* buffer, size_t size, const json_token_t* tokens,
                         size_t tokens_count) {
	FOUNDATION_UNUSED(path);
	FOUNDATION_UNUSED(path_size);
	FOUNDATION_UNUSED(buffer);
	FOUNDATION_UNUSED(size);
	FOUNDATION_UNUSED(tokens);
	FOUNDATION_UNUSED(tokens_count);
}
