#ifndef _BINDIFF_H_
#define _BINDIFF_H_

#include <sys/types.h>

typedef struct diff_s {
  char *first, *second;
  size_t size;
  off_t offset;
  int printable;
  struct diff_s *next;
} diff_t;

diff_t *compare(char *first, char *second, size_t size);
size_t format_difference(char *buf, size_t n, diff_t *difference);

#endif  // _BINDIFF_H_
