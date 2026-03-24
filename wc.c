/* wc.c: word counter  */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define IN  1 // inside a word
#define OUT 0 // outside a word

int main(int argc, char *argv[])
{
  int c, state = OUT;
  long nc = 0, nw = 0, nl = 0;
  int show_lines = 0, show_words = 0, show_chars = 0;

  // parse arguments
  if (argc > 1 && strcmp(argv[1], "-l") == 0)
    show_lines = 1;
  else if (argc > 1 && strcmp(argv[1], "-w") == 0)
    show_words = 1;
  else if (argc > 1 && strcmp(argv[1], "-c") == 0)
    show_chars = 1;
  else
    // default: show all
    show_lines = show_words = show_chars = 1;

  // count
  while ((c = getchar()) != EOF) {
    nc++;
    if (c == '\n')
      nl++;
    if (isspace(c))
      state = OUT;
    else if (state == OUT) {
      state = IN;
      nw++;
    }
  }

  // show results
  if (show_lines) printf("lines: %ld\n", nl);
  if (show_words) printf("words: %ld\n", nw);
  if (show_chars) printf("chars: %ld\n", nc);
  
  return 0;
}
