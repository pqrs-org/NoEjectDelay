#!/bin/sh

basedir=`dirname $0`
version=$(cat "$basedir/../version")

find $basedir/.. \( -name 'Info.plist.tmpl' -or -name 'Distribution.xml.tmpl' \) -print0 | while IFS= read -r -d '' f; do
  echo $f
  if [ -f "$f" ]; then
    sed "s|PKGVERSION|$version|g" "$f" > `dirname "$f"`/`basename "$f" .tmpl`
  fi
done
