#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

basedir="/Library/org.pqrs/SetEjectDelay"

# --------------------
uname=`uname -r`
case "${uname%%.*}" in
    9)
        kextfile="$basedir/SetEjectDelay.Leopard.kext"
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
        echo "Starting SetEjectDelay"
        sleep 10 # wait for avoid kext loading collision.
        kextload "$kextfile"
        ;;

    quickstart)
        echo "Starting SetEjectDelay"
        kextload "$kextfile"
        ;;

    stop)
        echo "Stopping SetEjectDelay"
        kextunload -b org.pqrs.driver.SetEjectDelay
        ;;

    *)
        echo "Usage: $0 {start|stop}"
        ;;
esac

exit 0
