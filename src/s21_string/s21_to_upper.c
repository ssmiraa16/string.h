#include "../s21_string.h"

void *s21_to_upper(const char *str) {
  char *res = s21_NULL;
  if (str) {
    s21_size_t len = s21_strlen(str);
    res = (char *)calloc(len, 5 + 1);
    if (res != s21_NULL) {
      for (s21_size_t i = 0; i < len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
          res[i] = str[i] - 32;
        } else {
          res[i] = str[i];
        }
      }
    }
  }
  return res;
}
