#include "../Application/List.h"

#include "Unity/unity_fixture.h"


#include "fff/fff.h"
#include "MemoryFunctionsMock.h"

DEFINE_FFF_GLOBALS;

TEST_GROUP(List2Tests);

static IList_t* uut;

IMemoryFunctions_t memoryFunctionsMock;

uint32_t* CustomAllocate(uint32_t size)
{
	return calloc(1, size);
}

uint32_t* CustomReallocate(void *ptr, uint32_t size)
{
	return realloc(ptr, size);
}

void CustomFree(void* ptr)
{
	free(ptr);
}

TEST_SETUP(List2Tests)
{
	RESET_FAKE(Allocate);
	Allocate_fake.custom_fake = CustomAllocate;

	RESET_FAKE(Reallocate);
	Reallocate_fake.custom_fake = CustomReallocate;

	RESET_FAKE(Free);
	Free_fake.custom_fake = CustomFree;

	memoryFunctionsMock.Allocate = Allocate;
	memoryFunctionsMock.Free = Free;
	memoryFunctionsMock.Reallocate = Reallocate;

	uut = List2_Create(&memoryFunctionsMock);
}

TEST_TEAR_DOWN(List2Tests)
{
	uut->Dispose(uut);
}

TEST(List2Tests, Constructor_AssignsAllFunctions)
{
	TEST_ASSERT_NOT_NULL(uut->Add);
	TEST_ASSERT_NOT_NULL(uut->Count);
	TEST_ASSERT_NOT_NULL(uut->GetById);
	TEST_ASSERT_NOT_NULL(uut->RemoveById);
	TEST_ASSERT_NOT_NULL(uut->IsPresent);
	TEST_ASSERT_NOT_NULL(uut->State);
	TEST_ASSERT_NOT_NULL(uut->Dispose);
}

TEST(List2Tests, Add_StoresItem)
{
	TEST_ASSERT_EQUAL(0, uut->Count(uut));
	
	uut->Add(uut, NULL);
	TEST_ASSERT_EQUAL(1, uut->Count(uut));

	uut->Add(uut, NULL);
	TEST_ASSERT_EQUAL(2, uut->Count(uut));
}

TEST(List2Tests, GetById_ReturnsCorrectItem)
{
	void* ptr1 = (void*)1111;
	void* ptr2 = (void*)2222;

	uut->Add(uut, ptr1);
	uut->Add(uut, ptr2);

	TEST_ASSERT_EQUAL(ptr2, uut->GetById(uut, 1));
	TEST_ASSERT_EQUAL(ptr1, uut->GetById(uut, 0));
}

TEST(List2Tests, IsPresent_ReturnsTrueIfItemIsPresent)
{
	void* ptr1 = (void*)1111;
	void* ptr2 = (void*)2222;
	void* ptr3 = (void*)3333;

	uut->Add(uut, ptr1);
	uut->Add(uut, ptr2);

	TEST_ASSERT_TRUE(uut->IsPresent(uut, ptr1));
	TEST_ASSERT_TRUE(uut->IsPresent(uut, ptr2));
	TEST_ASSERT_FALSE(uut->IsPresent(uut, ptr3));
}

TEST(List2Tests, RemoveById_RemovesRequestedItem)
{
	void* ptr1 = (void*)1111;
	void* ptr2 = (void*)2222;

	uut->Add(uut, ptr1);
	uut->Add(uut, ptr2);

	uut->RemoveById(uut, 1);

	TEST_ASSERT_TRUE(uut->IsPresent(uut, ptr1));
	TEST_ASSERT_FALSE(uut->IsPresent(uut, ptr2));
}

TEST_GROUP_RUNNER(List2Tests)
{
	RUN_TEST_CASE(List2Tests, Constructor_AssignsAllFunctions);
	RUN_TEST_CASE(List2Tests, Add_StoresItem);

	RUN_TEST_CASE(List2Tests, GetById_ReturnsCorrectItem);
	RUN_TEST_CASE(List2Tests, IsPresent_ReturnsTrueIfItemIsPresent);

	RUN_TEST_CASE(List2Tests, RemoveById_RemovesRequestedItem);
}
