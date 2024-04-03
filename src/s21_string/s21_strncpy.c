#include "../s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *ptr = dest;

  while (*src && n--) {
    *dest = *src;
    dest++;
    src++;
  }

  *dest = '\0';

  return ptr;
}
