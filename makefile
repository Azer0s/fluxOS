LOADER = bin/loader.o
BINARY = bin/flux.bin
LINKER_SRC = src/linker.ld

OBJECTS = bin/main.o \
bin/core/gdt.o \
bin/com/port.o \
bin/stdlib/memcpy.o \
bin/stdlib/memset.o \
bin/stdlib/stdio.o \
bin/stdlib/stdlib.o \
bin/stdlib/string.o \
bin/com/interrupts.o \
bin/com/interrupts_router.o \
bin/drivers/keyboard.o \
bin/drivers/mouse.o
		

all:
	@$(MAKE) clear
	@$(MAKE) kernel
	@$(MAKE) loader
	@$(MAKE) link
	@$(MAKE) iso
	@$(MAKE) run

clear:
	@echo
	@echo "\033[4m\033[1mClearing output folder\033[0m"
	@echo
	@rm -rf bin/ || true
	@mkdir bin
	@mkdir bin/core
	@mkdir bin/com
	@mkdir bin/stdlib
	@mkdir bin/drivers

kernel:
	@echo
	@echo "\033[4m\033[1mBuilding kernel\033[0m"
	@echo
	@$(foreach bin,$(OBJECTS),$(MAKE) $(bin);)

bin/%.o: src/%.c
	@gcc -m32 -Iinclude -nostdlib -std=c99 -fno-builtin -fno-exceptions -fno-leading-underscore -c -o  $@ $<

bin/%.o: src/%.cpp
	@gcc -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -c -o  $@ $<

bin/%.o: src/%.s
	@as --32 -o $@ $<

bin/%.o: src/%.asm
	@nasm -o $@ $< -f elf -F dwarf -g

loader:
	@echo
	@echo "\033[4m\033[1mBuilding bootloader\033[0m"
	@echo
	@$(MAKE) $(LOADER)

link:
	@echo
	@echo "\033[4m\033[1mLinking\033[0m"
	@echo
	@ld -melf_i386 -T $(LINKER_SRC) -o $(BINARY) $(LOADER) $(OBJECTS)

iso:
	@echo
	@echo "\033[4m\033[1mMaking iso\033[0m"
	@echo
	@mkdir bin/iso
	@mkdir bin/iso/boot
	@mkdir bin/iso/boot/grub
	@cp $(BINARY) bin/iso/boot/flux.bin
	@echo 'set timeout=0' >> bin/iso/boot/grub/grub.cfg
	@echo 'set default=0' >> bin/iso/boot/grub/grub.cfg
	@echo 'menuentry "flux" {' >> bin/iso/boot/grub/grub.cfg
	@echo '	multiboot /boot/flux.bin' >> bin/iso/boot/grub/grub.cfg
	@echo '	boot' >> bin/iso/boot/grub/grub.cfg
	@echo '}' >> bin/iso/boot/grub/grub.cfg
	@grub-mkrescue --output=bin/kernel.iso bin/iso

run:
	@echo
	@echo "\033[4m\033[1mRunning system\033[0m"
	@echo
	@qemu-system-i386 bin/kernel.iso	
