#!/bin/sh

# Check Installation supports this example
checkinstall.exe -p install.pkg --nobanner || exit

# Select CrossCompiler for OR1K/or1k
CROSS=RISCV32

# Build Application
make -C application -f Makefile.hello CROSS=${CROSS}

# Build Platform
make -C platform

platform/platform.${IMPERAS_ARCH}.exe -program application/HelloWorld.${CROSS}.elf

