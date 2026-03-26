// mgrep.c: enhanced grep usign popen
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define MAXLINE 1000

int main(int argc, char *argv[]) {
  FILE *fp;
  char cmd[MAXLINE], line[MAXLINE];
  int count = 0, show_count = 0, ignore_case = 0;
  char *pattern;

  // parse arguments
  if (argc < 2) {
    fprintf(stderr, "Usage: %s [-c] [-i] pattern [file...]\n", argv[0]);
    return 1;
  }

  // process options
  while (argc > 1 && argv[1][0] == '-') {
    if (strcmp(argv[1], "-c") == 0)
      show_count = 1;
    else if (strcmp(argv[1], "-i") == 0)
      ignore_case = 1;
    else {
      fprintf(stderr, "Unknown option: %s\n", argv[1]);
      return 1;
    }
    argc--;
    argv++;
  }

  if (argc < 2) {
      fprintf(stderr, "Pattern required\n");
      return 1;
  }

  pattern = argv[1];
  
  // build grep command
  strcpy(cmd, "grep ");
  if (ignore_case) strcat(cmd, "-i ");
  if (show_count) strcat(cmd, "-c ");
  strcat(cmd, "'");
  strcat(cmd, pattern);
  strcat(cmd, "'");

  // add files or stdin
  if (argc == 2) {
    strcat(cmd, " /dev/stdin");
  } else {
    for (int i = 2; i < argc; i++) {
      strcat(cmd, " ");
      strcat(cmd, argv[i]);
    }
  }

  // execute grep
  if ((fp = popen(cmd, "r")) == NULL) {
    perror("popen failed");
    return 1;
  }

  // read and display results
  while ((fgets(line, sizeof(line), fp)) != NULL)
    fputs(line, stdout);

  pclose(fp);
  return 0;

}

