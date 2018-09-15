LOADER = bin/loader.o
KERNEL = bin/kernel.o
LINKER = bin/flux.bin

LOADER_SRC = src/loader.s
LINKER_SRC = src/linker.ld
KERNEL_SRC = src/main.cpp

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

kernel:
	@echo
	@echo "\033[4m\033[1mBuilding kernel\033[0m"
	@echo
	@gcc -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -c -o $(KERNEL) $(KERNEL_SRC)

loader:
	@echo
	@echo "\033[4m\033[1mBuilding bootloader\033[0m"
	@echo
	@as --32 -o $(LOADER) $(LOADER_SRC) 

link:
	@echo
	@echo "\033[4m\033[1mLinking\033[0m"
	@echo
	@ld -melf_i386 -T $(LINKER_SRC) -o $(LINKER) $(LOADER) $(KERNEL)

iso:
	@echo
	@echo "\033[4m\033[1mMaking iso\033[0m"
	@echo
	@mkdir bin/iso
	@mkdir bin/iso/boot
	@mkdir bin/iso/boot/grub
	@cp $(LINKER) bin/iso/boot/flux.bin
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
