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
3. `# insmod belt.ko`
4. `$ tail /var/log/syslog` on Ubuntu or `$ tail /var/log/messages` on CentOS to see in syslog that the module is loaded.

To build and load the testing module do the following:

1. `$ cd belt-test`
2. `$ make`
3. `# insmod belt_test.ko` 

The last command will return `insmod: ERROR: could not insert module belt_test.ko: Resource temporarily unavailable`, the test results will be in syslog.
