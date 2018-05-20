#pragma once

#include "../Application/BasicTypes.h"

#include "fff/fff.h"

DECLARE_FAKE_VOID_FUNC1(Free, void*);
DECLARE_FAKE_VALUE_FUNC1(uint32_t*, Allocate, uint32_t);
DECLARE_FAKE_VALUE_FUNC2(uint32_t*, Reallocate, void*, uint32_t);
