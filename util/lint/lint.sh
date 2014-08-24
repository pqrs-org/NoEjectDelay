#!/bin/sh

basedir=`dirname $0`

for f in `find $basedir/../../* -name 'project.pbxproj' ! -ipath '*/Pods/*'`; do
    echo "Check $f"

    if [ "$(basename $(dirname $(dirname $(dirname $f))))/$(basename $(dirname $(dirname $f)))" = "kext/10.8" ]; then
        plutil -convert json -o - "$f" | "$basedir/xcodeproj.rb" SDKROOT || exit 1
    else
        plutil -convert json -o - "$f" | "$basedir/xcodeproj.rb" || exit 1
    fi
done
