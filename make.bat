newest %1.rel %1.c
%onerror% cf -o%1 %1
%onerror% cg -k -o%1 %1
%onerror% m80 =%1/z/m
%onerror% del %1.mac
newest %1.com %1.rel
l80 %msxc%\lib\ck,%1,%msxc%\lib\clib/s,%msxc%\lib\crun/s,%msxc%\lib\cend,%1/n/y/e

