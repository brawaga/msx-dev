**Please reference the original source if forked or distributed/modified another way.**

There I will publish all utils for MSX-DOS I have to implement due they are absent or I couldn't find. 

# LESS
The only parameter is being interpreted as a file name which is opened and paged on the screen. System hangs if you pass non-existing file (Why?), so be careful.

# PAD
Pads file given as the only parameter to 16kB boundary with FF byte. This is useful to prepare ROM files.

# WORKON
Copies header and library files to ramdisk, from directory given and directories mentioned in project.cfg of that directory.
That is useful because of faster compilation and also no way to say compliler there are several directories to look for headers.
Usage:
```
WORKON <dir>
```
Use COMMIT command to store current state of work, or ENDWORK to do the same but also remove ramdisk.

