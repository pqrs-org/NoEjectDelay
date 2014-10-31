all:
	./make-package.sh

build:
	$(MAKE) -C pkginfo
	$(MAKE) -C src/kext

clean:
	$(MAKE) -C pkginfo clean
	$(MAKE) -C src/kext clean

gitclean:
	git clean -f -x -d
