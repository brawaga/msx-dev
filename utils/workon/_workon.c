#include <stdio.h>
#include <bdosfunc.h>

#define detectPartition dtPttn
#define serveDir srvDir
#define smartClose smClos

char *constCreate = "H:_CREATE.BAT";
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
	char *s;
	char *cs;
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

char detectPartition(line)
	char *line;
{
	char *partsRaw="includes\0paths\0sources";
	char cand, *l, *p;	
	cand = (char)0;
	p = partsRaw;
	while (cand < 3) {
		l=line;
		while(*l && *p && *l==*p) {++l; ++p;}
		if (*l==':' && !*p) return cand;
		while(*p) ++p;
		++p; ++cand;
	}
	return (char)-1;
}

char isDir(partit)
	char partit;
{
	return partit == 0 || partit == 1;
}

char serveDir(isproject, newdir, files, create, commit)
	char isproject;	
	char *newdir;
	int *files;
	FILE *create, *commit;
{
	XREG regs;
	FIB finfo;
	char *pcent = "%";
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
		if (strcmp(finfo.name, &(constCreate[2]))) {
			++*files;
			fprintf(
				create,
				"echo copying %d\ncopy /P %s\\%s h:%s\n",
				*files,
				newdir, finfo.name, finfo.name
			);
			/* Rewriting project files only */
			if (isproject) fprintf(
				commit,
				"file #%d\nnewest %s\\%s h:%s\n%sonok%s echo No changes\n%sonerror%s echo Copying\n%sonerror%s copy /P h:%s %s\\$.$\n%sonerror%s del /P %s\\%s\n%sonerror%s ren /P %s\\$.$ %s\n",
				*files,
				newdir, finfo.name, finfo.name,
				pcent, pcent,
				pcent, pcent,
				pcent, pcent, finfo.name, newdir,
				pcent, pcent, newdir, finfo.name,
				pcent, pcent, newdir, finfo.name
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
	char neof;
	char state=(char)-1;
	int files=0;

	olddir[64]=newdir[64]=line[64]=(char)0;
	chpdrv(argv[1]);
	getcwd(olddir, 64);
	chdir(argv[1]);
	getcwd(newdir, 64);
	
	create = fopen(constCreate, "wt");
	commit = fopen("h:endwork.bat", "wt");
	config = fopen("project.cfg", "rt");  /* No real YAML support yet */
	if (create && commit) {
		if (config) {
			neof=0!=fgets(line, 65, config);
			line[0] = (char)0;
                        while (neof) {
				if (0==strncmp(line, "---", 3)) state=(char)-1;
				neof = detectPartition(line);
				if (neof != (char)-1) {state = neof; line[0]=(char)0;}
				if (state == (char)-1) line[0]=(char)0;
				srch = line;
				while (*srch == '\t' || *srch == ' ') ++srch;
				if (srch != line && *srch) {
					if (isDir(state)) {
						printf("Dir: %d\n", (int)state);
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
						serveDir(state, newdir, &files, create, commit);
					} else if (state==2) {
						puts("2\n");
						/* TODO handle sources */
						/* build make.bat */
					}
       					line[0] = (char)0;
 				}	
				neof=0!=fgets(line, 65, config);       			
			}
			fclose(config);
		}

		/* Config is absent or invalid */
		if (state==(char)-1) serveDir(state, newdir, &files, create, commit);

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
		fprintf(create, "copy %s\\commit.bat h:\n", argv[0]);
		fprintf(create, "h: \n del /P _create.bat\n");
		fprintf(commit, "%%1 ramdisk /d 0\n");
		if ((char)-1 != getpdrv(newdir)) {
			newdir[2] = (char)0;
			fprintf(commit, "%%1 %s\n", newdir);
		}
		close(create);
		close(commit);
		create = 0;
		commit = 0;
	} else {
		smartClose(commit);
		smartClose(create);
		printf("Failed to create batch files.\n");
	}
	printf("%d files to copy.\n", files);
	chdir(olddir);
	chdrv(olddrv);
}


