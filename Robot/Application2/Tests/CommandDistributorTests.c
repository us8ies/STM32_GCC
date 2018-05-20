#include "../Application/CommandDistributor.h"

#include "Unity/unity_fixture.h"
#include "stdbool.h"


#include "fff/fff.h"
#include "../Application/List.h"
#include "MemoryFunctionsMock.h"

DEFINE_FFF_GLOBALS;

#define CALL(object, func) object->func(object)
#define MSVC_POINTER_TO_STACK 0xCCCCCCCC

FAKE_VOID_FUNC2(StorageAdd, IList_t*, void*);
FAKE_VALUE_FUNC2(bool, StorageIsPresent, IList_t*, void*);
FAKE_VALUE_FUNC1(IList_t*, CreateList, IModuleFactory_t*);
FAKE_VALUE_FUNC1(uint32_t, StorageCount, IList_t*);
FAKE_VALUE_FUNC2(void*, StorageGetById, IList_t*, uint32_t);
FAKE_VALUE_FUNC(CommandDistributionResult_t, Subscriber1);
FAKE_VALUE_FUNC(CommandDistributionResult_t, Subscriber2);

void* CustomGetById(IList_t* list, uint32_t id)
{
	return StorageAdd_fake.arg1_history[id];
}

uint32_t CustomCount(IList_t* list)
{
	return StorageAdd_fake.call_count;
}


static uint32_t* CustomAllocate(uint32_t size)
{
	return (uint32_t*)calloc(1, size);
}

static void CustomFree(void* ptr)
{
	free(ptr);
}

static bool isPtrValid(void* ptr)
{
	return (uint32_t)ptr != MSVC_POINTER_TO_STACK && ptr != NULL;
}


TEST_GROUP(CommandDistributorTests);

static bool _uutDisposed = false;
static ICommandDistributor_t* uut;
static IList_t* storageMock;
static IModuleFactory_t* moduleFactoryMock;
static IMemoryFunctions_t* memoryFunctionsMock;


TEST_SETUP(CommandDistributorTests)
{
	_uutDisposed = false;

	RESET_FAKE(Free);
	RESET_FAKE(Allocate);
	RESET_FAKE(CreateList);
	RESET_FAKE(StorageAdd);
	RESET_FAKE(StorageIsPresent);
	RESET_FAKE(StorageCount);
	RESET_FAKE(StorageGetById);
	RESET_FAKE(Subscriber1);
	RESET_FAKE(Subscriber2);

	Allocate_fake.custom_fake = CustomAllocate;
	Free_fake.custom_fake = CustomFree;

	moduleFactoryMock = calloc(1, sizeof(IModuleFactory_t));
	moduleFactoryMock->CreateList = CreateList;

	memoryFunctionsMock = calloc(1, sizeof(IMemoryFunctions_t));
	memoryFunctionsMock->Allocate = Allocate;
	memoryFunctionsMock->Free = Free;

	storageMock = calloc(1, sizeof(IList_t));
	storageMock->Add = StorageAdd;
	storageMock->IsPresent = StorageIsPresent;
	storageMock->Count = StorageCount;
	storageMock->GetById = StorageGetById;

	CreateList_fake.return_val = storageMock;
	StorageCount_fake.custom_fake = CustomCount;
	StorageGetById_fake.custom_fake = CustomGetById;

	uut = CommandDistributor_Create(moduleFactoryMock, memoryFunctionsMock);
}

TEST_TEAR_DOWN(CommandDistributorTests)
{
	if (false == _uutDisposed)
	{
		uut->Dispose(uut);
	}

	free(moduleFactoryMock);
	free(memoryFunctionsMock);
}

TEST(CommandDistributorTests, Constructor_AssignsSubscribeFunction)
{
	TEST_ASSERT_TRUE(isPtrValid(uut->Subscribe));
}

TEST(CommandDistributorTests, Dispose_DisposesStateAndStorageAndItself)
{
	CommandDistributorState_t* expectedState = uut->State;

	uut->Dispose(uut);

	_uutDisposed = true;

	TEST_ASSERT_EQUAL(3, Free_fake.call_count);
	TEST_ASSERT_EQUAL(storageMock, Free_fake.arg0_history[0]);
	TEST_ASSERT_EQUAL(expectedState, Free_fake.arg0_history[1]);
	TEST_ASSERT_EQUAL(uut, Free_fake.arg0_history[2]);
}


TEST(CommandDistributorTests, Subscribe_StoresSubscriberInternally)
{
	void* expectedSubscriber = (void*)MSVC_POINTER_TO_STACK;

	StorageIsPresent_fake.return_val = false;

	SubscriptionResult_t result = uut->Subscribe(uut, 1, 2, expectedSubscriber);

	TEST_ASSERT_EQUAL(1, StorageAdd_fake.call_count);
	TEST_ASSERT_EQUAL(SuccessfullySubscribed, result);
}

TEST(CommandDistributorTests, Subscribe_SamePointerSecondTime_ReturnsAlreadySubscribed)
{
	void* expectedSubscriber = (void*)MSVC_POINTER_TO_STACK;

	StorageIsPresent_fake.return_val = true;

	SubscriptionResult_t result = uut->Subscribe(uut, 1, 2, expectedSubscriber);

	TEST_ASSERT_EQUAL(AlreadySubscribed, result);
	TEST_ASSERT_EQUAL(0, StorageAdd_fake.call_count);
}

TEST(CommandDistributorTests, Distribute_CallsCorrespondingHandler)
{
	uut->Subscribe(uut, 1, 2, Subscriber1);
	uut->Subscribe(uut, 2, 3, Subscriber2);

	uut->Distribute(uut, 2, 3);
	TEST_ASSERT_EQUAL(0, Subscriber1_fake.call_count);
	TEST_ASSERT_EQUAL(1, Subscriber2_fake.call_count);

	uut->Distribute(uut, 1, 2);
	TEST_ASSERT_EQUAL(1, Subscriber1_fake.call_count);
	TEST_ASSERT_EQUAL(1, Subscriber2_fake.call_count);
}

TEST(CommandDistributorTests, Distribute_WhenNoSubscriberIsRegistered_ReturnsMissingSubscriber)
{
	uut->Subscribe(uut, 1, 2, Subscriber1);

	CommandDistributionResult_t result = uut->Distribute(uut, 2, 3);
	TEST_ASSERT_EQUAL(MissingSubscriber, result);
	TEST_ASSERT_EQUAL(0, Subscriber1_fake.call_count);
}

TEST_GROUP_RUNNER(CommandDistributorTests)
{
	RUN_TEST_CASE(CommandDistributorTests, Constructor_AssignsSubscribeFunction);
	RUN_TEST_CASE(CommandDistributorTests, Dispose_DisposesStateAndStorageAndItself);

	RUN_TEST_CASE(CommandDistributorTests, Subscribe_StoresSubscriberInternally);
	RUN_TEST_CASE(CommandDistributorTests, Subscribe_SamePointerSecondTime_ReturnsAlreadySubscribed);
	RUN_TEST_CASE(CommandDistributorTests, Distribute_CallsCorrespondingHandler);
	RUN_TEST_CASE(CommandDistributorTests, Distribute_WhenNoSubscriberIsRegistered_ReturnsMissingSubscriber);
}
