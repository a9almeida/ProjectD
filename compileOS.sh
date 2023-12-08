# ProjectD by Aliya Goncalves Almeida & Maya Elysse

nasm bootload.asm
dd if=/dev/zero of=diskc.img bs=512 count=1000
dd if=bootload of=diskc.img bs=512 count=1 conv=notrunc
bcc -ansi -c -o kernel_c.o kernel.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel_c.o kernel_asm.o
./loadFile kernel
./loadFile message.txt

bcc -ansi -c -o shell_c.o shell.c
as86 userlib.asm -o userlib_asm.o
ld86 -o shell -d shell_c.o userlib_asm.o
./loadFile shell
./loadFile tstpr2
