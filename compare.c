#include <ctype.h>
#include <stdlib.h>

#include <bindiff.h>

void push(diff_t **root, diff_t *item) {
  item->next = *root;
  *root = item;
}

void reverse(diff_t **root) {
  diff_t *new_root = NULL;
  while (*root) {
    diff_t *current = *root;
    *root = (*root)->next;
    push(&new_root, current);
  }
  *root = new_root;
}

int is_printable(char c) {
  return isprint(c) || c == '\0';
}

diff_t *compare(char *first, char *second, size_t size) {
  if (size == 0)
    return NULL;

  int i;
  diff_t *chain = NULL;
  diff_t *difference = NULL;
  for (i = 0; i < size; i++)
    if (first[i] == second[i]) {
      if (difference != NULL) {
        push(&chain, difference);
        difference = NULL;
      }
    } else {
      if (difference == NULL) {
        difference = malloc(sizeof(diff_t));
        difference->size = 1;
        difference->next = NULL;
        difference->first = first+i;
        difference->second = second+i;
        difference->printable = is_printable(first[i]) && is_printable(second[i]);
        difference->offset = i;
      } else {
        difference->size += 1;
        difference->printable &= is_printable(first[i]) && is_printable(second[i]);
      }
    }

  if (difference != NULL)
    push(&chain, difference);

  reverse(&chain);

  return chain;
}
