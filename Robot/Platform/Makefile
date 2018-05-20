BUILDDIR = build

SOURCES = \
			STM32L152RBTx_MDK/Src/stm32l1xx_hal_msp.c \
			STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_rcc_ex.c \
			STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_flash_ex.c \
			STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_flash_ramfunc.c \
			STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_flash.c \
			STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_rcc.c \
			STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_cortex.c \
			STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal.c \
			STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_gpio.c \
			STM32L152RBTx_MDK/Src/main.c \
			STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_uart.c \
			STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_pwr_ex.c \
			STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_pwr.c \
			STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_dma.c \
			STM32L152RBTx_MDK/Src/stm32l1xx_it.c \
			STM32L152RBTx_MDK/Src/system_stm32l1xx.c \
			../../STM32L152RBTx_GCC/startup.s \

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

INCLUDES = \
			-I../Interfaces \
			-ISTM32L152RBTx_MDK/Inc \
			-ISTM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Inc \
			-ISTM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Inc/Legacy \
			-ISTM32L152RBTx_MDK/Drivers/CMSIS/Device/ST/STM32L1xx/Include \
			-ISTM32L152RBTx_MDK/Drivers/CMSIS/Include \

LIB = $(BUILDDIR)/lib_platform.a

CC = ../../STM32L152RBTx_GCC/compiler/bin/arm-none-eabi-gcc
AR = ../../STM32L152RBTx_GCC/compiler/bin/arm-none-eabi-ar

CFLAGS  = -O0 -g -Wall -I.\
		-mcpu=cortex-m3 -mthumb \
		$(INCLUDES) -DUSE_HAL_DRIVER -DSTM32L152xB -D__MICROLIB -D_RTE_

to_output_path = $(subst ../,up/,$(1))
define run_compile 
	mkdir -p $(dir $(call to_output_path,$@))	
	$(CC) -c $(CFLAGS) $< -o $(call to_output_path,$@)
endef

all: clean $(LIB)

$(LIB): $(OBJECTS)
	$(AR) -crs $@ $(call to_output_path,$(OBJECTS))

$(BUILDDIR)/%.o: %.c
	$(run_compile)

$(BUILDDIR)/%.o: %.s
	$(run_compile)

clean:
	rm -rf build