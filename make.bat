a:
cd \dev\c\include
cf -o%1 %1
cd \dev\c\lib
fpc %1 lib clib mlib crun
cg -k -o%1 %1
m80 =%1/z
rem del %1.mac
l80 a:\dev\c\lib\ck,%1,a:\dev\c\lib\clib/s,a:\dev\c\lib\crun/s,a:\dev\c\lib\cend,%1/n/e

