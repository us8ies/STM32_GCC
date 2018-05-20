#include "CommandDistributor.h"
#include "MemoryFunctions.h"
#include "List.h"
#include "ModuleFactory.h"

typedef struct CommandDistributorState 
{
	IMemoryFunctions_t* memoryFunctions;
	IList_t* storage;
} CommandDistributorState_t;

typedef struct
{
	uint32_t ReceiverType;
	uint32_t CommandType;
	void (*CommandExecutor)(void);
} Subscriber_t;

static SubscriptionResult_t _subscribe(ICommandDistributor_t* this, uint32_t receiverType, uint32_t commandType, void* commandExecutor)
{
	if (false == this->State->storage->IsPresent(this->State->storage, commandExecutor))
	{
		Subscriber_t* subscriber = (Subscriber_t*)this->State->memoryFunctions->Allocate(sizeof(Subscriber_t));
		subscriber->ReceiverType = receiverType;
		subscriber->CommandType = commandType;
		subscriber->CommandExecutor = commandExecutor;

		this->State->storage->Add(this->State->storage, subscriber);
		return SuccessfullySubscribed;
	}

	return AlreadySubscribed;
}

static CommandDistributionResult_t _distribute(ICommandDistributor_t* this, uint32_t receiverType, uint32_t commandType)
{
	CommandDistributionResult_t result = MissingSubscriber;

	uint32_t numberOfSubscribers = this->State->storage->Count(this->State->storage);

	for(uint32_t i = 0;i < numberOfSubscribers; i++)
	{
		Subscriber_t* subscriber = this->State->storage->GetById(this->State->storage, i);

		if (subscriber->CommandType != commandType || subscriber->ReceiverType != receiverType)
			continue;

		subscriber->CommandExecutor();

		result = Success;
	}

	return result;
}

static void _disposeSubscribers(ICommandDistributor_t* this)
{
	uint32_t numberOfSubscribers = this->State->storage->Count(this->State->storage);

	for (uint32_t i = 0; i < numberOfSubscribers; i++)
	{
		Subscriber_t* subscriber = this->State->storage->GetById(this->State->storage, i);
		this->State->memoryFunctions->Free(subscriber);
	}
}

static void _dispose(ICommandDistributor_t* this)
{
	_disposeSubscribers(this);

	IMemoryFunctions_t* memoryFunctions = this->State->memoryFunctions;

	memoryFunctions->Free(this->State->storage);
	memoryFunctions->Free(this->State);
	memoryFunctions->Free(this);
}

ICommandDistributor_t* CommandDistributor_Create(IModuleFactory_t* moduleFactory, IMemoryFunctions_t* memoryFunctions)
{
	ICommandDistributor_t* commandDistributor = (ICommandDistributor_t*)memoryFunctions->Allocate(sizeof(ICommandDistributor_t));
	commandDistributor->Subscribe = _subscribe;
	commandDistributor->Dispose = _dispose;
	commandDistributor->Distribute = _distribute;

	IList_t* storage = moduleFactory->CreateList(moduleFactory);
	CommandDistributorState_t* state = (CommandDistributorState_t*)memoryFunctions->Allocate(sizeof(CommandDistributorState_t));
	
	commandDistributor->State = state;
	commandDistributor->State->storage = storage;
	commandDistributor->State->memoryFunctions = memoryFunctions;

	return commandDistributor;
}
