Treating all arguments as file names, checks arg1 represents the newest one, otherwise sets nonzero errorlevel (and the variable ERRORLEVEL).
Also sets auxillary variables ONOK and ONERROR for you can make conditional execution like this:
```
NEWEST file1.ext file2.ext file3.ext
%onok% echo file1.ext is the newest
%onerror% echo file2.ext or file3.ext is the newest
```
If /i passed as a parameter, the utility will await file names from stdin, each on separate line.
