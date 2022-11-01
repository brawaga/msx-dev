#include <bdosfunc.h>
#include <stdio.h>
#include <string.h>
#include "int32.h"

char **arg, *filename, buf[81], argn;

char *trim(s)
	char *s;
{
	char c, *t;
	if (!s) return 0;
	c = *s;
	while (c==32 || c==9 || c==13 || c==10) c = *(++s);
	if (!*s) return s;
	t = s;
	while (*t) ++t;
	c = *(--t);
	while (c==32 || c==9 || c==13 || c==10) {*t=0; c = *(--t);}
	return s;
}

char putsw(s)
	char *s;
{
	char *t;
	char left;
	char first, cur;
       	first = *s;
	while (first) {
		t = s;
		fflush(stdout);
		left = *((char *)0xF3B0) - *((char *)0xF3DD) + (char)1;
		if (!left) left = *((char *)0xF3B0);
		if (first<33 || first==127) {
			for(++t; cur=*t,cur && (cur<33 || cur==127); ++t);
		} else {
			for(++t, --left; cur=*t,cur=(char)!(cur<33 || cur == 127),cur && left; ++t, --left);
			if (cur) {
				puts("\05\n");
				t = s;
				continue;
			}
		}
		first = *t;
		*t = (char)0;
		puts(s);
		s = t;
		*t = first;
	}

}

char exitel(v)
	char v;
{
	static char *onok="ONOK";
	static char *onerror="ONERROR";
	char *buf="0\0rem";	
	*buf = v+'0';
	call(5, (char)0, "ERRORLEVEL", 0x6C, buf++);
	call(5, (char)0, v?onerror:onok, 0x6C, buf++);
	call(5, (char)0, v?onok:onerror, 0x6C, buf);
	exit((int)v);
}

char *nxtFN2();
char *nxtFN3()
{
	if (strcmp("/i", filename)) return filename; else return nxtFN2();
}
char *nxtFN0()
{
	++arg;
	filename=(--argn)?*arg:0;
	return nxtFN3();
}
char *nxtFN2()
{
	if (gets(buf, 80)) {
	       	filename = trim(buf);
		return nxtFN3();
	}
	return nxtFN0();
}
char *nxtFN1()
{
       	if (strcmp("/i", filename)) return nxtFN0(); 
       	return nxtFN2();
}
char *nextFN() {
	return (filename == *arg)?nxtFN1():nxtFN2(); 
}

char main(argc, argv)
	int argc;
	char **argv;
{
	int32 dt, maxdt;
	char i, j, k;
	char *dtp, *maxdtp;
	filename = *(arg = argv);
	argn = argc;
	if (!nextFN()) {
		putsw("NEWEST arg1, arg2,...\nTreating all arguments as file names, checks arg1 represents the newest one, otherwise sets nonzero errorlevel (and the variable ERRORLEVEL).\nAlso sets auxillary variables ONOK and ONERROR for you can make conditional execution like this:\n\nNEWEST file1.ext file2.ext file3.ext\n%onok% echo file1.ext is the newest\n%onerror% echo file2.ext or file3.ext is the newest\n\nIf /i passed as a parameter (NOT TESTED), the utility will await file names from stdin, each on separate line.");
		return 0;
	}
   	printf("file name: %s\n", filename);
    	maxdt.v16.n1 = call(5, (char)0, 0, 0x51, filename);  
	if (dummya()) exitel(2);
	maxdt.v16.n0 = swap32(maxdt.v16.n1);
	while (nextFN()) {
	   	printf("file name: %s\n", filename);
		dt.v16.n1 = call(5, (char)0, 0, 0x51, filename);  
		if (dummya()) continue;
		dt.v16.n0 = swap32(dt.v16.n0);
		dtp = ((char *)&dt)+3;
		maxdtp = ((char *)&maxdt)+3;
		for (i=4; i; --i,--dtp,--maxdtp) {
			j = *maxdtp; k = *dtp;
			if (j<k) exitel((char)1);
			if (j>k) break;
		}
	}	
	exitel((char)0);
}
