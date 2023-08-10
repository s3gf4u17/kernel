# 32 bit infrastructure, do not include exception handling glibc and others
GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = build/loader.o \
	build/gdt.o \
	build/hardware/port.o \
	build/drivers/driver.o \
	build/hardware/interruptstubs.o \
	build/hardware/interrupts.o \
	build/drivers/keyboard.o \
	build/drivers/mouse.o \
	build/kernel.o

build/%.o : src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -o $@ -c $<

build/%.o : src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

kernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

kernel.iso: kernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "OS" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run: kernel.iso
	(killall virtualboxvm && sleep 1) || true
	virtualboxvm --startvm kernel &

.PHONY: clean
clean:
	rm -rf build kernel.bin kernel.iso