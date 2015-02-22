#!/bin/sh
cd ../
make clean
make
cd test
qemu-system-x86_64  -kernel ../simux  -no-reboot -m 64 -initrd mod.mod -serial stdio
