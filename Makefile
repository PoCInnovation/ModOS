CC		= x86_64-elf-gcc
ASMC	= x86_64-elf-as

CFLAGS		+= -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mno-red-zone -m64
LIBGCC_DIR	= /opt/x86_64-elf/lib/gcc/x86_64-elf/12.3.0
LDFLAGS		+= -L$(LIBGCC_DIR) -lgcc

SRC			= src/kernel.c
OBJ			= $(SRC:.c=.o)
BOOT		= src/boot.asm
BOOT_BIN	= src/boot.o

KERNEL_BIN	= kernel.bin
GRUB		= grub.cfg
LINKER_FILE	= linker.ld
NAME		= modos.iso

BUILD_DIR	= build
BOOT_DIR	= $(BUILD_DIR)/boot
GRUB_DIR	= $(BOOT_DIR)/grub

all: $(NAME)

$(NAME): $(OBJ)
	$(ASMC) $(BOOT) -o $(BOOT_BIN)
	$(CC) -o $(KERNEL_BIN) $(BOOT_BIN) $(OBJ) -T $(LINKER_FILE) -nostdlib $(LDFLAGS)

grub:
	grub-file --is-x86-multiboot2 $(KERNEL_BIN)

	mkdir -p $(BOOT_DIR) $(GRUB_DIR)
	cp $(KERNEL_BIN) $(BOOT_DIR)/
	cp $(GRUB) $(GRUB_DIR)/

	grub-mkrescue -o $(NAME) $(BUILD_DIR)

qemu:
	qemu-system-x86_64 -cdrom $(NAME) -vga std -serial mon:stdio

fclean:
	rm -rf $(OBJ) $(BOOT_BIN) $(KERNEL_BIN) $(NAME) $(BOOT_DIR)

re: fclean all
