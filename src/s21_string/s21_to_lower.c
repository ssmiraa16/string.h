#include "../s21_string.h"

void *s21_to_lower(const char *str) {
  char *result = s21_NULL;
  if (str) {
    result = (char *)malloc((s21_strlen(str) + 1) * sizeof(char));
  }
  if (result) {
    s21_strncpy(result, str, s21_strlen(str) + 1);
    for (char *i = result; *i; i++) {
      if (*i >= 'A' && *i <= 'Z') *i += 32;
    }
  }
  return (void *)result;
}
