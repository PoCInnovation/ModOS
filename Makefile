x86_64_ASM_SRC =	src/header.asm 			\
				  	src/main.asm			\
					src/main64.asm

x86_64_ASM_OBJ := $(patsubst src/%.asm, src/%.o, $(x86_64_ASM_SRC))

SRC	= 	src/kernel.c				\
		src/debug.c					\
		src/string.c				\
		src/pixel.c					\
		src/font.c					\

OBJ	=	$(SRC:.c=.o)
CFLAGS += -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CC	=	x86_64-elf-gcc

# objcopy -O elf32-i386 -B i386 -I binary assets/zap-ext-light32.psf assets/zap-ext-light32.o

FONT = assets/font.o

GRUB_FILE =	grub.cfg
LINKER_FILE = linker.ld
EXEC = modos.iso

$(x86_64_ASM_OBJ): src/%.o : src/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $(patsubst src/%.o, src/%.asm, $@) -o $@

build-x86_64: $(x86_64_ASM_OBJ) $(OBJ)
	mkdir -p dist/x86_64
	mkdir -p build/boot/
	mkdir -p build/boot/grub
	cp $(GRUB_FILE) build/boot/grub
	as assets/font.s -o $(FONT)
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T $(LINKER_FILE) $(x86_64_ASM_OBJ) $(OBJ) $(FONT)
	cp dist/x86_64/kernel.bin build/boot/

	grub-file --is-x86-multiboot2 dist/x86_64/kernel.bin

	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/$(EXEC) build/

qemu:
	qemu-system-x86_64 -cdrom dist/x86_64/modos.iso -vga std -serial tcp::4444,server,nowait

fclean:
	rm -rf build/
	rm -f $(x86_64_ASM_OBJ) $(OBJ) dist/x86_64/kernel.bin dist/x86_64/modos.iso

re: fclean $(EXEC)

.PHONY: build-x86_64
