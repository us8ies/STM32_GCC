#include <ApplicationInterface.h>
#include <PlatformInterface.h>

#include <stdint.h>
#include <stdio.h>

void RunApplication(void)
{
	uint16_t counter = 0u;
	while(1)
	{
		Delay(500u);
		
		char message[20] = {0};
		
		sprintf(message, "App12 %d\r\n", counter++);
		
		Send((uint8_t *)message, 20);
	}
}