#!/bin/sh
qemu-system-i386  -kernel ../simux -d int,cpu_reset -no-reboot -m 64 -initrd mod.mod
