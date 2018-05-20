#pragma once
#include <stdint.h>
#include "MemoryFunctions.h"
#include "ModuleFactory.h"

typedef struct ICommandDistributor ICommandDistributor_t;
typedef struct CommandDistributorState CommandDistributorState_t;

typedef enum
{
	SuccessfullySubscribed,
	AlreadySubscribed
}SubscriptionResult_t;

typedef enum
{
	Success,
	MissingSubscriber
}CommandDistributionResult_t;

typedef struct ICommandDistributor
{
	SubscriptionResult_t(*Subscribe)(ICommandDistributor_t* this, uint32_t receiverType, uint32_t commandType, void(*handler)(void));

	void(*Dispose)(ICommandDistributor_t*);

	CommandDistributionResult_t (*Distribute)(ICommandDistributor_t*, uint32_t, uint32_t);

	CommandDistributorState_t * State;
}ICommandDistributor_t;

ICommandDistributor_t* CommandDistributor_Create(IModuleFactory_t * moduleFactory, IMemoryFunctions_t * memoryFunctions);