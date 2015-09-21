#!/bin/sh

sh '/Library/Application Support/org.pqrs/NoEjectDelay/uninstall_core.sh'

# remove receipts
rm -f /var/db/receipts/org.pqrs.driver.NoEjectDelay.*

exit 0
