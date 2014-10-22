#pragma once

#include <new>
#include <utility>
#include <type_traits>
#include <cstddef>

void *xmalloc(int n);
void xfree(void *ptr);
int xcopy(void *dst, const void *src, int n);
void xclear(void *dst, int n);

struct OrDie_t {};
const OrDie_t OrDie = {};

void *operator new(size_t size, const OrDie_t&);
void *operator new[](size_t size, const OrDie_t&);

template <typename T>
T *allocate_memory(int n = 1)
{
	return (T*)xmalloc(sizeof(T) * n);
}

template <typename T>
T &allocate_memory(T *&ptr)
{
	ptr = (T*)xmalloc(sizeof(T));
	return *ptr;
}

template <typename T>
void free_memory(T *ptr)
{
	if (ptr) xfree(ptr);
}

template <typename T>
int copy_memory(T *dst, const T *src, int n = 1)
{
	return xcopy(dst, src, sizeof(T) * n);
}

template <typename T>
void clear_memory(T *dst, int n = 1)
{
	xclear(dst, sizeof(T)*n);
}
