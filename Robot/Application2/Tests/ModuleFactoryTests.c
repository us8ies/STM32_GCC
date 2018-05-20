#include "../Application/ModuleFactory.h"

#include "Unity/unity_fixture.h"

#include "MemoryFunctionsMock.h"
#include "fff/fff.h"
#include <stdbool.h>
DEFINE_FFF_GLOBALS;

#define CALL(object, func) object->func(object)

TEST_GROUP(ModuleFactoryTests);

IMemoryFunctions_t* memoryFunctionsMock;
IModuleFactory_t* moduleFactory;

static void CreateMemoryFunctionsMock(void)
{
	memoryFunctionsMock = (IMemoryFunctions_t*)calloc(1, sizeof(IMemoryFunctions_t));
	memoryFunctionsMock->Allocate = Allocate;
	memoryFunctionsMock->Free = Free;
}

uint32_t * ModuleFactoryTestsAllocate(uint32_t size)
{
	return calloc(1, size);
}

static void PrepareFakes(void)
{
	CreateMemoryFunctionsMock();

	RESET_FAKE(Free);
	RESET_FAKE(Allocate);

	Free_fake.custom_fake = free;
	Allocate_fake.custom_fake = ModuleFactoryTestsAllocate;
}

static bool ModuleIsDisposed(void)
{
	for (uint32_t i = 0; i < Free_fake.arg_history_len; i++)
	{
		if (moduleFactory == Free_fake.arg0_history[i])
			return true;
	}

	return false;
}


TEST_SETUP(ModuleFactoryTests)
{
	PrepareFakes();

	moduleFactory = ModuleFactory_Create(memoryFunctionsMock);
}

TEST_TEAR_DOWN(ModuleFactoryTests)
{
	if(false == ModuleIsDisposed())
		moduleFactory->DisposeModule(moduleFactory);

	free(memoryFunctionsMock);
}

TEST(ModuleFactoryTests, Constructor_CreatesModuleAndSetsState)
{
	TEST_ASSERT_NOT_NULL(moduleFactory);
	TEST_ASSERT_NOT_NULL(moduleFactory->State);
	TEST_ASSERT_EQUAL(2, Allocate_fake.call_count);
}

TEST(ModuleFactoryTests, Dispose_DisposesModuleAndItsState)
{
	void* expectedState = moduleFactory->State;
	void* expectedModule = moduleFactory;

	moduleFactory->DisposeModule(moduleFactory);

	TEST_ASSERT_EQUAL(2, Free_fake.call_count);
	TEST_ASSERT_EQUAL(expectedState, Free_fake.arg0_history[0]);
	TEST_ASSERT_EQUAL(expectedModule, Free_fake.arg0_history[1]);
}

TEST(ModuleFactoryTests, CreateList_CreatesList)
{
	TEST_IGNORE_MESSAGE("not implemented yet");
}

TEST_GROUP_RUNNER(ModuleFactoryTests)
{
	RUN_TEST_CASE(ModuleFactoryTests, Constructor_CreatesModuleAndSetsState);
	RUN_TEST_CASE(ModuleFactoryTests, Dispose_DisposesModuleAndItsState);
	RUN_TEST_CASE(ModuleFactoryTests, CreateList_CreatesList);
}
