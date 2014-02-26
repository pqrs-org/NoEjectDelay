#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

# --------------------------------------------------
sudo rm -rf /tmp/NoEjectDelay
mkdir /tmp/NoEjectDelay

sudo cp -R build/Release/NoEjectDelay.kext /tmp/NoEjectDelay/NoEjectDelay.signed.kext
bash ../../../files/extra/codesign.sh /tmp/NoEjectDelay
sudo chown -R root:wheel /tmp/NoEjectDelay

sudo kextutil -t /tmp/NoEjectDelay/NoEjectDelay.signed.kext
