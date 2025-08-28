#!/bin/sh

CXXFLAGS="-O3 -target i686-windows-msvc -march=i386 -ffunction-sections -fno-builtin-printf -fno-builtin-sprintf"

mkdir pe.x86
lld-link -machine:x86 -def:msvcrt.def -out:pe.x86/msvcrt.lib

clang $CXXFLAGS malloc.c         -Lpe.x86 -lmsvcrt -o pe.x86/malloc.exe         -fuse-ld=lld -Wl,-nodefaultlib,-entry:main
clang $CXXFLAGS printf.c         -Lpe.x86 -lmsvcrt -o pe.x86/printf.exe         -fuse-ld=lld -Wl,-nodefaultlib,-entry:main
clang $CXXFLAGS x86.c            -Lpe.x86 -lmsvcrt -o pe.x86/x86.dll            -fuse-ld=lld -Wl,-nodefaultlib,-noentry,-dll,-base:0x1000
clang $CXXFLAGS x86_arithmetic.c -Lpe.x86 -lmsvcrt -o pe.x86/x86_arithmetic.exe -fuse-ld=lld -Wl,-nodefaultlib,-entry:main
clang $CXXFLAGS x87.c            -Lpe.x86 -lmsvcrt -o pe.x86/x87.dll            -fuse-ld=lld -Wl,-nodefaultlib,-noentry,-dll,-base:0x1000
