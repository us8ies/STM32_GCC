#pragma once

#include "BasicTypes.h"

typedef struct MemoryFunctionsInterface
{
	uint32_t*(*Allocate)(uint32_t);
	uint32_t*(*Reallocate)(void*, uint32_t);
	void(*Free)(void*);

}IMemoryFunctions_t;

IMemoryFunctions_t* MemoryFunctions_Create();

