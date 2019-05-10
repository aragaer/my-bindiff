#include <ctype.h>
#include <stdlib.h>

#include <bindiff.h>

diff_t *compare(char *first, char *second, size_t size) {
  if (size == 0)
    return NULL;

  int i;
  diff_t *result = NULL;
  for (i = 0; i < size; i++)
    if (first[i] != second[i]) {
      if (result == NULL) {
        result = malloc(sizeof(diff_t));
        result->size = 1;
        result->next = NULL;
        result->first = first+i;
        result->second = second+i;
        result->printable = isprint(first[i]) && isprint(second[i]);
        result->offset = i;
      } else {
        result->size += 1;
        result->printable &= isprint(first[i]) && isprint(second[i]);
      }
    }

  return result;
}
