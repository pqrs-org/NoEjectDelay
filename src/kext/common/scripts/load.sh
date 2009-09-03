#!/bin/sh

sudo cp -R build/Release/NoEjectDelay.kext /tmp/
sudo kextutil -t /tmp/NoEjectDelay.kext
