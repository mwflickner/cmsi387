OBJECTS = loader.o kmain.o io.o screen.o serial.o gdt.o idt.o gdt_idt.o pic.o printf.o keyboard.o breakpoint.o util.o kheap.o paging.o ordered_array.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
			 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I include -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

program: program.s
	nasm -f bin $< -o iso/modules/$@

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R                              \
				-b boot/grub/stage2_eltorito    \
				-no-emul-boot                   \
				-boot-load-size 4               \
				-A os                           \
				-input-charset utf8             \
				-quiet                          \
				-boot-info-table                \
				-o os.iso                       \
				iso

run: os.iso
	bochs -f bochsrc.txt -q

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

%.s: %.c
	$(CC) -S $(CFLAGS)  $< -o $@

clean:
	rm $(OBJECTS) kernel.elf os.iso

debug: os.iso
	./bochs/bochs -f bochsrc.debug -q 

