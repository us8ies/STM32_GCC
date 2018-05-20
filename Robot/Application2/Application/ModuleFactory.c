#include "MemoryFunctions.h"
#include "ModuleFactory.h"

#define STATE (this->State)

typedef struct ModuleFactoryState 
{
	uint32_t dummy_property; //TODO: remove it
	IMemoryFunctions_t * MemoryFunctions;
} ModuleFactoryState_t;

static void _dispose(IModuleFactory_t *this)
{
	IMemoryFunctions_t *memory_functions = STATE->MemoryFunctions;

	memory_functions->Free(STATE);
	memory_functions->Free(this);
}

IModuleFactory_t* ModuleFactory_Create(IMemoryFunctions_t * MemoryFunctions)
{
	IModuleFactory_t* result = (IModuleFactory_t*)MemoryFunctions->Allocate(sizeof(IModuleFactory_t));
	result->State = (ModuleFactoryState_t*)MemoryFunctions->Allocate(sizeof(ModuleFactoryState_t));
	result->DisposeModule = _dispose;
	result->State->MemoryFunctions = MemoryFunctions;

	return result;
}
