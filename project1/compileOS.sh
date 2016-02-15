# Create an empty image file named floppya.img
dd if=/dev/zero of=floppya.img bs=512 count=2880

# Copy bootload into beginning of floppya
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc

# Compile kernel.c
bcc -ansi -c -o kernel.o kernel.c

# Assemble kernel.asm and link it with kernel
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o

# Copy compiled kernel into sector 3 of floppya
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3