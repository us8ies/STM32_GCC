#pragma once

#include "stdbool.h"
#include "stdint.h"
#include "MemoryFunctions.h"

typedef struct ListState ListState_t;
typedef struct IList IList_t;

typedef struct IList
{
	void(*Add)(IList_t*, void*);
	bool(*IsPresent)(IList_t*, void*);
	uint32_t(*Count)(IList_t*);
	void*(*GetById)(IList_t*, uint32_t);
	void(*RemoveById)(IList_t*, uint32_t);

	void(*Dispose)(IList_t*);
	
	ListState_t* State;
}IList_t;

IList_t* List2_Create(IMemoryFunctions_t*);