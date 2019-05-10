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

static char *test_compare_different_string() {
  char *str1 = "x";
  char *str2 = "y";
  diff_t *result = compare(str1, str2, 1);
  mu_assert("Should be different", result != NULL);
  mu_assert("Only one difference", result->next == NULL);
  mu_assert("Difference is 1 byte", result->size == 1);
  mu_assert("Difference starts immediately",
            result->first == str1 && result->second == str2);
  mu_assert("Offset is 0", result->offset == 0);
  mu_assert("Difference is printable", result->printable);
  free(result);
  return NULL;
}

static char *test_compare_different_binary() {
  char str1[] = {1};
  char str2[] = {2};
  diff_t *result = compare(str1, str2, 1);
  mu_assert("Should be different", result != NULL);
  mu_assert("Only one difference", result->next == NULL);
  mu_assert("Difference is 1 byte", result->size == 1);
  mu_assert("Difference starts immediately",
            result->first == str1 && result->second == str2);
  mu_assert("Offset is 0", result->offset == 0);
  mu_assert("Difference is not printable", !result->printable);
  free(result);
  return NULL;
}

static char *test_printable_if_both_printable() {
  char str1[] = {1};
  char str2[] = "x";
  diff_t *result = compare(str1, str2, 1);
  mu_assert("Difference is not printable if first is not printable",
            !result->printable);
  free(result);

  result = compare(str2, str1, 1);
  mu_assert("Difference is not printable if second is not printable",
            !result->printable);
  free(result);

  return NULL;
}

static char *all_tests() {
  mu_run_test(test_compare_empty);
  mu_run_test(test_compare_different_string);
  mu_run_test(test_compare_different_binary);
  mu_run_test(test_printable_if_both_printable);
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
