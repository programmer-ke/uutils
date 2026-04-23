// waitfile: wait until file stops changing
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

char *progname;

void error (char *s1, char *s2)  // print error message and die
{
  extern char *progname;

  if (progname)
    fprintf(stderr, "%s: ", progname);
  fprintf(stderr, s1, s2);
  if (errno > 0)
    fprintf(stderr, " (%s)", strerror(errno));
  fprintf(stderr, "\n");
  exit(1);
}

int main(int argc, char *argv[]) {
  int fd;
  struct stat stbuf;
  time_t old_time = 0;

  progname = argv[0];
  if (argc < 2)
    error("Usage: %s filename [cmd]", progname);
  if ((fd = open(argv[1], 0)) == -1)
    error("can't open %s", argv[1]);
  fstat(fd, &stbuf);
  while (stbuf.st_mtime != old_time) {
    old_time = stbuf.st_mtime;
    sleep(60);
    fstat(fd, &stbuf);
  }

  if (argc == 2) { // copy file
    execlp("cat", "cat", argv[1], (char *) 0);
    error("can't execute cat %s", argv[1]); // executes if execlp fails to replace process
  } else {
    execvp(argv[2], &argv[2]);
    error("can't execute %s", argv[2]);  // executes if execvp fails to replace process
  }
  exit(0);
}
