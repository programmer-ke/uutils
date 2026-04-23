// shellexec: execute a command in the shell
//   demonstrates built in system library
#include <signal.h>
#include <stdio.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

char *progname;

int mysystem(char *s) {  // run command line 2
  int status, pid, w, tty;
  void (*istat)(), (*qstat)();
  extern char *progname;

  fflush(stdout);
  tty = open("/dev/tty", 2);
  if (tty == -1) {
    fprintf(stderr, "%s: canot open /dev/tty\n", progname);
    return -1;
  }

  if ((pid = fork()) == 0) {
    close(0); dup(tty);
    close(1); dup(tty);
    close(2); dup(tty);
    close(tty);
    execlp("sh", "sh", "-c", s, (char *) 0);
    exit(127);
  }

  close(tty);
  istat = signal(SIGINT, SIG_IGN);
  qstat = signal(SIGQUIT, SIG_IGN);

  while((w = wait(&status)) != pid && w != -1)
    ;
  if (w == -1)
    status = -1;
  signal(SIGINT, istat);
  signal(SIGQUIT, qstat);
  return status;
}


int main(int argc, char *argv[])
{
  progname = argv[0];
  char *command;

  if (argc != 2) {
    fprintf(stderr, "usage: %s command\n", progname);
    return 1;
  }

  command = argv[1];
    
  /* Run a command */
  printf("Running '%s' command:\n", command);
  int result = mysystem(command);
    
  if (result == -1) {
    fprintf(stderr, "Failed to execute command\n");
    return 1;
  }
    
  return 0;
}
