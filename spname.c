// spname: return correctly spelled filename

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define DIRSIZ 255

int spname(char *, char *);
int mindist(char *, char *, char*);
int spdist(char *, char *);


int main(int argc, char *argv[]){
  char corrected[DIRSIZ+1];
  int result;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <path>\n", argv[0]);
    return 1;
  }

  result = spname(argv[1], corrected);
  if (result == -1) {
    fprintf(stderr, "No reasonable match found for %s\n", argv[1]);
    return 1;
  } else {
    printf("%s\n", corrected);
  }
}

/* spname(char *oldname, char*newname);
 * returns -1 if no reasonable match to oldname,
 *          0 if exact match,
 *          1 if corrected.
 * stores corrected name in newname
 */
int spname(char *oldname, char *newname) {
  char *p, guess[DIRSIZ+1], best[DIRSIZ+1];
  char *new = newname, *old = oldname;

  for (;;) {
    while (*old=='/') // skip slashes
      *new++ = *old++;
    *new = '\0';

    if (*old == '\0') // exact or corrected
      return strcmp(oldname, newname) != 0;

    p = guess; // copy the next component into guess buffer
    for (; *old != '/' && *old != '\0'; old++)
      if (p < guess+DIRSIZ)
	*p++ = *old;
    *p = '\0';

    if (mindist(newname, guess, best) >= 3)
      return -1;  // no reasonable match
    for (p = best; *new = *p++;)  // add to end
      new++;                      // of new name
  }
}


int mindist(char *dir, char *guess, char *best) { // search dir for guess
  int d, nd;
  struct dirent *entry;
  DIR *dirp;

  if (dir[0] == '\0')
    dir = ".";

  d = 3; // minimum distance

  if ((dirp=opendir(dir)) == NULL) // open directory or return failure
    return d;

  while ((entry = readdir(dirp)) != NULL)
    if (entry->d_ino) {
      // inode has an entry
      nd = spdist(entry->d_name, guess);
      if (nd <= d && nd != 3) {
	strcpy(best, entry->d_name);
	d = nd;
	if (d == 0)  // exact match
	  break;
      }
    }

  closedir(dirp);
  return d;
}

/* spdist: return distance between two names
 *
 * rough spelling metric
 * 0 if the strings are identical
 * 1 if two characters are transposed
 * 2 if one char wrong, added, deleted
 * 3 otherwise
 */
#define EQ(s, t) (strcmp(s,t) == 0)

int spdist(char *s, char *t) {
  while (*s++ == *t)
    if (*t++ == '\0')
      return 0;  // exact match

  if (*--s) {

    if (*t) {
      if (s[1] && t[1] && *s == t[1] && *t == s[1] && EQ(s+2, t+2))
	return 1;  // transposition
    }
    if (EQ(s+1, t+1))
      return 2; // 1 character mismatch

    if (EQ(s+1, t))
      return 2;  // extra character in s
  }

  if (*t && EQ(s, t+1))
    return 2; // missing character in s

  return 3;
}

 
