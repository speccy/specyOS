#!/bin/bash

DISK=spex.img
SIZE=65536

losetup /dev/loop1 "$DISK"

IMAGE_SIZE=`wc -c < $DISK`
IMAGE_SIZE_SECTORS=`expr $IMAGE_SIZE / 512`
MAPPER_LINE="0 $IMAGE_SIZE_SECTORS linear 7:1 0"

echo "$MAPPER_LINE" | dmsetup create hda

kpartx -a /dev/mapper/hda
mount /dev/mapper/hda1 /mnt
