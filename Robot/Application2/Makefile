BUILDDIR = build

rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
SOURCES = $(call rwildcard,./,*.c)

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

INCLUDES =  -I../Interfaces \
			-IApplication \
			-ITests \
			-ITests/fff \
			-ITests/Unity \

LIB = $(BUILDDIR)/lib_unit_tests.a

CC = ../../STM32L152RBTx_GCC/compiler/bin/arm-none-eabi-gcc
AR = ../../STM32L152RBTx_GCC/compiler/bin/arm-none-eabi-ar

CFLAGS  = -O0 -g -Wall -I.\
		-mcpu=cortex-m3 -mthumb \
		$(INCLUDES) -DUSE_HAL_DRIVER -DSTM32L152xB -D__MICROLIB -D_RTE_ -DNULL=0

to_output_path = $(subst ../,up/,$(1))

all: clean $(LIB)

$(LIB): $(OBJECTS)
	$(AR) -crs $@ $(call to_output_path,$(OBJECTS))

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $(call to_output_path,$@))	
	$(CC) -c $(CFLAGS) $< -o $(call to_output_path,$@)

$(BUILDDIR)/%.o: %.s
	mkdir -p $(dir $(call to_output_path,$@))	
	$(CC) -x assembler-with-cpp -c $(CFLAGS) $< -o $(call to_output_path,$@)

clean:
	rm -rf build