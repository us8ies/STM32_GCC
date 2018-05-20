#include "MemoryFunctionsMock.h"

DEFINE_FAKE_VOID_FUNC1(Free, void*);
DEFINE_FAKE_VALUE_FUNC1(uint32_t*, Allocate, uint32_t);
DEFINE_FAKE_VALUE_FUNC2(uint32_t*, Reallocate, void*, uint32_t);