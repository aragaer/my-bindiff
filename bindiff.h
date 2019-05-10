#ifndef _BINDIFF_H_
#define _BINDIFF_H_

typedef struct diff_s {
  char *first, *second;
  size_t size;
  off_t offset;
  int printable;
  struct diff_s *next;
} diff_t;

diff_t *compare(char *first, char *second, size_t size);

#endif  // _BINDIFF_H_
