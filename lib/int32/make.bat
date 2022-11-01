copy %msxc%\batch\arel.bat *.*
%shell% %msxc%\batch\genliba %_DISK% crun32 
del arel.bat
ren crun32.lib crun32.rel
m80 =int32/z

