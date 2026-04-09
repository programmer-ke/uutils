/* readslow: keep reading, waiting for more */
#include <unistd.h>

#define STDIN 0
#define STDOUT 1
#define SIZE 8192

int main(){
  char buf[SIZE];
  int n;

  for (;;) {
    while ((n = read(0, buf, sizeof buf)) > 0)
      write(1, buf, n);
    sleep(10);
  }
  return 0;
}
