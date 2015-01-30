#!/bin/sh
qemu-system-i386  -kernel kernel -d int,cpu_reset -no-reboot -m 64
