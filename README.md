**Please reference the original source if forked or distributed/modified another way.**

There I will publish all utils for MSX-DOS I have to implement due they are absent or I couldn't find. 

# System start up
Very likely you will add those lines to your autoexec.bat:
```
path=a:\utils;a:\msx-c\batch;a:\msx-c\bin
set msxc=a:\msx-c
set include=a:\msx-c\include
```
The real paths may vary depending on how you store your files.

# Building
Any utility compiles via calling make.bat with C source file name without extension given as an only param.
Make sure %msxc% environment variable is set to the directory where MSX-C is installed.
Also, ensure one of two conditions is also satisfied:
1) you set the %include% variable to where all the header files are available;
2) project.cfg contains all the dirs where the header files are and you compile on a RAM-disk created via workon utility.

