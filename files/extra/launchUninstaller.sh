#!/bin/sh

TMPDIR=`/usr/bin/mktemp -d /tmp/NoEjectDelay_uninstaller.XXXXXX` || exit 1
/usr/bin/rsync -a /Applications/NoEjectDelay.app/Contents/Applications/NoEjectDelayUninstaller.app "$TMPDIR"
sh '/Library/Application Support/org.pqrs/NoEjectDelay/setpermissions.sh' "$TMPDIR/NoEjectDelayUninstaller.app"
/usr/bin/open "$TMPDIR/NoEjectDelayUninstaller.app"
