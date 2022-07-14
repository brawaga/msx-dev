set echo off

echo tree.c
newest tree.rel tree.c
%onok% echo Fresh, skipping.
%onerror% cf tree > cf.log
%onerror% cg -k tree > cg.log
%onerror% m80 =tree/z > m80.log

echo tsttree0.c
newest tsttree0.rel tsttree0.c
%onok% echo Fresh, skipping.
%onerror% cf tsttree0 > cf.log
%onerror% cg -k tsttree0 > cg.log
%onerror% m80 =tsttree0/z/m > m80.log

echo tsttree0.com
newest tsttree0.com %msxc%\lib\ck.rel tree.rel tsttree0.rel %msxc%\lib\clib.rel %msxc%\lib\crun32.rel %msxc%\lib\cend.rel
%onok% echo Fresh, skipping.
%onerror% l80 %msxc%\lib\ck,tree,tsttree0,%msxc%\lib\clib/s,%msxc%\lib\crun32/s,%msxc%\lib\cend,tsttree0/n/y/e:xmain

echo avltree.c
newest avltree.rel avltree.c
%onok% echo Fresh, skipping.
%onerror% cf avltree > cf.log
%onerror% cg -k avltree > cg.log
%onerror% m80 =avltree/z > m80.log

echo tstavl0.c
newest tstavl0.rel tstavl0.c
%onok% echo Fresh, skipping.
%onerror% cf tstavl0 > cf.log
%onerror% cg -k tstavl0 > cg.log
%onerror% m80 =tstavl0/z/m > m80.log

echo tstavl0.com
newest tstavl0.com %msxc%\lib\ck.rel tree.rel avltree.rel tstavl0.rel %msxc%\lib\clib.rel %msxc%\lib\crun32.rel %msxc%\lib\cend.rel
%onok% echo Fresh, skipping.
%onerror% l80 %msxc%\lib\ck,tree,avltree,tstavl0,%msxc%\lib\clib/s,%msxc%\lib\crun32/s,%msxc%\lib\cend,tstavl0/n/y/e:xmain

