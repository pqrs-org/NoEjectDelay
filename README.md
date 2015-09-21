# NoEjectDelay

NoEjectDelay clears the eject key delay.
Use this software when you change the eject key behavior in KeyRemap4MacBook.


## Web pages

* master: https://pqrs.org/macosx/keyremap4macbook/noejectdelay.html
* backup: http://tekezo.github.io/pqrs.org/


## System requirements

Mac OS X 10.11 or higher.

* If you require NoEjectDelay for Mac OS X 10.10 and 10.9, use NoEjectDelay 6.1.0.
* If you require NoEjectDelay for Mac OS X 10.8, use NoEjectDelay 5.4.0.
* If you require NoEjectDelay for Mac OS X 10.7, use NoEjectDelay 4.1.0.
* If you require NoEjectDelay for Mac OS X 10.6, use NoEjectDelay 3.6.0.
* If you require NoEjectDelay for Mac OS X 10.5, use NoEjectDelay 1.1.0.


## How to build

System requirements:

* OS X 10.10+
* Xcode 7.0+
* Command Line Tools for Xcode

### Step 1: Getting source code

Download the source to master.tar.gz in the current directory, this can be re-executed to restart a cancelled download.

    curl -OLC - https://github.com/tekezo/NoEjectDelay/archive/master.tar.gz

Extract the master.tar.gz file to "NoEjectDelay-master" and delete the tar.gz file

    tar -xvzf master.tar.gz && rm master.tar.gz

### Step2: Building a package

    cd NoEjectDelay-master
    make

The `make` script will create a redistributable **NoEjectDelay-VERSION.dmg** in the current directory.


**Note:**
The build may fail if you have changed any environment variables or if you have modified scripts in the `/usr/bin` locations. Use a clean environment (new account) if this is the case.
