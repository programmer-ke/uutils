/* view.c: view files iwth error handling */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *progname; // for error messages

// Error-checking fopen
FILE *efopen(char *file, char *mode)
{
  FILE *fp;

  if ((fp = fopen(file, mode)) == NULL) {
    fprintf(stderr, "Cannot open %s mode %s\n", file, mode);
    exit(1);
  }
  return fp;
}

void view(FILE *fp)
{
  int c, line = 1;

  printf("%6d:", line++);
  while ((c = getc(fp)) != EOF) {
    putchar(c);
    if (c == '\n')
      printf("%6d:", line++);
  }
}

int main(int argc, char *argv[]) {
  FILE *fp;

  if (argc == 1) {
    // no arguments: read from stdin
    view(stdin);

  } else {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-") == 0) {
	// dash means stdin
	view(stdin);
      } else {
	fp = efopen(argv[i], "r");
	view(fp);
	if (argc > 2)
	  printf("\n==== %s ====\n", argv[i]);
	fclose(fp);
      }
    }
  }
  
}
