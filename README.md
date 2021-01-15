**Please reference the original source if forked or distributed/modified another way.**

There I will publish all utils for MSX-DOS I have to implement due they are absent or I couldn't find. 

# LESS
The only parameter is being interpreted as a file name which is opened and paged on the screen. If no parameter given, stdin will be used, so you can pipe to it.

# PAD
Pads file given as the only parameter to 16kB boundary with FF byte. This is useful to prepare ROM files.

# WORKON
Copies header and library files to RAM disk, from directory given and directories mentioned in project.cfg of that directory.
Project config file is kinda YAML, but parser is very primitive. Two keys supported, both are list of paths: «paths» key encloses all project paths, all files will be copied to RAM disk and then back to the original place, «includes» key encloses paths that will be copied to RAM disk only, so you can put your MSX-C headers directory and libraries directory.
That is useful because of faster compilation and also no way to say compliler there are several directories to look for headers.
MSXC.BAT given in MSX-C works fine on RAM disk.
Usage:
```
WORKON <dir>
```
Use COMMIT command to store current state of work, or ENDWORK to do the same but also remove ramdisk.

