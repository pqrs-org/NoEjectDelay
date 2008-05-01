#!/bin/sh

sudo cp -R build/Release/NoEjectDelay.kext /tmp/
sudo kextload -t /tmp/NoEjectDelay.kext
