#pragma once

//----------------------------------------------------------------------
// Configuration
//----------------------------------------------------------------------

#define NG_ASSERTION_ENABLED

//----------------------------------------------------------------------
//----------------------------------------------------------------------

void assertion_abort(const char *assertion, const char *file,
	int line, const char *func);

#ifdef NG_ASSERTION_ENABLED
	#define NG_ASSERT(expr)                  \
	do {                                     \
		if (!(expr)) {                       \
			assertion_abort(#expr,           \
				__FILE__, __LINE__,          \
				__PRETTY_FUNCTION__);        \
		}                                    \
	} while (0)
#else
	#define NG_ASSERT(expr) ((void)0)
#endif

#define NG_SLICE_BOUNDS_CHECK(index, length) \
	NG_ASSERT((unsigned)(index) <= (unsigned)(length))

#define NG_IDX_BOUNDS_CHECK(index, length) \
	NG_ASSERT((unsigned)(index) < (unsigned)(length))

void die(const char *msg, ...);
void warn(const char *msg, ...);

#define NG_DELETE_COPY(Type) \
	Type(const Type&) = delete; \
	Type &operator=(const Type&) = delete

#define NG_DELETE_MOVE(Type) \
	Type(Type&&) = delete; \
	Type &operator=(Type&&) = delete

#define NG_DELETE_COPY_AND_MOVE(Type) \
	NG_DELETE_COPY(Type); \
	NG_DELETE_MOVE(Type)

#ifdef _WIN32
	#define NG_LUA_API extern "C" __declspec(dllexport)
#else
	#define NG_LUA_API extern "C"
#endif

#define NG_ENABLE_SINGLETON(globalptr)            \
do {                                              \
	if (globalptr) {                              \
		die("There can only be one " #globalptr); \
	}                                             \
	globalptr = this;                             \
} while (0)

#define NG_DISABLE_SINGLETON(globalptr) \
do {                                    \
	globalptr = nullptr;                \
} while (0)

