#!/bin/sh
cd ../
make clean
make
cd test
qemu-system-x86_64  -kernel ../simux -d int,cpu_reset -no-reboot -m 64 -initrd mod.mod
