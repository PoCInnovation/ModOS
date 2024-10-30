SRC	= 	src/kernel.c				\
		src/string.c				\
		src/pixel.c					\
		src/debug.c					\
		src/font.c					\

# objcopy -O elf32-i386 -B i386 -I binary assets/zap-ext-light32.psf assets/zap-ext-light32.o
FONTS = assets/zap-ext-light32.o

CFLAGS += -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CC	=	i686-elf-gcc
OBJ	=	$(SRC:.c=.o)
KERNEL_BIN = kernel.bin

ASMC = i686-elf-as
BOOT_FILE =	src/boot.asm
BOOT_BIN = src/boot.o

GRUB_FILE =	grub.cfg

LINKER_FILE = linker.ld

EXEC = modos.iso

all: $(EXEC)

$(EXEC):	${OBJ}
	$(ASMC) $(BOOT_FILE) -o $(BOOT_BIN)
	$(CC) -T $(LINKER_FILE) -o $(KERNEL_BIN) $(BOOT_BIN) $(OBJ) $(FONTS) -lgcc -ffreestanding -O2 -nostdlib

	grub-file --is-x86-multiboot $(KERNEL_BIN)

	cp $(KERNEL_BIN) build/boot/
	cp $(GRUB_FILE) build/boot/grub/
	grub-mkrescue -o $(EXEC) build/

qemu:
	qemu-system-i386 -cdrom modos.iso -vga std -serial tcp::4444,server,nowait

fclean:
	rm -f $(OBJ) $(BOOT_BIN) $(KERNEL_BIN) build/boot/$(KERNEL_BIN) build/boot/grub/$(GRUB_FILE) $(EXEC)

re: fclean $(EXEC)
