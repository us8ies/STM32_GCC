#include "ModuleA.h"
#include "MemoryFunctions.h"

#define STATE (this->State)

typedef struct ModuleAState
{
	uint32_t Counter;
	IMemoryFunctions_t * MemoryFunctions;
	uint32_t Lock;

} ModuleAState_t;

static uint32_t _foo(void)
{
	return 5u;
}

static uint32_t _getCounter(IModuleA_t* this)
{
	return STATE->Counter++;
}

static void _setCounter(IModuleA_t* this, uint32_t newValue)
{
	STATE->Counter = newValue;
}

static void _dispose(IModuleA_t* this)
{
	IMemoryFunctions_t * memFunc = STATE->MemoryFunctions;

	memFunc->Free(STATE);
	STATE = NULL;

	memFunc->Free(this);
}

IModuleA_t* ModuleA_Create(IMemoryFunctions_t * memoryFunctions)
{
	IModuleA_t* this = (IModuleA_t*)memoryFunctions->Allocate(sizeof(IModuleA_t));
	
	STATE = (ModuleAState_t*)memoryFunctions->Allocate(sizeof(ModuleAState_t));
	STATE->MemoryFunctions = memoryFunctions;
	STATE->Counter = 1;

	this->Foo = _foo;
	this->GetCounter = _getCounter;
	this->Dispose = _dispose;
	this->SetCounter = _setCounter;

	return this;
}