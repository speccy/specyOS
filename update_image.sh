#!/bin/bash

#sudo /sbin/losetup /dev/loop0 floppy.img
#sudo mount /dev/loop0 /mnt2
./util/mount_image.sh
sudo cp spex-kernel /mnt/boot/kernel
#sudo cp initrd.img /mnt/boot/initrd
#sudo umount /dev/loop0
./util/unmount_image.sh
#sudo /sbin/losetup -d /dev/loop0
