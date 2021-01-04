#include <stdio.h>
#include <bdosfunc.h>

char *constPaths = "paths:";
char *constCreate = "_CREATE.BAT";
char *constMask = "*.*";


char chdrv(no)
	char no;
{
	/* bdos(0x0E, (int)no); */
	return calla(5, (char)0, 0, 0x000E, (int)no);
}

char getpdrv(path)
	char *path;
{
	return path[1] == ':'? (path[0]-'A') & (char)31 : (char)-1;
}

char chpdrv(path)
	char *path;
{
	char drv = getpdrv(*path);
	if (drv != (char)-1) chdrv(drv);
}

char *strchrs(s, cs)
	char *s, *cs;
{
	char *ss = s;
	char *css = cs;
	while (*ss && *ss != *css) {
		if (*css) ++css; else {
			++ss;
			css=cs;
		}
	}
	return ss;
}

char smartClose(f)
	FILE **f;
{
	if (*f) {
		fclose(*f);
		*f = 0;
	}
}

char serveDir(newdir, files, create, commit)
	char *newdir;
	int *files;
	FILE *create, *commit;
{
	XREG regs;
	FIB finfo;
	printf("Directory: %s\n", newdir);
	finfo.fib_id = 0xFF;
	regs.bc = 0x2340;
	regs.de = (int)constMask;
	regs.hl = (int)constMask;
	regs.ix = (int)&finfo;
	callxx(5, &regs);  /* Find first */
	/* While it finds */
	while (0==((char *)&regs.af)[1]) {
		printf("\tPreparing to copy: %s\n", finfo.name);

		/* Skipping _CREATE.BAT file */
		if (strcmp(finfo.name, constCreate)) {
			++*files;
			fprintf(
				create,
				"echo copying %d\ncopy /P %s\\%s h:%s\n",
				*files,
				newdir, finfo.name, finfo.name
			);
			/* Rewriting project files only */
			if (eof) fprintf(
				commit,
				"echo copying %d\ncopy /P h:%s %s\\$.$\ndel /P %s\\%s\nren /P %s\\$.$ %s\n",
				*files,
				finfo.name, newdir,
				newdir, finfo.name,
				newdir, finfo.name
			);
		}
		regs.bc = 0x2341;
		callxx(5, &regs);  /* Find next */
	}
}

int main(argc, argv)
	int argc;
	char **argv;
{
	FILE *commit;
	FILE *create;
	FILE *config;
	char *srch;
	char olddir[65];
	char newdir[65];
	char line[65];
	char olddrv = *((char*)4);
	char newdrv;
	char eof;
	int files=0;

	olddir[64] = (char)0;
	newdir[64] = (char)0;
	line[64] = (char)0;
	chpdrv(argv[1]);
	getcwd(olddir, 64);
	chdir(argv[1]);
	getcwd(newdir, 64);
	
	create = fopen(*constCreate, "wt");
	commit = fopen("h:endwork.bat", "wt");
	config = fopen("project.cfg", "rt");  /* No real YAML support yet */
	if (create && commit) {
		if (config) {
			fgets(line, 65, config);

			/* Expect triple dash */
			eof = 1;
			if (0==strncmp(line, "---", 3)) {
				while (!(eof=0==fgets(line, 65, config))) {       			
					if (strncmp(line, constPaths, 6)==0) break;
				}
			}
			if (eof) smartClose(&config);
		}
		memcpy(line, newdir, 65);
		eof = 1;  /* Now it is flag of project directory. */
		while (line[0]) {
			serveDir(newdir, &files, create, commit);
			if (config && fgets(line, 65, config)) {
				srch = line;
				while (*srch && (*srch == '\t' || *srch == ' ')) ++srch;
				if (srch != line && *srch) {
					memcpy(newdir, srch, 65+line-srch);
					srch = strchrs(newdir, "\n\r");
					*srch = (char)0;
					newdrv = getpdrv(newdir);
					chdir(olddir);
					if (newdrv == (char)-1 || newdrv == *((char *)4)) {
						chdir(argv[1]);
					} else {
						chpdrv(newdir);
						getcwd(olddir, 64);
					}
					chdir(newdir);
					getcwd(newdir, 64);
					eof = (char)0;
				} else {
					line[0] = (char)0;
					smartClose(&config);
				}
			} else {
				line[0] = (char)0;
				smartClose(&config);
			}
		}
		chdir(olddir);
		chpdrv(argv[1]);
		getcwd(olddir, 64);
		chdir(argv[1]);
		getcwd(newdir, 64);
		chdir(olddir);
		srch = argv[0];
		while (*srch) ++srch;
		while (*srch != '\\' && srch != argv[0]) --srch;
		*srch = (char)0;
		fprintf(create, "copy %s\commit.bat h:\n", argv[0]);
		fprintf(create, "h:\ndel /P %s\\_create.bat\n", newdir);
		fprintf(commit, "%%1 ramdisk /d 0\n");
		if ((char)-1 != getpdrv(newdir)) {
			newdir[2] = (char)0;
			fprintf(commit, "%%1 %s\n", newdir);
		}
		fclose(commit);
		fclose(create);
		smartClose(&config);
	} else {
		printf("Failed to create batch files.");
	}
	printf("%d files to copy.\n", files);
	chdir(olddir);
	chdrv(olddrv);
}

