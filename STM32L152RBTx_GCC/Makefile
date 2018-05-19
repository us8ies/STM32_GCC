BUILDDIR = build

SOURCES = \
			../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_cortex.c \
			../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_rcc.c \
			../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_flash.c \
			../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_flash_ramfunc.c \
			../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_flash_ex.c \
			../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_rcc_ex.c \
			../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_uart.c \
			../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_pwr_ex.c \
			../STM32L152RBTx_MDK/Src/stm32l1xx_hal_msp.c \
			../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal.c \
			../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_gpio.c \
			../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_pwr.c \
			../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_dma.c \
			../STM32L152RBTx_MDK/Src/stm32l1xx_it.c \
			../STM32L152RBTx_MDK/Src/system_stm32l1xx.c \
			../STM32L152RBTx_MDK/Src/main.c \
			startup.s

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

INCLUDES = -I../STM32L152RBTx_MDK/Inc \
			-I../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Inc \
			-I../STM32L152RBTx_MDK/Drivers/STM32L1xx_HAL_Driver/Inc/Legacy \
			-I../STM32L152RBTx_MDK/Drivers/CMSIS/Device/ST/STM32L1xx/Include \
			-I../STM32L152RBTx_MDK/Drivers/CMSIS/Include \

ELF = $(BUILDDIR)/program.elf
HEX = $(BUILDDIR)/program.hex
BIN = $(BUILDDIR)/program.bin

CC = compiler/bin/arm-none-eabi-gcc
LD = compiler/bin/arm-none-eabi-gcc
AR = compiler/bin/arm-none-eabi-ar
SZ = compiler/bin/arm-none-eabi-size
OBJCOPY = compiler/bin/arm-none-eabi-objcopy

CFLAGS  = -O0 -g -Wall -I.\
		-mcpu=cortex-m3 -mthumb \
		$(INCLUDES) -DUSE_HAL_DRIVER -DSTM32L152xB -D__MICROLIB -D_RTE_

LDSCRIPT = linker_script.ld
LDFLAGS += -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3 -specs=nano.specs -specs=nosys.specs

to_output_path = $(subst ../,up/,$(1))
define run_compile 
	mkdir -p $(dir $(call to_output_path,$@))	
	$(CC) -c $(CFLAGS) $< -o $(call to_output_path,$@)
endef

all: clean $(HEX)

$(HEX): $(ELF) $(BIN)
	$(OBJCOPY) -O ihex $< $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(call to_output_path,$(OBJECTS))
	$(SZ) --format=berkeley $@

$(BUILDDIR)/%.o: %.c
	$(run_compile)

$(BUILDDIR)/%.o: %.s
	$(run_compile)

flash: $(BIN)
	st-flash write $(BIN) 0x8000000

clean:
	rm -rf build

PHONY: clean flash tgt build