BUILDDIR = build

#ARCHIVES =  ../Platform/build/lib_platform.a ../Application2/build/lib_unit_tests.a
ARCHIVES =  ../Platform/build/lib_platform.a ../Application1/build/lib_appliction1.a

ELF = $(BUILDDIR)/program.elf
HEX = $(BUILDDIR)/program.hex
BIN = $(BUILDDIR)/program.bin

rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
OBJECTS = $(call rwildcard,$(BUILDDIR)/,*.o)

LD      = ../../STM32L152RBTx_GCC/compiler/bin/arm-none-eabi-gcc
AR      = ../../STM32L152RBTx_GCC/compiler/bin/arm-none-eabi-ar
OBJCOPY = ../../STM32L152RBTx_GCC/compiler/bin/arm-none-eabi-objcopy
SZ      = ../../STM32L152RBTx_GCC/compiler/bin/arm-none-eabi-size

LDSCRIPT = linker_script.ld
LDFLAGS += -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3 -specs=nano.specs -specs=nosys.specs

to_output_path = $(BUILDDIR)/$(subst ../,up/,$(1))

all: clean $(HEX)

$(HEX): $(BIN)
	$(OBJCOPY) -O ihex $(ELF) $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(ELF): $(ARCHIVES)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)
	$(SZ) --format=berkeley $@

%.a: FORCE
	mkdir -p $(dir $(call to_output_path,$@))
	$(AR) x $@
	mv '*.o' $(dir $(call to_output_path,$@))

FORCE:
	
clean:
	rm -rf build