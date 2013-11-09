#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

version=$(cat version)

make clean build || exit $?

# --------------------------------------------------
# http://developer.apple.com/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptPackaging/packaging_kext.html
echo "Copy Files"

rm -rf pkgroot
mkdir -p pkgroot

basedir="/Library/org.pqrs/NoEjectDelay"
mkdir -p "pkgroot/$basedir"
for ostype in 10.8 10.9; do
    if [ $ostype == "10.8" ]; then
        # We must not sign kext for OS X 10.8 or prior.
        cp -R src/kext/${ostype}/build/Release/NoEjectDelay.kext "pkgroot/$basedir/NoEjectDelay.${ostype}.kext"
    else
        # We should sign kext after OS X 10.9.
        cp -R src/kext/${ostype}/build/Release/NoEjectDelay.kext "pkgroot/$basedir/NoEjectDelay.${ostype}.signed.kext"
    fi
done
cp -R files/scripts "pkgroot/$basedir"

mkdir -p                            "pkgroot/$basedir/extra"
cp -R pkginfo/Scripts/preinstall    "pkgroot/$basedir/extra/uninstall.sh"
cp -R files/extra/setpermissions.sh "pkgroot/$basedir/extra/"

mkdir -p                  "pkgroot/Library"
cp -R files/LaunchDaemons "pkgroot/Library"

# Sign with Developer ID
bash files/extra/codesign.sh pkgroot

# Setting file permissions.
#
# Note:
#   If target files are already exists in system disk,
#   PackageMaker uses their permissions.
#
#   For example:
#     If /Library/org.pqrs permission is 0777 by accidental reasons,
#     the directory permission will be 0777 in Archive.bom
#     even if we set this directory permission to 0755 by setpermissions.sh.
#
#   Then, we need to repair file permissions in postinstall script.
#   Please also see postinstall.
#
sh "files/extra/setpermissions.sh" pkgroot
sh "files/extra/setpermissions.sh" pkginfo
chmod 755 \
    pkginfo/Scripts/postinstall \
    pkginfo/Scripts/preinstall \
    pkginfo/fixbom.rb

# --------------------------------------------------
echo "Create pkg"

pkgName="NoEjectDelay.pkg"
pkgIdentifier="org.pqrs.driver.NoEjectDelay"
archiveName="NoEjectDelay-${version}"

rm -rf $archiveName
mkdir $archiveName

pkgbuild \
    --root pkgroot \
    --component-plist pkginfo/pkgbuild.plist \
    --scripts pkginfo/Scripts \
    --identifier $pkgIdentifier \
    --version $version \
    --install-location "/" \
    $archiveName/Installer.pkg

echo "Fix Archive.bom"
pkgutil --expand $archiveName/Installer.pkg $archiveName/expanded
ruby pkginfo/fixbom.rb $archiveName/expanded/Bom pkgroot/
pkgutil --flatten $archiveName/expanded $archiveName/Installer.pkg
rm -r $archiveName/expanded


productbuild \
    --distribution pkginfo/Distribution.xml \
    --package-path $archiveName \
    $archiveName/$pkgName

rm -f $archiveName/Installer.pkg

# --------------------------------------------------
echo "Sign with Developer ID"
bash files/extra/codesign-pkg.sh $archiveName/$pkgName

# --------------------------------------------------
echo "Make Archive"

# Note:
# Some third vendor archiver fails to extract zip archive.
# Therefore, we use dmg instead of zip.

rm -f $archiveName.dmg
hdiutil create -nospotlight $archiveName.dmg -srcfolder $archiveName
rm -rf $archiveName
chmod 644 $archiveName.dmg
