#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_substr(const char *string, size_t first, size_t last) {
  assert(first <= last && last <= strlen(string));

  size_t substr_size = last - first;
  char *substr = (char *)malloc(sizeof(char) * (substr_size + 1));
  if (!substr) {
    warn("get_substr() - memory allocation error");
    return NULL;
  }

  for (size_t i = first; i < last; i++) {
    substr[i - first] = string[i];
  }
  substr[substr_size] = '\0';

  return substr;
}

void emergency_free_split(char **splits, size_t num);

char **strsplit(const char *string, const char sep) {
  size_t string_size = strlen(string);
  size_t substr_count = 0;

  for (size_t i = 0; i <= string_size; i++) {
    if (string[i] == sep || string[i] == '\0') {
      substr_count++;
    }
  }

  char **substrs = (char **)malloc(sizeof(char *) * (substr_count + 1));
  if (!substrs) {
    warn("strplit() - memory allocation error");
    return NULL;
  }

  size_t arr_idx = 0, first = 0, last;

  for (size_t string_idx = 0; string_idx <= string_size; string_idx++) {
    if ((string[string_idx] == sep || string[string_idx] == '\0')) {
      last = string_idx;
      char *substr = get_substr(string, first, last);

      if (!substr) {
        warn("strsplit() - get_substr() function failed");
        emergency_free_split(substrs, arr_idx);
        fprintf(stderr, "strsplit() - get_substr function failed\n");
        return NULL;
      }

      substrs[arr_idx++] = substr;

      first = string_idx + 1;
    }
  }
  substrs[arr_idx] = NULL;

  return substrs;
}

void free_split(char **splits) {
  for (size_t i = 0; splits[i]; i++)
    free(splits[i]);
  free(splits);
}

// in case  of failure will free all allocated memory
void emergency_free_split(char **splits, size_t num) {
  for (size_t i = 0; i < num; i++)
    free(splits[i]);
  free(splits);
}

size_t splitsize(char **splits) {
  char *split;
  size_t size = 0;

  while ((split = splits[size]) != NULL)
    size++;

  return size;
}

char **split_pop(char ***split) {
  size_t size = splitsize(*split);

  if (!size) {
    *split[0] = NULL;
  } else if (size == 1) {
    free((*split)[0]);
    split[0] = NULL;
  } else {
    free((*split)[size - 1]);
    (*split)[size - 1] = NULL;

    char **split_upd = realloc(*split, sizeof(char *) * size);
    if (!split_upd) {
      warn("split_pop() - memory reallcation error\n");
      free_split(*split); // in case of failure kill the split

      return NULL;
    }

    *split = split_upd;
  }

  return *split;
}

/*
void test_strsplit() {
  // Test case 1: Basic split
  char *test1 = "Moscow;;;;;23;;34";
  char **result1 = strsplit(test1, ';');
  char *expected1[] = {"Moscow", "", "", "", "", "23", "", "34", NULL};
  for (int i = 0; expected1[i] != NULL; i++) {
    assert(strcmp(result1[i], expected1[i]) == 0);
  }
  free_split(result1);
  printf("Test 1 passed\n");

  // Test case 2: Single character
  char *test2 = "a";
  char **result2 = strsplit(test2, ';');
  char *expected2[] = {"a", NULL};
  for (int i = 0; expected2[i] != NULL; i++) {
    assert(strcmp(result2[i], expected2[i]) == 0);
  }
  free_split(result2);
  printf("Test 2 passed\n");

  // Test case 3: Empty string
  char *test3 = "";
  char **result3 = strsplit(test3, ';');
  char *expected3[] = {NULL};
  assert(strcmp(result3[0], "\0") == 0);
  free_split(result3);
  printf("Test 3 passed\n");

  // Test case 4: No separator
  char *test4 = "HelloWorld";
  char **result4 = strsplit(test4, ';');
  char *expected4[] = {"HelloWorld", NULL};
  for (int i = 0; expected4[i] != NULL; i++) {
    assert(strcmp(result4[i], expected4[i]) == 0);
  }
  free_split(result4);
  printf("Test 4 passed\n");

  // Test case 5: Multiple separators
  char *test5 = ";;a;;b;;";
  char **result5 = strsplit(test5, ';');
  char *expected5[] = {"", "", "a", "", "b", "", "", NULL};
  for (int i = 0; expected5[i] != NULL; i++) {
    assert(strcmp(result5[i], expected5[i]) == 0);
  }
  free_split(result5);
  printf("Test 5 passed\n");

  printf("All tests passed!\n");
}
*/
