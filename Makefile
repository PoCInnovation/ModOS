# Tools
CC = gcc
LD = ld
ASM = nasm

# Paths
SRC = boot/multiboot_header.asm boot/kernel_entry.asm kernel/kernel.c
BUILD_DIR = build

# Output
KERNEL = $(BUILD_DIR)/kernel.elf

all: $(KERNEL)

$(KERNEL): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(ASM) -f elf64 boot/multiboot_header.asm -o $(BUILD_DIR)/multiboot_header.o
	$(ASM) -f elf64 boot/kernel_entry.asm -o $(BUILD_DIR)/kernel_entry.o
	$(CC) -m64 -ffreestanding -c kernel/kernel.c -o $(BUILD_DIR)/kernel.o
	$(LD) -n -o $(KERNEL) -T kernel/linker.ld $(BUILD_DIR)/*.o

clean:
	rm -rf $(BUILD_DIR)

