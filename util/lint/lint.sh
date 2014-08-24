#!/bin/sh

basedir=`dirname $0`

for f in `find $basedir/../../* -name 'project.pbxproj' ! -ipath '*/Pods/*'`; do
    echo "Check $f"

    plutil -convert json -o - "$f" | "$basedir/xcodeproj.rb" || exit 1
done
