#include <stdlib.h>

#include <bindiff.h>

diff_t *compare(char *first, char *second, size_t size) {
  if (size == 0)
    return NULL;
  diff_t *result = malloc(sizeof(diff_t));
  result->size = 1;
  result->next = NULL;
  result->first = first;
  result->second = second;
  result->printable = 1;
  result->offset = 0;
  return result;
}
