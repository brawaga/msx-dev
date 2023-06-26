**Please reference the original source if forked or distributed/modified another way.**

# Briefly
ASCII MSX-C 1.2 and M80 assembler needed, command2.com 2.4+ recommended.
make.bat or make*.bat to build, install.bat or somehow likely named to install a lib. Start with setting environment vars mentioned below, then building and installing the int32 lib.

#Description
There I will publish all utils for MSX-DOS I have to implement due they are absent or I couldn't find.
I decided to use ASCII MSX-C 1.2 and M80 assembler for development, L80 for linking. It is a pity they limit me to 6 characters length for procedure names, but yet I am allowed to work with directories and link a lot of object files.
Anyway, if someone will hint how to overcome 6 characters limitation for assembler and linker (MSX-C itself can work with 16 characters) I'll appreciate.

# System start up
Very likely you will add those lines to your autoexec.bat:
```
path=a:\utils;a:\msx-c\batch;a:\msx-c\bin
set msxc=a:\msx-c
set include=a:\msx-c\include
```
The real paths may vary depending on how you store your files.
Some of projects rely on pre-initial 32-bit support libs you can find here,
and those want command2.com of version 2.4 at least to build without modification of make.bat, that's why that's a recommended version.

# Building
Any utility compiles via calling make.bat with C source file name without extension given as an only param.
Make sure %msxc% environment variable is set to the directory where MSX-C is installed.
Also, ensure one of two conditions is also satisfied:
1) you set the %include% variable to where all the header files are available;
2) project.cfg contains all the dirs where the header files are and you compile on a RAM-disk created via workon utility.


