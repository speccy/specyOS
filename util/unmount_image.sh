#!/bin/bash

umount /mnt
kpartx -d /dev/mapper/hda
dmsetup remove hda
losetup -d /dev/loop1
