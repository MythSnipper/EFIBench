.PHONY: build clean debug run givesudo copy-to-usbroot copy-to-device qemu qemu-kvm qemu-refresh

DEVICE ?= /dev/null

#part to install on
EFI_PART ?= $(DEVICE)p1

#UEFI files for QEMU
QEMU_UEFI_CODE = OVMF_CODE.fd
QEMU_UEFI_VARS = OVMF_VARS.fd

#tools used
CC = gcc
LD = ld
OCP = objcopy
AS = nasm
STRIP = strip

CFLAGS = \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-unused-variable \
	-Ignu-efi/inc \
	-Iinclude \
	-fpic \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fshort-wchar \
	-mno-red-zone \
	-maccumulate-outgoing-args \
	-O2

LDFLAGS = \
	-shared \
	-Bsymbolic \
	-Lgnu-efi/x86_64/lib \
	-Lgnu-efi/x86_64/gnuefi \
	-Tgnu-efi/gnuefi/elf_x86_64_efi.lds \
	gnu-efi/x86_64/gnuefi/crt0-efi-x86_64.o

LDFLAGS_L = \
	-lgnuefi \
	-lefi

OCPFLAGS = \
	-j .text \
	-j .sdata \
	-j .data \
	-j .rodata \
	-j .dynamic \
	-j .dynsym \
	-j .rel \
	-j .rela \
	-j .rel.* \
	-j .rela.* \
	-j .reloc \
	--output-target efi-app-x86_64 \
	--subsystem=10


debug: givesudo clean build copy-to-usbroot copy-to-device qemu-refresh qemu

run: givesudo clean build copy-to-usbroot copy-to-device qemu-refresh qemu-kvm

givesudo:
	sudo echo give me sudo

clean:
	rm -rf build/*

#build code into build directory
build:
	mkdir -p build
	$(CC) $(CFLAGS) -c src/EFIBench.c -o build/EFIBench.o
	$(LD) $(LDFLAGS) build/EFIBench.o -o build/EFIBench.so $(LDFLAGS_L)
	$(STRIP) build/EFIBench.so
	$(OCP) $(OCPFLAGS) build/EFIBench.so build/EFIBench.efi


#copy code and data to usbroot
copy-to-usbroot:
	mkdir -p usbroot/EFI/BOOT
	cp build/EFIBench.efi usbroot/EFI/BOOT/BOOTX64.EFI
	cp gnu-efi/Shell_Full.efi usbroot/EFI/BOOT/SHELLX64.EFI

#copy files to device
copy-to-device:
	lsblk

	sudo mount $(EFI_PART) mount/
	sudo rm -rf mount/*
	sudo cp -r usbroot/* mount/
	sudo tree mount/
	sudo sync
	sudo umount $(EFI_PART)

#test in qemu(fast)
qemu-kvm:
	sudo sync
	sudo -E qemu-system-x86_64 \
		-cpu host \
		-enable-kvm \
		-m 8192 \
		-net none \
		-drive if=pflash,format=raw,unit=0,file=ovmf/temp/$(QEMU_UEFI_CODE),readonly=on \
		-drive if=pflash,format=raw,unit=1,file=ovmf/temp/$(QEMU_UEFI_VARS) \
		-usb -device usb-storage,drive=nuckusb \
		-drive file=$(DEVICE),if=none,format=raw,id=nuckusb

#test in qemu(software emulated, pause on start, needs gdb)
qemu:
	sudo sync
	sudo -E qemu-system-x86_64 \
		-accel tcg \
		-m 8192 \
		-net none \
		-drive if=pflash,format=raw,unit=0,file=ovmf/temp/$(QEMU_UEFI_CODE),readonly=on \
		-drive if=pflash,format=raw,unit=1,file=ovmf/temp/$(QEMU_UEFI_VARS) \
		-usb -device usb-storage,drive=nuckusb \
		-drive file=$(DEVICE),if=none,format=raw,id=nuckusb \
		-s -S

#refresh UEFI firmware files used for qemu
qemu-refresh:
	rm -f ovmf/temp/$(QEMU_UEFI_CODE) ovmf/temp/$(QEMU_UEFI_VARS)
	cp ovmf/$(QEMU_UEFI_CODE) ovmf/temp/
	cp ovmf/$(QEMU_UEFI_VARS) ovmf/temp/





#ramdisk size 100M
ramdisk_size = 102400

ramdisk:
	sudo modprobe brd rd_nr=1 rd_size=$(ramdisk_size)

	printf "g\nn\n\n\n\nt\n1\nw\n" | sudo fdisk /dev/ram0

	sudo partprobe /dev/ram0
	sleep 1

	sudo mkfs.vfat -F32 -n EFIBENCH /dev/ram0p1

	lsblk /dev/ram0

uramdisk:
	sudo modprobe -r brd




