all:
	./make-package.sh

build:
	$(MAKE) -C pkginfo
	$(MAKE) -C src/kext

clean:
	$(MAKE) -C src/kext clean
	rm -rf pkgroot
	rm -rf *.pkg
	rm -rf *.zip

source:
	./make-source.sh
