#pragma once
#include "MemoryFunctions.h"
#include "ModuleA.h"
#include "List.h"

typedef struct IModuleFactory IModuleFactory_t;
typedef struct ModuleFactoryState ModuleFactoryState_t;

typedef struct IModuleFactory
{
	IModuleA_t(*CreateModuleA)(IModuleFactory_t*);

	IList_t*(*CreateList)(IModuleFactory_t*);

	void(*DisposeModule)(IModuleFactory_t *);

	ModuleFactoryState_t * State;
}IModuleFactory_t;

IModuleFactory_t* ModuleFactory_Create(IMemoryFunctions_t * MemoryFunctions);





