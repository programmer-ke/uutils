// timeout: set time limit on a process
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

int pid;
char *progname;

void onalarm() {  // kill child when alarm arrives
  kill(pid, SIGKILL);
}

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
  int sec = 10, status;

  progname = argv[0];
  if (argc > 1 && argv[1][0] == '-') {
    sec = atoi(&argv[1][1]);
    argc--;
    argv++;
  }

  if (argc < 2)
    error("Usage: %s [-10] command", progname);
  
  if ((pid = fork()) == 0) {
    execvp(argv[1], &argv[1]);
    error("couldn't start %s", argv[1]);
  }
  signal(SIGALRM, onalarm);
  alarm(sec);
  if (wait(&status) == -1 || (status & 0177) != 0)
    error("%s killed", argv[1]);
  exit((status >> 8) & 0377);
}
