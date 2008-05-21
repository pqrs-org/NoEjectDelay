#!/bin/sh

version=$(cat version)

packagemaker=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker
pkgName="NoEjectDelay-${version}.pkg"

echo "char * const config_version = \"$version-Leopard\";" > src/kext/Leopard/version.hpp

make clean build || exit $?

# --------------------------------------------------
# http://developer.apple.com/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptPackaging/packaging_kext.html
echo "Copy Files"

sudo rm -rf pkgroot
sudo mkdir -p pkgroot

basedir="/Library/org.pqrs/NoEjectDelay"
sudo mkdir -p "pkgroot/$basedir"
for ostype in Leopard; do
    sudo cp -R src/kext/${ostype}/build/Release/NoEjectDelay.kext "pkgroot/$basedir/NoEjectDelay.${ostype}.kext"
done
sudo cp -R files/scripts "pkgroot/$basedir"

sudo mkdir -p "pkgroot/$basedir/extra"
sudo cp -R pkginfo/Resources/preflight "pkgroot/$basedir/extra/uninstall.sh"

sudo mkdir -p "pkgroot/Library"
sudo cp -R files/LaunchDaemons pkgroot/Library
sudo mkdir -p "pkgroot/Library/PreferencePanes"

sudo find pkgroot -type d -print0 | xargs -0 sudo chmod 755
sudo find pkgroot -type f -print0 | xargs -0 sudo chmod 644
sudo find pkgroot -name '*.sh' -print0 | xargs -0 sudo chmod 755
sudo chown -R root:wheel pkgroot

sudo chmod 1775 pkgroot/Library
sudo chown root:admin pkgroot/Library

# --------------------------------------------------
echo "Exec PackageMaker"

sudo rm -rf $pkgName
sudo $packagemaker -build \
    -p $pkgName \
    -f pkgroot \
    -ds \
    -r pkginfo/Resources \
    -i pkginfo/Info.plist \
    -d pkginfo/Description.plist

# --------------------------------------------------
echo "Make Archive"

sudo chown -R root:wheel $pkgName
sudo tar zcf $pkgName.tar.gz $pkgName
sudo rm -rf $pkgName
tar zxf $pkgName.tar.gz

# --------------------------------------------------
echo "Cleanup"
sudo rm -rf pkgroot
make -C src/kext clean
