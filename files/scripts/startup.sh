#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

basedir="/Library/org.pqrs/NoEjectDelay"

# --------------------
uname=`uname -r`
case "${uname%%.*}" in
    12)
        kextfile="$basedir/NoEjectDelay.10.8.kext"
        ;;
    13)
        kextfile="$basedir/NoEjectDelay.10.9.signed.kext"
        ;;
esac

if [ "x$kextfile" == 'x' ]; then
    exit 1
fi

# --------------------
argument="$1"
[ -z "$argument" ] && argument=start
case "$argument" in
    start)
        echo "Starting NoEjectDelay"
        kextload "$kextfile"
        ;;

    stop)
        echo "Stopping NoEjectDelay"
        kextunload -b org.pqrs.driver.NoEjectDelay
        ;;

    *)
        echo "Usage: $0 {start|stop}"
        ;;
esac

exit 0
