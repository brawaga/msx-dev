set echo off

echo dlist.c
newest dlist.rel dlist.c
%onok% echo Fresh, skipping.
%onerror% cf dlist > cf.log
%onerror% cg -k dlist > cg.log
%onerror% m80 =dlist/z > m80.log

echo testdl0.c
newest testdl0.rel testdl0.c
%onok% echo Fresh, skipping.
%onerror% cf testdl0 > cf.log
%onerror% cg -k testdl0 > cg.log
%onerror% m80 =testdl0/z/m > m80.log

echo tesdtdl0.com
newest testdl0.com testdl0.rel dlist.rel %msxc%\lib\ck.rel %msxc%\lib\clib.rel %msxc%\lib\crun32.rel %msxc%\lib\cend.rel
%onok% echo Fresh, skipping.
%onerror% l80 %msxc%\lib\ck,dlist,testdl0,%msxc%\lib\clib/s,%msxc%\lib\crun32/s,%msxc%\lib\cend,testdl0/n/y/e:xmain

echo testdl1.c
newest testdl1.rel testdl1.c
%onok% echo Fresh, skipping.
%onerror% cf testdl1 > cf.log
%onerror% cg -k testdl1 > cg.log
%onerror% m80 =testdl1/z/m > m80.log

echo tesdtdl1.com
newest testdl1.com testdl1.rel dlist.rel %msxc%\lib\ck.rel %msxc%\lib\clib.rel %msxc%\lib\crun32.rel %msxc%\lib\cend.rel
%onok% echo Fresh, skipping.
%onerror% l80 %msxc%\lib\ck,dlist,testdl1,%msxc%\lib\clib/s,%msxc%\lib\crun32/s,%msxc%\lib\cend,testdl1/n/y/e:xmain


