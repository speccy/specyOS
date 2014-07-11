#!/bin/bash

# run_bochs.sh
# mounts the correct loopback device, runs bochs, then unmounts.

qemu-system-i386 -m 512M -vga std -soundhw pcspk -hda spex.img 

