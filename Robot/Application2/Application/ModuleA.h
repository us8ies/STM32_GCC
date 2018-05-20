#pragma once

#include "BasicTypes.h"
#include "MemoryFunctions.h"

typedef struct IModuleA IModuleA_t;
typedef struct ModuleAState ModuleAState_t;

typedef struct IModuleA
{
	uint32_t(*Foo)(void);
	uint32_t(*GetCounter)(IModuleA_t*);
	void(*Dispose)(IModuleA_t*);
	void(*SetCounter)(IModuleA_t*, uint32_t);

	ModuleAState_t * State;
}IModuleA_t;

IModuleA_t* ModuleA_Create(IMemoryFunctions_t * MemoryFunctions);
