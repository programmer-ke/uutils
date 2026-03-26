// fcopy.c: copy files using filepointers
#include <stdio.h>
#include <stdlib.h>

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

int main(int argc,  char *argv[])
{
  FILE *fin, *fout;
  int c;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
    return 1;
  }

  fin = efopen(argv[1], "r");
  fout = efopen(argv[2], "w");

  while ((c = getc(fin)) != EOF) {
    putc(c, fout);
  }

  fclose(fin);
  fclose(fout);
  return 0;
}
