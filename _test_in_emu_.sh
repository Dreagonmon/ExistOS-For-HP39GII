#!/bin/bash

SYSPATH=build/SystemR/ExistOS.sys
EMUPATH=/run/media/dreagonmon/Data/WindowsPrograms/ExistOSEmu-v2.2

cp "$SYSPATH" "$EMUPATH"

cd "$EMUPATH"
wine qemu-system-arm.exe

sleep 1.0
