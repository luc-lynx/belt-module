Linux Crypto API Belt module
----------------------------
A module for Linux Kernel Crypto API that implements BELT encryption algorithm (256 key version). 

The algorithm is a part of Belarussian National Standard STB 34.101.31

The repository has two modules - the first implements BELT encrytion algorithm, the second is used for testing and contains some test vectors from STB 34.101.31 standard.

Build and install
-----------------
If you're building the modules on Cent OS, make sure you have `kernel-devel` package installed in your system.

For main module do the following steps:

1. `$ cd belt`
2. `$ make`
3. `# insmod belt.ko` to only load it
4. `# make install` to install and load the module
5. `$ tail /var/log/syslog` on Ubuntu or `$ tail /var/log/messages` on CentOS to see in syslog that the module is loaded.
6. The other way to make sure the module is loaded is to run `$ lsmod | grep belt`. If you install the module, you can load it by running `# modprobe belt# modprobe belt`.

To build and load the testing module do the following:

1. `$ cd belt-test`
2. `$ make`
3. `# insmod belt_test.ko` 

The last command will return `insmod: ERROR: could not insert module belt_test.ko: Resource temporarily unavailable`, the test results will be in syslog.
