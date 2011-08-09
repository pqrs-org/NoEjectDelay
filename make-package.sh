#!/bin/sh

version=$(cat version)

packagemaker=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker
pkgName="NoEjectDelay.pkg"
archiveName="NoEjectDelay-${version}.pkg.zip"

make clean build || exit $?

# --------------------------------------------------
# http://developer.apple.com/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptPackaging/packaging_kext.html
echo "Copy Files"

rm -rf pkgroot
mkdir -p pkgroot

basedir="/Library/org.pqrs/NoEjectDelay"
mkdir -p "pkgroot/$basedir"
for ostype in 10.6 10.7; do
    cp -R src/kext/${ostype}/build/Release/NoEjectDelay.kext "pkgroot/$basedir/NoEjectDelay.${ostype}.kext"
done
cp -R files/scripts "pkgroot/$basedir"

mkdir -p "pkgroot/$basedir/extra"
cp -R pkginfo/Resources/preflight "pkgroot/$basedir/extra/uninstall.sh"

mkdir -p "pkgroot/Library"
cp -R files/LaunchDaemons pkgroot/Library

find pkgroot -type d -print0 | xargs -0 chmod 755
find pkgroot -type f -print0 | xargs -0 chmod 644
find pkgroot -name '*.sh' -print0 | xargs -0 chmod 755

# --------------------------------------------------
echo "Exec PackageMaker"

rm -rf $pkgName

# Note: Don't add --no-recommend option.
# It breaks /Library permission.
# (It overwrites /Library permission with pkgroot/Library permission.)
# - Mac OS X 10.5: /Library is 1775
# - Mac OS X 10.6: /Library is 0755
$packagemaker \
    --root pkgroot \
    --info pkginfo/Info.plist \
    --resources pkginfo/Resources \
    --title "NoEjectDelay $version" \
    --no-relocate \
    --out $pkgName

# --------------------------------------------------
echo "Make Archive"

zip -X -r $archiveName $pkgName
rm -rf $pkgName
chmod 644 $archiveName
unzip $archiveName

# --------------------------------------------------
echo "Cleanup"
rm -rf pkgroot
make -C src/kext clean
