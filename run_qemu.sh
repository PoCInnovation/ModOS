mkdir -p iso/boot/grub
cp build/kernel.elf iso/boot/kernel.elf
grub-mkrescue -o my_os.iso iso
qemu-system-x86_64 -cdrom my_os.iso
