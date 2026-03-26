// lpager.c: line pager with environment configuration
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LINES 24

char *progname;

int main(int argc, char *argv[])
{
  FILE *fp;
  int c, lines = 0, maxlines = DEFAULT_LINES;
  char *envval;

  progname = argv[0];

  // check environment first
  if ((envval = getenv("LINESPERPAGE")) != NULL)
    maxlines = atoi(envval);

  // command line overrides environment
  if (argc > 2 && strcmp(argv[1], "-n") == 0) {
    maxlines = atoi(argv[2]);
    argc -= 2;
    argv += 2;
  }

  if (maxlines <= 0) {
    fprintf(stderr, "%s: invalid line count\n", progname);
    return 1;
  }

  // process input
  if (argc == 1) {
    fp = stdin;
  } else {
    if ((fp = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr, "%s: can't open %s\n", progname, argv[1]);
      return 1;
    }

  }

  printf("Showing %d lines per page (from %s)\n", maxlines, envval ? "environment": "default");

  while ((c = getc(fp)) != EOF) {
    putchar(c);
    if (c == '\n') {
      lines++;
      if (lines >= maxlines) {
	printf("--More-- (press Enter)");
	getchar(); // wait for Enter
	lines = 0;
	  
      }
    }
  }
  

  if (fp != stdin)
    fclose(fp);

  return 0;
  
}
