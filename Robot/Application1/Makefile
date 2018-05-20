BUILDDIR = build

SOURCES = Application1.c 

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

INCLUDES = -I../Interfaces

LIB = $(BUILDDIR)/lib_appliction1.a

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