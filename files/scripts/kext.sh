#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

basedir="/Library/org.pqrs/SetEjectDelay"
kextfile=''
uname=`uname -r`
case "${uname%%.*}" in
    9)
        kextfile="$basedir/SetEjectDelay.Leopard.kext"
        ;;
esac

if [ "x$kextfile" == 'x' ]; then
    exit 1
fi

if [ "$1" == 'unload' ]; then
    kextunload -b org.pqrs.driver.SetEjectDelay
else
    kextload "$kextfile"
    sleep 3 # wait for a while just after kextload.

    sysctl="$basedir/scripts/sysctl.sh"
    [ -f "$sysctl" ] && "$sysctl"

    "$basedir/scripts/save.sh"
fi

exit 0
