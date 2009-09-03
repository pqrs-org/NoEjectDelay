#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

basedir="/Library/org.pqrs/NoEjectDelay"

# --------------------
uname=`uname -r`
case "${uname%%.*}" in
    10)
        kextfile="$basedir/NoEjectDelay.SnowLeopard.kext"
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
        sleep 10 # wait for avoid kext loading collision.
        kextload "$kextfile"
        ;;

    quickstart)
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
