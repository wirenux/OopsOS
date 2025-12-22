AS=i686-elf-as
CC=i686-elf-gcc

BUILD_DIR=build
KERNEL_DIR=kernel

# Find all .c files in the kernel directory
C_SOURCES=$(wildcard $(KERNEL_DIR)/*.c) $(wildcard $(KERNEL_DIR)/commands/*.c)

# Convert the list of .c files into a list of .o files in the build directory
OBJS=$(patsubst $(KERNEL_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
OBJS += $(BUILD_DIR)/boot.o

# Output
KERNEL_BIN=oopsos.bin
ISO=OopsOs.iso

# Flags
CFLAGS=-ffreestanding -m32 -O2 -Wall -Wextra -nostdlib
LDFLAGS=-T linker.ld -ffreestanding -m32 -nostdlib

all: $(ISO)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Assemble bootloader
$(BUILD_DIR)/boot.o: boot.s | $(BUILD_DIR)
	$(AS) boot.s -o $(BUILD_DIR)/boot.o

# Compiles ANY .c file in kernel/ into a .o file in build/
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel + bootloader
$(KERNEL_BIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $(KERNEL_BIN) $(OBJS)
	@echo "Checking multiboot..."
	@if grub-file --is-x86-multiboot $(KERNEL_BIN) || grub2-file --is-x86-multiboot $(KERNEL_BIN); then \
		echo "Multiboot confirmed"; \
	else \
		echo "ERROR: The file is not multiboot"; exit 1; \
	fi

$(ISO): $(KERNEL_BIN)
	@mkdir -p iso/boot/grub
	@cp $(KERNEL_BIN) iso/boot/
	@echo 'menuentry "OopsOs" { multiboot /boot/oopsos.bin }' > iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso/ || grub2-mkrescue -o $(ISO) iso/

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 1024 -boot d -vga std

# Usage: make write DEVICE=/dev/sdX
write: $(ISO)
ifeq ($(DEVICE),)
	$(error You must specify DEVICE, e.g., make write DEVICE=/dev/sdX)
endif
	@echo "WARNING: This will erase all data on $(DEVICE)!"
	@bash -c '\
		read -p "Are you sure? (Y/n): " confirm; \
		if [ "$$confirm" != "" ]; then \
			echo "Aborted."; \
		else \
			echo "Writing $(ISO) to $(DEVICE)..."; \
			sudo dd if=$(ISO) of=$(DEVICE) bs=4M status=progress conv=fsync; \
			sync; \
			echo "Done!"; \
		fi'

clean:
	rm -rf $(BUILD_DIR) $(KERNEL_BIN) $(ISO) iso