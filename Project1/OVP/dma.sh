#!/bin/sh

# Check Installation supports this example
checkinstall.exe -p install.pkg --nobanner || exit

# Select CrossCompiler for OR1K/or1k
CROSS=RISCV32

# Build Application
make -C application CROSS=${CROSS}

# Build Platform
make -C platform

platform/platform.${IMPERAS_ARCH}.exe -program application/dmaTest.${CROSS}.elf
#platform/platform.${IMPERAS_ARCH}.exe -program application/dmaTest.${CROSS}.elf --mpdegui

