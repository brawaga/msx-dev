cf -o%1 %1
fpc %1 lib clib mlib crun
cg %2 -k -o%1 %1
m80 =%1/z
del %1.mac
l80 ck,%1,clib/s,crun/s,cend,%1/n/e

