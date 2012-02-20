NoEjectDelay
============

NoEjectDelay clears the eject key delay.
Use this software when you change the eject key behavior in KeyRemap4MacBook.


System requirements
-------------------

Mac OS X 10.6 or higher.

If you require NoEjectDelay for Mac OS X 10.5.x, use NoEjectDelay 1.1.0.


How to build
------------

Requirements:

* OS X 10.7
* Xcode 4.3+
* Command Line Tools for Xcode
* Auxiliary Tools for Xcode

Please install PackageMaker.app in /Applications/Utilities.
(PackageMaker.app is included in Auxiliary Tools for Xcode.)

### Step1

Getting source code.
Execute a following command in Terminal.app.

<pre>
git clone --depth 10 https://github.com/tekezo/NoEjectDelay.git
</pre>

### Step2

Building a package.
Execute a following command in Terminal.app.

<pre>
cd NoEjectDelay
make
</pre>

Then, NoEjectDelay-VERSION.pkg.zip has been created in the current directory.
It's a distributable package.
