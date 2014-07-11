CC=i686-elf-gcc
AS=i686-elf-as
LD=i686-elf-ld

OUTPUT = kernel

OBJECTS = $(patsubst %.c, %.o, $(wildcard kernel/*.c))
OBJECTS += $(patsubst %.c, %.o, $(wildcard kernel/*/*.c))

VID_SRC = $(patsubst %.c, %.o, $(wildcard kernel/video/*.c))
CPU_SRC = $(patsubst %.c, %.o, $(wildcard kernel/cpu/*.c))
DEV_SRC = $(patsubst %.c, %.o, $(wildcard kernel/devices/*.c))
MEM_SRC = $(patsubst %.c, %.o, $(wildcard kernel/mem/*.c))


CFLAGS = -I./kernel/include -std=gnu99 -ffreestanding -O2 -Wall -Wextra 
LDFLAGS = -ffreestanding -o2 -nostdlib

all: 	loader spex-kern cpu video devices memory
		@$(CC) -T kernel/linker.ld $(LDFLAGS) -o spex-kernel $(OBJECTS) kernel/boot.o -lgcc

spex-kern: kernel/main.c
		@$(CC) $(CFLAGS) -o kernel/main.o -c kernel/main.c
		@$(CC) $(CFLAGS) -o kernel/system.o -c kernel/system.c
		
loader: kernel/boot.s
		nasm -felf -o kernel/boot.o kernel/boot.s
		
video:	$(VID_SRC)

cpu:	$(CPU_SRC)
#		@$(CC) $(CFLAGS) -c -o  $@ $^

devices:	$(DEV_SRC)

memory:		$(MEM_SRC)
		
clean:	
		@rm -f kernel/*.o
		@rm -f kernel/*/*.o

