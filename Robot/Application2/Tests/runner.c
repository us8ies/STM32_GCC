#include "Unity/unity_fixture.h"
#include <PlatformInterface.h>

void runAllTests(void)
{
	RUN_TEST_GROUP(ModuleATests);
	RUN_TEST_GROUP(ModuleFactoryTests);
	RUN_TEST_GROUP(CommandDistributorTests);
	RUN_TEST_GROUP(List2Tests);
}

void main2(int argc, const char **argv)
{
	char *unityArguments[2] = { "", "-v" };

	UnityMain(2, unityArguments, runAllTests);
}

void RunApplication(void)
{
	main2(0, 0);
	
	uint16_t counter = 0u;
	while(1)
	{
		Delay(1000);
		char message[20] = {0};
		
		sprintf(message, "%d\r", counter++);
		
		Send((uint8_t *)message, 20);
	}
}

int _write(int file, char *ptr, int len)
{
	Send((uint8_t*)ptr, len);
    return len;
}
