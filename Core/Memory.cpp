#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "Core/Memory.h"
#include "Core/Utils.h"

void *xmalloc(int n)
{
	void *mem = malloc(n);
	if (!mem)
		die("nextgame: out of memory");
	return mem;
}

void xfree(void *ptr)
{
	free(ptr);
}

int xcopy(void *dst, const void *src, int n)
{
	memmove(dst, src, n);
	return n;
}

void xclear(void *dst, int n)
{
	memset(dst, 0, n);
}

void *operator new(size_t size, const OrDie_t &)
{
	void *mem = operator new(size, std::nothrow);
	if (!mem)
		die("nextgame: out of memory");
	return mem;
}

void *operator new[](size_t size, const OrDie_t &)
{
	void *mem = operator new[](size, std::nothrow);
	if (!mem)
		die("nextgame: out of memory");
	return mem;
}


