#!/bin/sh

ostype=$(basename $(pwd))
version=$(cat ../../../version)
echo "char NoEjectDelay_version[] = \"$version-$ostype\";" > version.hpp
