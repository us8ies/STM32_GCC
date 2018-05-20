#include "List.h"

typedef struct ListState
{
	IMemoryFunctions_t* memoryFunctions;
	uint32_t count;
	void** items;

}ListState_t;

static void _add(IList_t* this, void* ptr)
{
	if (this->State->count == 0)
	{
		this->State->items = (void**)this->State->memoryFunctions->Allocate(sizeof(void**));
	}
	else
	{
		this->State->items = (void**)this->State->memoryFunctions->Reallocate(this->State->items, sizeof(void**) * (this->State->count + 1));
	}

	this->State->items[this->State->count] = ptr;
	this->State->count++;
}

static bool _isPresent(IList_t* this, void* ptr)
{
	for (uint32_t i = 0; i < this->Count(this); i++)
	{
		void* item = this->GetById(this, i);

		if (item == ptr)
			return true;
	}

	return false;
}

static uint32_t _count(IList_t* this)
{
	return this->State->count;
}

static void* _getById(IList_t* this, uint32_t id)
{
	return this->State->items[id];
}

static void _removeById(IList_t* this, uint32_t id)
{






}

static void _dispose(IList_t* this)
{
	IMemoryFunctions_t* memoryFunctions = this->State->memoryFunctions;

	memoryFunctions->Free(this->State->items);
	memoryFunctions->Free(this->State);
	memoryFunctions->Free(this);
}

IList_t* List2_Create(IMemoryFunctions_t* memoryFunctions)
{
	IList_t* result = (IList_t*)memoryFunctions->Allocate(sizeof(IList_t));
	result->Add = _add;
	result->Count = _count;
	result->GetById = _getById;
	result->RemoveById = _removeById;
	result->IsPresent = _isPresent;
	result->Dispose = _dispose;

	ListState_t* state = (ListState_t*)memoryFunctions->Allocate(sizeof(ListState_t));

	result->State = state;
	result->State->memoryFunctions = memoryFunctions;
	result->State->count = 0;

	return result;

}