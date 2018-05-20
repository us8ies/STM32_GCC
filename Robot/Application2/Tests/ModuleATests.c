#include "../Application/ModuleA.h"

#include "Unity/unity_fixture.h"
#include <stdbool.h>
#include "MemoryFunctionsMock.h"
#include "fff/fff.h"
DEFINE_FFF_GLOBALS;

#define CALL(object, func) object->func(object)

TEST_GROUP(ModuleATests);

IModuleA_t* moduleA_1;
IModuleA_t* moduleA_2;
IMemoryFunctions_t* memoryFunctionsMock;

uint32_t * MyAllocate(uint32_t size)
{
	return calloc(1, size);
}

static void CreateMemoryFunctionsMock(void)
{
	memoryFunctionsMock = (IMemoryFunctions_t*)calloc(1, sizeof(IMemoryFunctions_t));
	memoryFunctionsMock->Allocate = Allocate;
	memoryFunctionsMock->Free = Free;
}

static void PrepareFakes(void)
{
	CreateMemoryFunctionsMock();

	RESET_FAKE(Free);
	RESET_FAKE(Allocate);

	Free_fake.custom_fake = free;
	Allocate_fake.custom_fake = MyAllocate;
}

static void PrepareModuleA(void)
{
	moduleA_1 = ModuleA_Create(memoryFunctionsMock);
	moduleA_2 = ModuleA_Create(memoryFunctionsMock);
}

static bool ModuleAIsDisposed(void)
{
	for (uint32_t i = 0; i < Free_fake.arg_history_len; i++)
	{
		if (moduleA_1 == Free_fake.arg0_history[i])
			return true;
	}

	return false;
}

TEST_SETUP(ModuleATests)
{
	PrepareFakes();

	PrepareModuleA();
}

TEST_TEAR_DOWN(ModuleATests)
{
	if(!ModuleAIsDisposed())
		moduleA_1->Dispose(moduleA_1);

	moduleA_2->Dispose(moduleA_2);

	free(memoryFunctionsMock);
}

TEST(ModuleATests, Foo_Returns5)
{
	TEST_ASSERT_EQUAL(5, moduleA_1->Foo());
}

TEST(ModuleATests, GetCounter_IncrementsAfterEachCall)
{
	TEST_ASSERT_EQUAL(1, moduleA_1->GetCounter(moduleA_1));
	TEST_ASSERT_EQUAL(2, moduleA_1->GetCounter(moduleA_1));
}

TEST(ModuleATests, Constructor_SetCounterToOne)
{
	TEST_ASSERT_EQUAL(1, CALL(moduleA_1, GetCounter));
}

TEST(ModuleATests, Constructor_SetsStateNotNull)
{
	TEST_ASSERT_NOT_EQUAL(NULL, moduleA_1->State);
	TEST_ASSERT_EQUAL(4, Allocate_fake.call_count); //checking that mocks are cleared
	TEST_ASSERT_EQUAL_PTR(moduleA_1->State, Allocate_fake.return_val_history[1]);
}

TEST(ModuleATests, Dispose_CallsFreeOnMemoryFunctions)
{
	void* expected[] = { moduleA_1->State, moduleA_1 };

	moduleA_1->Dispose(moduleA_1);
	
	TEST_ASSERT_EQUAL(2, Free_fake.call_count);
	TEST_ASSERT_EQUAL_PTR_ARRAY(expected, Free_fake.arg0_history, 2);
}

TEST(ModuleATests, SetCounter_SetoresNewValue)
{
	moduleA_1->SetCounter(moduleA_1, 99);

	TEST_ASSERT_EQUAL(99, moduleA_1->GetCounter(moduleA_1));
}

TEST(ModuleATests, GetCounter_MultipleInstances_ReturnsDifferentValues)
{
	moduleA_1->SetCounter(moduleA_1, 11);
	moduleA_2->SetCounter(moduleA_2, 22);

	TEST_ASSERT_EQUAL(11, moduleA_1->GetCounter(moduleA_1));
	TEST_ASSERT_EQUAL(22, moduleA_2->GetCounter(moduleA_2));
}

TEST_GROUP_RUNNER(ModuleATests)
{
	RUN_TEST_CASE(ModuleATests, Foo_Returns5);
	
	RUN_TEST_CASE(ModuleATests, GetCounter_IncrementsAfterEachCall);
	RUN_TEST_CASE(ModuleATests, SetCounter_SetoresNewValue);

	RUN_TEST_CASE(ModuleATests, Constructor_SetCounterToOne);
	RUN_TEST_CASE(ModuleATests, Constructor_SetsStateNotNull);
	RUN_TEST_CASE(ModuleATests, Dispose_CallsFreeOnMemoryFunctions);

	RUN_TEST_CASE(ModuleATests, GetCounter_MultipleInstances_ReturnsDifferentValues);
}
