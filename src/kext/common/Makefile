#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	xcodebuild -alltargets -configuration $(CONFIGURATION) MODULE_VERSION=`cat ../../../version` CURRENT_PROJECT_VERSION=`cat ../../../version` build

clean:
	xcodebuild -alltargets -configuration $(CONFIGURATION) clean
	rm -rf build

reload:
	sudo ./scripts/unload.sh
	sudo ./scripts/load.sh
