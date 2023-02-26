#!/bin/bash

mkdir -p ./flash

# # FAT
# sudo mount -o loop,offset=32256,uid=$(id -u),gid=$(id -g) ./flash.img ./flash

# LittleFs, need littlefs-fuse
echo "==== Mount ===="
sudo losetup /dev/loop0 ./flash.img
sudo lfs -o allow_other -o uid=$(id -u) -o gid=$(id -g) /dev/loop0 ./flash
echo "Press ENTER to umount."
read name
echo "==== Umount ===="
sudo umount ./flash
sudo losetup -d /dev/loop0
echo "==== Done ===="
