#include <stdio.h>
#include <msxbios.h> 

int main(argc, argv)
	int argc;
	char **argv;
{
	FILE *f;
	char line[81];
	char *result;
	char pageSize = 20;
	char lineCount = pageSize;
	char *prompt = "Page: [Sp], line: [Rtn], exit: [Esc]?";
	int i;
	f = argc>1?fopen(argv[1], "rt"):stdin;
	if (f) {
		result = fgets(line, 80, f);
		i = ferror(f);
		fprintf("Error: %d\n", i);
		while (result) {
			if (lineCount) {
				--lineCount;
				puts(line);
				result = fgets(line, 80, f);
			} else {
				char c;
				fprintf(stderr, prompt);
				c = chget();
				switch(c) {
				case 'K': case 'k': case 'L': case 'l':
				case 'D': case 'd': case '1': case '!':
				case 13:
					lineCount = 1; break;
				case 'N': case 'n': case 32:
					lineCount = pageSize; break;
				case 'Q': case 'q': case 'X': case 'x':
				case '.': case 27:
					result = 0; break;
				};
				fprintf(stderr, "\x1BM");
			}
		}
		if (argc>1) fclose(f);
	} else {
		printf("Error: input unavailable.\n");
	}
}

