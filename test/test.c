#include <stdio.h>
#include <stdlib.h>

#include <bindiff.h>

#include "minunit.h"

int tests_run = 0;

static char *test_compare_empty() {
  diff_t *result = compare(NULL, NULL, 0);
  mu_assert("Should be no differences", result == NULL);
  return NULL;
}

static char *test_compare_different() {
  char *str1 = "x";
  char *str2 = "y";
  diff_t *result = compare(str1, str2, 1);
  mu_assert("Should be different", result != NULL);
  mu_assert("Only one difference", result->next == NULL);
  mu_assert("Difference is 1 byte", result->size == 1);
  mu_assert("Difference starts immediately",
            result->first == str1 && result->second == str2);
  free(result);
  return NULL;
}

static char *all_tests() {
  mu_run_test(test_compare_empty);
  mu_run_test(test_compare_different);
  return NULL;
}

int main() {
  char *result = all_tests();
  if (result != NULL)
    printf("%s\n", result);
  else
    printf("ALL TESTS PASSED\n");
  printf("Tests run: %d\n", tests_run);

  return result != 0;
}
