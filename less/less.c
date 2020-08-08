
#include <stdio.h>

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
  f = fopen(argv[1], "rt");
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
      c = getch();
      switch(c) {
        case 13: lineCount = 1; break;
        case 32: lineCount = pageSize; break;
        case 27: result = 0; break;
      };
      fprintf(stderr, "\x1BM");
    }
  }
  fclose(f);
}

