#pragma once

#include <functional>

namespace esc {

	struct AsyncResult
	{
	};

	using AsyncCallbackFn = std::function<void(const AsyncResult& result)>;

}