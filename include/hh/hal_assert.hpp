/** @file hal_assert.hpp
*   @brief Created on 2021-08-28 by Ben
*/
#pragma once
#include <assert.h>

#ifdef NDEBUG
    #define assert(x) ((void)0)
#else
    #define assert(x) while((x));
#endif