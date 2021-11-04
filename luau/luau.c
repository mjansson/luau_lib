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

#include <lua.h>

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

static int
luau_panic(luau_State* state, int errcode) {
	FOUNDATION_UNUSED(errcode);
	string_const_t errmsg = {0, 0};
	errmsg.str = lua_tolstring(state, -1, &errmsg.length);
	if (!errmsg.length)
		errmsg = string_const(STRING_CONST("<no error message>"));
	log_errorf(HASH_LUAU, ERROR_SCRIPT, STRING_CONST("unprotected error in call to Lua API: %.*s (%d)"), STRING_FORMAT(errmsg), errcode);
	return 0;
}

static void*
luau_allocator(luau_State* state, void* userdata, void* block, size_t oldsize, size_t newsize) {
	FOUNDATION_UNUSED(state, userdata);
	if (!newsize && oldsize) {
		memory_deallocate(block);
	} else if (newsize) {
		if (!block)
			block = memory_allocate(HASH_LUAU, newsize, 0, MEMORY_PERSISTENT);
		else
			block = memory_reallocate(block, newsize, 0, oldsize, MEMORY_PERSISTENT);
		if (!block)
			log_panicf(HASH_LUAU, ERROR_OUT_OF_MEMORY,
			           STRING_CONST("Unable to allocate Luau memory (%" PRIsize " bytes)"), newsize);
	}
	return block;
}

luau_t*
luau_allocate(void) {
	luau_t* env = memory_allocate(HASH_LUAU, sizeof(luau_t), 0, MEMORY_PERSISTENT | MEMORY_ZERO_INITIALIZED);

	luau_State* state = env ? lua_newstate(luau_allocator, env) : nullptr;
	if (!state) {
		log_error(HASH_LUAU, ERROR_INTERNAL_FAILURE, STRING_CONST("Unable to allocate Lua state"));
		memory_deallocate(env);
		return 0;
	}

	lua_Callbacks* callbacks = lua_callbacks(state);
	callbacks->panic = luau_panic;

	// Disable automagic gc
	//lua_gc(state, LUA_GCCOLLECT, 0);

	env->state = state;
	//env->calldepth = 0;

#if 0
#if BUILD_ENABLE_LUA_THREAD_SAFE
	semaphore_initialize(&env->execution_right, 1);
	env->queue_head = 0;
	atomic_store32(&env->queue_tail, 0);
#endif

	int stacksize = lua_gettop(state);

	luaL_openlibs(state);

	lua_module_registry_initialize(state);

	lua_pop(state, lua_gettop(state) - stacksize);

	array_push(_lua_instances, env);
#endif
	return env;
}

void
luau_deallocate(luau_t* env) {
	if (!env)
		return;

	//FOUNDATION_ASSERT(env->calldepth == 0);
	FOUNDATION_ASSERT(env->state);

#if 0
	lua_gc(env->state, LUA_GCCOLLECT, 0);

	lua_module_registry_finalize(env->state);
#endif
	lua_close(env->state);

#if 0
#if BUILD_ENABLE_LUA_THREAD_SAFE
	semaphore_finalize(&env->execution_right);
#endif

	for (size_t ienv = 0, esize = array_size(_lua_instances); ienv != esize; ++ienv) {
		if (_lua_instances[ienv] == env) {
			array_erase(_lua_instances, ienv);
			break;
		}
	}
#endif
	memory_deallocate(env);
}
