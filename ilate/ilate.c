#include <stdio.h>
#include <bdosfunc.h>

int main(argc, argv)
	int argc;
	char **argv;
{
	FILE *f;
	char line[82];
	char varn[81];
	char varv[256];
	char *result;
	char *newpos;
	char *msg = 0;
	int i;
	memset(varv, (char)0, 256);
	memset(varn, (char)0, 81);
	f = argc>1?fopen(argv[1], "rt"):stdin;
	result = line+81;
	if (f) {
		while (1) {
			if (result==line+81) {
				result=fgets(line, 81, f);
				i = ferror(f);
				if (i) {
					fprintf(stderr, "Error: %d\n", i);
					exit(1);
				}
			}
			if (!result) break;
			newpos = strchr(result, '%');
			if (newpos) {
				*newpos = (char)0;
				puts(result);
				result = ++newpos;
				newpos = strchr(result, '%');
				if (newpos) {
					if (newpos != result) { 
						*newpos = (char)0;
						memcpy(varn, result, newpos-result+1);
			       			call(5, (char)0, varn, 0xFF6B, varv);
						puts(varv);
						result = newpos+1;
					} else {
						putchar('%');
						++result;
					}
				} else {
					/* TODO copy part of varn and load another one */
					newpos = line+81;
					i = newpos-result;
       					msg="Variable name is too long.\n";
					if (i>=80) break;
					memcpy(varn, result, i);
					result = newpos = varn+i;
					fgets(result, 80-i, f);
					newpos = strchr(newpos, '%'); 
					if (!newpos) break;
					*(newpos++)=(char)0;
					msg = 0;
       		       			call(5, (char)0, varn, 0xFF6B, varv);
					puts(varv);
					i = 80-(newpos-varn);
					result = line+81-i;
					if (i) memcpy(result, newpos, i);
				}
			} else {
				puts(result);
				result = line+81;
			}
		}
		if (msg) fputs(stderr, msg);
		if (argc>1) fclose(f);
	} else {
		printf("Error: input unavailable.\n");
	}
}

