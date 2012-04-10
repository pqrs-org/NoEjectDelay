all:
	./make-package.sh

build:
	$(MAKE) -C pkginfo
	$(MAKE) -C src/kext

clean:
	git clean -f -x -d
