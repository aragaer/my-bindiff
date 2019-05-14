#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bindiff.h>

#include "minunit.h"

int tests_run = 0;

void free_results(diff_t *result) {
  while (result) {
    diff_t *to_delete = result;
    result = result->next;
    free(to_delete);
  }
}

static char *test_compare_empty() {
  diff_t *result = compare(NULL, NULL, 0);
  mu_assert("Should be no differences", result == NULL);
  return NULL;
}

static char *test_compare_similar() {
  diff_t *result = compare("hello", "hello", 6);
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
  free_results(result);
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
  free_results(result);
  return NULL;
}

static char *test_printable_if_both_printable() {
  char str1[] = {1};
  char str2[] = "x";
  diff_t *result = compare(str1, str2, 1);
  mu_assert("Difference is not printable if first is not printable",
            !result->printable);
  free_results(result);

  result = compare(str2, str1, 1);
  mu_assert("Difference is not printable if second is not printable",
            !result->printable);
  free_results(result);

  return NULL;
}

static char *test_compare_almost_similar() {
  char *str1 = "hello1";
  char *str2 = "hello2";
  diff_t *result = compare(str1, str2, strlen(str1));
  mu_assert("Should be different", result != NULL);
  mu_assert("Only one difference", result->next == NULL);
  mu_assert("Difference is 1 byte", result->size == 1);
  mu_assert("Difference is the last byte only",
            result->first == strchr(str1, '1')
            && result->second == strchr(str2, '2'));
  mu_assert("Offset is correct", result->offset == strchr(str1, '1')-str1);
  mu_assert("Difference is printable", result->printable);
  free_results(result);
  return NULL;
}

static char *test_compare_almost_similar_bin() {
  char *str1 = "hello\x01";
  char *str2 = "hello\x02";
  diff_t *result = compare(str1, str2, strlen(str1));
  mu_assert("Should be different", result != NULL);
  mu_assert("Only one difference", result->next == NULL);
  mu_assert("Difference is 1 byte", result->size == 1);
  mu_assert("Difference is the last byte only",
            result->first == strchr(str1, 1)
            && result->second == strchr(str2, 2));
  mu_assert("Offset is correct", result->offset == strchr(str1, 1)-str1);
  mu_assert("Difference is not printable", !result->printable);
  free_results(result);
  return NULL;
}

static char *test_compare_completely_different() {
  char *str1 = "abcd";
  char *str2 = "1234";
  diff_t *result = compare(str1, str2, strlen(str1));
  mu_assert("Should be different", result != NULL);
  mu_assert("Only one difference", result->next == NULL);
  mu_assert("Everything is different", result->size == strlen(str1));
  mu_assert("Difference starts immediately",
            result->first == str1 && result->second == str2);
  mu_assert("Offset is 0", result->offset == 0);
  mu_assert("Difference is printable", result->printable);
  free_results(result);
  return NULL;
}

static char *test_compare_print_bin_mix() {
  char *str1 = "abc\x01";
  char *str2 = "abde";
  diff_t *result = compare(str1, str2, strlen(str1));
  mu_assert("Should be different", result != NULL);
  mu_assert("Only one difference", result->next == NULL);
  mu_assert("Correct difference size", result->size == 2);
  mu_assert("Difference starts at right offset",
            result->first == str1+2 && result->second == str2+2);
  mu_assert("Offset is correct", result->offset == 2);
  mu_assert("Difference is not printable", !result->printable);
  free_results(result);
  return NULL;
}

struct expected_difference {
  char *first, *second;
  off_t offset;
  size_t size;
  int printable;
};

#define NELEM(a) (sizeof(a)/(sizeof(*a)))

char msg[80];

static char *verify_one_difference(diff_t *result,
                                   struct expected_difference *expected) {
  sprintf(msg, "Should be a difference");
  mu_assert(msg, result != NULL);
  mu_assert("Correct size", result->size == expected->size);
  mu_assert("Correct start position",
            result->first == expected->first
            && result->second == expected->second);
  mu_assert("Correct offset", result->offset == expected->offset);
  mu_assert("Correct printable flag",
            result->printable == expected->printable);
  return NULL;
}

static char *verify_differences(diff_t *result,
                                struct expected_difference *expected,
                                int count) {
  int i;
  for (i = 0; i < count; i++) {
    char *test_result = verify_one_difference(result, expected+i);
    if (test_result != NULL)
      return test_result;
    result = result->next;
  }
  mu_assert("No more differences", result == NULL);
  return NULL;
}

static char *test_two_differences() {
  char *str1 = "axxxxxxxxxb";
  char *str2 = "cxxxxxxxxxd";
  struct expected_difference expected[] =
    {
     {
      .first = str1,
      .second = str2,
      .offset = 0,
      .size = 1,
      .printable = 1,
     },
     {
      .first = strchr(str1, 'b'),
      .second = strchr(str2, 'd'),
      .offset = strchr(str1, 'b') - str1,
      .size = 1,
      .printable = 1,
     },
    };
  diff_t *result = compare(str1, str2, strlen(str1));
  char *test_result = verify_differences(result, expected, NELEM(expected));
  free_results(result);
  return test_result;
}

static char *test_format_text_difference() {
  char buf[80];
  char *expected = "Difference starting at offset 0 (size 1):\n>>> str y\n<<< str x\n";
  diff_t difference = {.first = "y", .second = "x",
                       .size = 1, .offset = 0,
                       .printable = 1, .next = NULL};
  format_difference(buf, sizeof(buf), &difference);
  mu_assert("Text formatting is correct",
            strcmp(buf, expected) == 0);
  return NULL;
}

static char *test_format_binary_difference() {
  char buf[80];
  char *expected = "Difference starting at offset 0 (size 1):\n>>> hex 01\n<<< hex 02\n";
  diff_t difference = {.first = "\x01", .second = "\x02",
                       .size = 1, .offset = 0,
                       .printable = 0, .next = NULL};
  format_difference(buf, sizeof(buf), &difference);
  mu_assert("Binary formatting is correct",
            strcmp(buf, expected) == 0);
  return NULL;
}

static char *all_tests() {
  mu_run_test(test_compare_empty);
  mu_run_test(test_compare_similar);
  mu_run_test(test_compare_different_string);
  mu_run_test(test_compare_different_binary);
  mu_run_test(test_printable_if_both_printable);
  mu_run_test(test_compare_almost_similar);
  mu_run_test(test_compare_almost_similar_bin);
  mu_run_test(test_compare_completely_different);
  mu_run_test(test_compare_print_bin_mix);
  mu_run_test(test_two_differences);
  mu_run_test(test_format_text_difference);
  mu_run_test(test_format_binary_difference);
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
