#!/bin/sh
qemu-system-i386  -kernel ../simux -d int,cpu_reset -no-reboot -m 64 -vnc :1 -initrd mod.mod
