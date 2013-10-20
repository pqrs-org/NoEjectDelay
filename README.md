NoEjectDelay
============

NoEjectDelay clears the eject key delay.
Use this software when you change the eject key behavior in KeyRemap4MacBook.


Web pages
---------

* master: https://pqrs.org/macosx/keyremap4macbook/noejectdelay.html
* backup: http://tekezo.github.io/pqrs.org/


System requirements
-------------------

Mac OS X 10.8 or higher.

* If you require NoEjectDelay for Mac OS X 10.5, use NoEjectDelay 1.1.0.
* If you require NoEjectDelay for Mac OS X 10.6, use NoEjectDelay 3.6.0.
* If you require NoEjectDelay for Mac OS X 10.7, use NoEjectDelay 4.1.0.


How to build
------------

Requirements:

* OS X 10.8+
* Xcode 5.0.1+
* Command Line Tools for Xcode
* Auxiliary Tools for Xcode

Please install PackageMaker.app into /Applications/Utilities.
(PackageMaker.app is included in Auxiliary Tools for Xcode.)

### Step1: Getting source code

Execute a following command in Terminal.app.

<pre>
git clone --depth 10 https://github.com/tekezo/NoEjectDelay.git
</pre>

### Step2: Building a package

Execute a following command in Terminal.app.

<pre>
cd NoEjectDelay
make
</pre>

Then, NoEjectDelay-VERSION.dmg has been created in the current directory.
It's a distributable package.


**Note:**<br />
Build may be failed if you changed environment values or changed /usr/bin files.<br />
Use clean environment (new account) if build was failed.
