/// \file hal_assert.hpp
/// \brief Created on 2021-08-28 by Ben

#pragma once

#if defined(WIN32) || defined(__linux__) || defined(__APPLE__)
#include <cassert>
#elif defined(NDEBUG)
#define assert(x) ((void)0)
#else
#define assert(x) while((x));
#endif
