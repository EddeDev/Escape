#pragma once

#include "Escape/PrimitiveTypes.h"
#include "Escape/RefCounting.h"

#define ESCAPE_BIND_CALLBACK(func, ...) \
[__VA_ARGS__](auto&&... args) -> decltype(auto) \
{ \
	return func(std::forward<decltype(args)>(args)...); \
}