# Project name
PROJ_NAME=stm32F4_usb_mp3

# Sources
SRCS = main.c stm32f4xx_it.c system_stm32f4xx.c syscalls.c

# FreeRTOS
FREERTOS_SRC = ./lib/FreeRTOS
FREERTOS_INC = $(FREERTOS_SRC)/include/  
FREERTOS_PORT_INC = $(FREERTOS_SRC)/portable/GCC/ARM_$(ARCH)/

# Audio
SRCS += Audio.c mp3.c

# USB
SRCS += usbh_usr.c usb_bsp.c

# UART
SRCS += serial.c

#======================================================================#
#Cross Compiler
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
SIZE=arm-none-eabi-size
ARCH=CM4F
#======================================================================#
#Flags
CFLAGS=-g -mlittle-endian -mthumb
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS+=-D USE_STDPERIPH_DRIVER
CFLAGS+=-I./


#stm32-flash
CFLAGS+=-Wl,-T,stm32_flash.ld





vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

# Includes
CFLAGS += -Iinc -Ilib/Core/cmsis -Ilib/Core/stm32
CFLAGS += -Ilib/Conf
CFLAGS+=-I$(FREERTOS_INC)
CFLAGS+=-I$(FREERTOS_PORT_INC)
CFLAGS+=-I./lib/Utilities/STM32F4-Discovery
CFLAGS+=-I./lib/common

# Library paths
LIBPATHS = -Llib/StdPeriph -Llib/USB_OTG
LIBPATHS += -Llib/USB_Host/Core -Llib/USB_Host/Class/MSC
LIBPATHS += -Llib/fat_fs
LIBPATHS += -Llib/helix

# Libraries to link
LIBS = -lm -lhelix -lfatfs -lstdperiph -lusbhostcore -lusbhostmsc -lusbcore

# Extra includes
CFLAGS += -Ilib/StdPeriph/inc
CFLAGS += -Ilib/USB_OTG/inc
CFLAGS += -Ilib/USB_Host/Core/inc
CFLAGS += -Ilib/USB_Host/Class/MSC/inc
CFLAGS += -Ilib/fat_fs/inc
CFLAGS += -Ilib/helix/pub

# add startup file to build
SRCS += lib/startup_stm32f4xx.s ./lib/Utilities/STM32F4-Discovery/stm32f4_discovery.c \
		$(FREERTOS_SRC)/tasks.c $(FREERTOS_SRC)/list.c $(FREERTOS_SRC)/portable/MemMang/heap_1.c \
		$(FREERTOS_SRC)/portable/GCC/ARM_$(ARCH)/port.c \
		$(FREERTOS_SRC)/croutine.c \
		$(FREERTOS_SRC)/queue.c  \
		$(FREERTOS_SRC)/timers.c  \
		./lib/common/stdlib.c \
		./lib/common/string.c

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj
	$(SIZE) $(PROJ_NAME).elf

lib:
	$(MAKE) -C lib FLOAT_TYPE=$(FLOAT_TYPE)

proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBPATHS) $(LIBS)
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

flash:
	st-flash write $(PROJ_NAME).bin 0x8000000

screen:
	echo sudo screen /dev/ttyUSB0 9600 

clean:
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	$(MAKE) clean -C lib # Remove this line if you don't want to clean the libs as well
	
