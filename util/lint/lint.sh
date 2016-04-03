#!/bin/sh

basedir=`dirname $0`

find $basedir/../../* -name 'project.pbxproj' ! -ipath '*/Pods/*' | while read f; do
    echo "Check $f"

    plutil -convert json -o - "$f" | "$basedir/xcodeproj.rb" || exit 1
done
