#!/bin/sh

mkdir pe.x86
lld-link -machine:x86 -def:msvcrt.def -out:pe.x86/msvcrt.lib

clang -O3 -target i686-windows-msvc -march=i386 -ffunction-sections -fno-builtin-printf printf.c         -Lpe.x86 -lmsvcrt -o pe.x86/printf.exe         -fuse-ld=lld -Wl,-nodefaultlib,-entry:main
clang -O3 -target i686-windows-msvc -march=i386 -ffunction-sections -fno-builtin-printf x86.c            -Lpe.x86 -lmsvcrt -o pe.x86/x86.dll            -fuse-ld=lld -Wl,-nodefaultlib,-noentry,-dll,-base:0x1000
clang -O3 -target i686-windows-msvc -march=i386 -ffunction-sections -fno-builtin-printf x86_arithmetic.c -Lpe.x86 -lmsvcrt -o pe.x86/x86_arithmetic.exe -fuse-ld=lld -Wl,-nodefaultlib,-entry:main
