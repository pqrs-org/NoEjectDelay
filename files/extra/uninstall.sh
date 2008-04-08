#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

# ----------------------------------------
# uninstall
basedir="/Library/org.pqrs/SetEjectDelay"
if [ -d $basedir ]; then
    "$basedir/scripts/unload.sh"
    rm -rf "$basedir"
fi

rm -f "/Library/LaunchDaemons/org.pqrs.SetEjectDelay.load.plist"

exit 0
