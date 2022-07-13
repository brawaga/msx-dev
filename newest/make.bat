set echo off

newest newest.rel newest.c
echo newest.c
%onok% echo Fresh, skipping.
%onerror% cf %1 newest > cf.log
%onerror% rem fpc newest mlib lib > fpc.log
%onerror% cg -k %2 newest > cg.log
%onerror% m80 =newest/z > m80.log
%onerror% del newest.mac

newest newest.com ck.rel newest.rel mlib.rel clib.rel crun32.rel cend.rel
%onok% echo Linked already.
%onerror% l80 ck,newest,mlib/s,clib/s,crun32/s,cend,newest/n/y/e:xmain

