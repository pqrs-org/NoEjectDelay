#!/bin/sh

sudo cp -R build/Release/SetEjectDelay.kext /tmp/
sudo kextload -t /tmp/SetEjectDelay.kext
sudo /Library/org.pqrs/SetEjectDelay/scripts/sysctl.sh
