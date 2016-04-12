# Create an empty image file named floppya.img
dd if=/dev/zero of=floppya.img bs=512 count=2880

# Copy bootload into beginning of floppya
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc

# Copy map.img and dir.img into floppya
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc

# Compile kernel.c
bcc -ansi -c -o kernel.o kernel.c

# Assemble kernel.asm
as86 kernel.asm -o kernel_asm.o

# Link kernel with kernel.asm
ld86 -o kernel -d kernel.o kernel_asm.o

# Copy compiled kernel into sector 3 of floppya
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

# Compile cal.c, fib.c and shell.c
bcc -ansi -c -o Shell.o Shell.c
bcc -ansi -c -o cal.o cal.c
bcc -ansi -c -o fib.o fib.c

# Assemble lib.asm
as86 lib.asm -o lib_asm.o

# Link cal, fib and shell with lib
ld86 -o Shell -d Shell.o lib_asm.o
ld86 -o cal -d cal.o lib_asm.o
ld86 -o fib -d fib.o lib_asm.o

# Load files onto floppya.img
./loadFile Shell
./loadFile cal
./loadFile fib
./loadFile lonmsg
./loadFile msg
./loadFile test1
./loadFile test2
