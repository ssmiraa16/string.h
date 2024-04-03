#include "../s21_sscanf.h"

int s21_sscanf(const char *src, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int result = 0;
  scan_specifier(format, src, &result, &ap);
  va_end(ap);
  return result;
}

void scan_specifier(const char *format, const char *src, int *result,
                    va_list *args) {
  int i = 0;
  char temp[1024] = {'\0'};
  strcpy(temp, src);
  char *str = s21_NULL;
  str = temp;
  int count_item = 0;
  int stop_scan = 0;
  int for_n = 0;
  int skip = 0;
  flgs flag = {0};
  specif specf = {0};
  while (format[i] != '\0' && stop_scan != -1) {
    skip = 0;
    if (format[i] == '%') {
      if (i - 1 > -1 && isspace(format[i - 1]) != 0) {
        skip = 1;
      }
      s21_memset(&flag, 0, sizeof(flag));
      flag.width = -1;
      s21_memset(&specf, 0, sizeof(specf));
      pasrer(format, &i, &specf, &flag, &stop_scan);
      if (specf.i == 1) {
        scan_i(&str, &specf);
      }
      if (specf.p == 1) {
        specf.x = 1;
      }
      if (specf.d == 1) {
        scan_d(&str, &flag, args, &count_item, &stop_scan, &for_n);
      }
      if (specf.c == 1 || specf.s == 1) {
        scan_s(&str, args, &flag, &count_item, &specf, &stop_scan, &skip,
               &for_n);
      }
      if (specf.f == 1 || specf.G == 1 || specf.g == 1 || specf.E == 1 ||
          specf.e == 1) {
        scan_f(&str, args, &flag, &count_item, &stop_scan, &for_n);
      }
      if (specf.X == 1 || specf.x == 1 || specf.o == 1 || specf.p == 1) {
        scan_hex(&str, &flag, args, &specf, &count_item, &stop_scan, &for_n);
      }
      if (specf.u == 1) {
        scan_u(&str, &flag, args, &count_item, &stop_scan, &for_n);
      }
      if (specf.n == 1) {
        scan_n(args, &flag, &stop_scan, &for_n);
      }
      *result = count_item;
    }
    i++;
  }
}

int pasrer(const char *format, int *index, specif *spec, flgs *fl, int *stop) {
  int end = *index + 1;
  if (format[*index] == '%' && format[*index + 1] == '%')
    *stop = -1;
  else {
    searchwidth(&end, format, fl);
    searchlen(&end, format, fl);
    searchspec(&end, format, spec);
    *index = end;
  }
  return 0;
}

int searchspec(int *index, const char *format, specif *spec) {
  int ret = 1;
  char *s = s21_NULL;
  s = s21_strchr("cdieEfgGosuxXpn", format[*index]);
  if (s != s21_NULL) {
    specifier(spec, s[0]);
  } else
    ret = 0;
  return ret;
}
int searchlen(int *begin, const char *format, flgs *fl) {
  int ret = 1;
  char *s = s21_NULL;
  s = s21_strchr("lLh*", format[*begin]);
  if (s != s21_NULL) {
    flags_scan(fl, format, *begin);
    if (fl->len_l == 1 || fl->len_h == 1 || fl->len_L == 1 || fl->star == 1)
      *begin += 1;
    else if (fl->len_l == 2)
      *begin += 2;
  } else
    ret = 0;
  return ret;
}
int searchwidth(int *begin, const char *format, flgs *fl) {
  int res = 1;
  char buf[32] = {'\0'};
  int i = 0;
  int num = 0;
  while (format[*begin] >= '0' && format[*begin] <= '9') {
    buf[i] = format[*begin];
    i++;
    *begin += 1;
  }
  if (i > 0) {
    num = con_ch_int(buf);
    fl->width = num;
  }
  if (i == 0) res = 0;
  return res;
}

void flags_scan(flgs *fl, const char *ch, int i) {
  switch (ch[i]) {
    case 'h':
      fl->len_h = 1;
      break;
    case 'l':
      fl->len_l = 1;
      if (ch[i + 1] == 'l') fl->len_l++;
      break;
    case 'L':
      fl->len_L = 1;
      break;
    case '*':
      fl->star = 1;
      break;
    default:
      break;
  }
}

void specifier(specif *spec, char ch) {
  switch (ch) {
    case 'c':
      spec->c = 1;
      break;
    case 'd':
      spec->d = 1;
      break;
    case 'i':
      spec->i = 1;
      break;
    case 'e':
      spec->e = 1;
      break;
    case 'E':
      spec->E = 1;
      break;
    case 'f':
      spec->f = 1;
      break;
    case 'g':
      spec->g = 1;
      break;
    case 'G':
      spec->G = 1;
      break;
    case 'o':
      spec->o = 1;
      break;
    case 's':
      spec->s = 1;
      break;
    case 'x':
      spec->x = 1;
      break;
    case 'X':
      spec->X = 1;
      break;
    case 'p':
      spec->p = 1;
      break;
    case 'n':
      spec->n = 1;
      break;
    case 'u':
      spec->u = 1;
      break;
    default:
      break;
  }
}

long long int con_ch_int(const char *str) {
  long long int res;
  int sign;
  unsigned int i;
  res = 0;
  sign = 1;
  i = 0;
  while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r' ||
         str[i] == '\v' || str[i] == '\f' || str[i] == '\x0B')
    i++;
  if (str[i] == '-' || str[i] == '+') {
    if (str[i] == '-') sign = -1;
    i++;
  }
  while (str[i] >= '0' && str[i] <= '9') {
    res = res * 10 + str[i] - '0';
    i++;
  }
  return res * sign;
}

void scan_i(char **src, specif *spec) {
  int offset = 0;
  char buf[512] = {'\0'};
  skip_space(src);
  strcpy(buf, *src);
  if (buf[offset] == '-') {
    offset++;
  }
  if (buf[offset] == '0' && (buf[offset + 1] == 'X' || buf[offset + 1] == 'x'))
    spec->x = 1;
  else if (buf[offset] == '0')
    spec->o = 1;
  else
    spec->d = 1;
}

void scan_d(char **src, flgs *flag, va_list *args, int *count, int *stop_scan,
            int *for_n) {
  char t[512] = {'\0'};
  char b[512] = {'\0'};
  char *temp = t;
  char *buf = b;
  int len = 0;
  long long int d = 0;
  void *res = s21_NULL;
  if (*stop_scan != -1) {
    skip_space(src);
    if (*src[0] == '+') (*src)++;
    strcpy(buf, *src);
    if (buf[0] == '-') len++;
    while (buf[len] != '\0' && is_num(buf[len]) == 1) len++;
    if (len != 0 && (!(len == 1 && buf[0] == '-'))) {
      if (flag->star == 1)
        *src += len;
      else {
        get_char(src, &buf, &temp, &len, flag);
        *for_n = (int)s21_strlen(temp);
        d = con_ch_int(temp);
        if (flag->star != 1) {
          res = va_arg(*args, void *);
          convert_int(d, res, flag);
          *count += 1;
        }
      }
    } else {
      *stop_scan = -1;
      if (s21_strlen(*src) == 0) *count = -1;
    }
  }
}

int is_num(char c) {
  int res = 0;
  if (c >= '0' && c <= '9') res = 1;
  return res;
}

void skip_space(char **src) {
  while (**src != '0' && isspace(**src) != 0) {
    (*src)++;
  }
}

void get_char(char **src, char **buf, char **temp, int *len, flgs *f) {
  if (*len <= f->width || f->width == -1) {
    s21_strncpy(*temp, *buf, *len);
    (*src) += *len;
  } else if (*len >= f->width) {
    s21_strncpy(*temp, *buf, f->width);
    (*src) += f->width;
  }
}

void convert_int(long long int num, void *ptr, flgs *f) {
  if (f->len_l == 1)
    *(long *)ptr = (long int)num;
  else if (f->len_l == 2)
    *(long long *)ptr = num;
  else if (f->len_h == 1)
    *(short *)ptr = (short int)num;
  else
    *(int *)ptr = (int)num;
}

void scan_s(char **str, va_list *args, flgs *flag, int *count, specif *s,
            int *stop_scan, int *skip, int *for_n) {
  char t1[512] = {'\0'};
  int len = 0;
  if (*stop_scan != -1) {
    len = 0;
    if (s->c != 1 || (s->c == 1 && *skip == 1)) skip_space(str);
    if (s->c == 1) {
      if (*str[0] != '\0') len = 1;
    } else if (s->s == 1)
      len = ischar(*str);
    if (len != 0) {
      if (flag->width <= len && flag->width != -1) {
        s21_strncpy(t1, *str, flag->width);
        *str += flag->width;
      } else if (flag->width == -1 || flag->width > len) {
        s21_strncpy(t1, *str, len);
        *str += len;
      }
      *for_n = (int)s21_strlen(t1);
      if (flag->star != 1) {
        copy_string(args, flag, t1, s);
        *count += 1;
      }
    } else {
      *stop_scan = -1;
      *count = -1;
    }
  }
}
void copy_string(va_list *args, flgs *flag, char *t1, specif *s) {
  void *res = va_arg(*args, void *);
  wchar_t temp[1024] = {'\0'};
  if (flag->len_l == 1) {
    mbstowcs(temp, t1, s21_strlen(t1));
    if (s->c == 1)
      *(wchar_t *)res = *(char *)t1;
    else
      wcscpy((wchar_t *)res, temp);
  } else if (flag->len_l == 0) {
    if (s->c == 1)
      *(char *)res = *(char *)t1;
    else
      strcpy((char *)res, t1);
  }
}

int ischar(char *str) {
  int i = 0;
  while (str[i] != '\0' && isspace(str[i]) == 0) i++;
  return i;
}

void scan_f(char **src, va_list *args, flgs *flag, int *count, int *stop_scan,
            int *for_n) {
  char t[512] = {'\0'};
  char b[512] = {'\0'};
  char *temp = t;
  char *buf = b;
  int len = 0;
  long double num = 0;
  void *res = s21_NULL;
  if (*stop_scan != -1) {
    skip_space(src);
    strcpy(buf, *src);
    len = is_float(buf);
    if (!(len == 0 || (len == 1 && buf[0] == '-'))) {
      get_char(src, &buf, &temp, &len, flag);
      *for_n = (int)s21_strlen(temp);
      num = atoi_f(temp);
      if (flag->star != 1) {
        res = va_arg(*args, void *);
        convert_float(num, res, flag);
        *count += 1;
      }
    } else {
      *stop_scan = -1;
      if (s21_strlen(*src) == 0) *count = -1;
    }
  }
}

int is_float(char *str) {
  int i = 0;
  if (str[0] == '-') i++;
  while (str[i] != '\0' && is_num(str[i]) == 1) i++;
  if (str[i] == '.') {
    i++;
    while (str[i] != '\0' && is_num(str[i]) == 1) i++;
  }
  if (str[i] == 'e' || str[i] == 'E') {
    i++;
    if (str[i] == '+')
      i++;
    else if (str[i] == '-')
      i++;
    while (str[i] != '\0' && is_num(str[i]) == 1) i++;
  }
  return i;
}

void convert_float(long double num, void *ptr, flgs *f) {
  if (f->len_l == 1)
    *(double *)ptr = (double)num;
  else if (f->len_l == 2 || f->len_L == 1)
    *(long double *)ptr = num;
  else
    *(float *)ptr = (float)num;
}

long double atoi_f(char *str) {
  long double num = 0;
  int eps = 0;
  int i = 0;
  int s_sign = 1;
  int sign = 1;
  int power = 0;
  if (str[0] == '-') {
    s_sign = -1;
    i = 1;
  }
  while (str[i] != '\0' && is_num(str[i]) == 1) {
    num = num * 10.0 + (str[i] - 48);
    i++;
  }
  if (str[i] == '.') {
    i++;
    while (str[i] != '\0' && is_num(str[i]) == 1) {
      num = num * 10.0 + (str[i] - 48);
      eps--;
      i++;
    }
  }
  if (str[i] == 'e' || str[i] == 'E') {
    i++;
    sign = 1;
    if (str[i] == '+')
      i++;
    else if (str[i] == '-') {
      sign = -1;
      i++;
    }
    power = 0;
    while (str[i] != '\0' && is_num(str[i]) == 1) {
      power = power * 10 + (str[i] - 48);
      i++;
    }
    eps = eps + power * sign;
  }
  while (eps > 0) {
    num = num * 10.0;
    eps--;
  }
  while (eps < 0) {
    num = num * 0.1;
    eps++;
  }
  return num * s_sign;
}

void scan_hex(char **src, flgs *f, va_list *args, specif *spec, int *result,
              int *stop_scan, int *for_n) {
  char t[512] = {'\0'};
  char b[512] = {'\0'};
  char *temp = t;
  char *buf = b;
  int len = 0;
  int offset = 0;
  int sign = 1;
  long long unsigned int num = 0;
  if (*stop_scan != -1) {
    skip_space(src);
    strcpy(buf, *src);
    if (buf[0] == '-') {
      sign = -1;
      offset++;
    }
    only_hex_o(src, buf, spec, &len, &offset);
    if (len != 0) {
      get_char(src, &buf, &temp, &len, f);
      *for_n = (int)s21_strlen(temp);
      num = atoi_hex(temp + offset, spec, &sign);
      if (f->star != 1) {
        conv_hex(f, args, &num, spec);
        *result += 1;
      }
    } else {
      *stop_scan = -1;
      if (s21_strlen(*src) == 0) *result = -1;
    }
  }
}

void only_hex_o(char **src, char *buf, specif *spec, int *len, int *sdvig) {
  if ((spec->x == 1 || spec->X == 1)) {
    if ((buf[*sdvig] == '0' && (buf[1] == 'x' || buf[*sdvig + 1] == 'X')))
      *sdvig += 2;
    strcpy(buf, *src);
    *len += *sdvig;
    while (buf[*len] != '\0' &&
           s21_strchr("0123456789abcdefABCDEF", buf[*len]) != s21_NULL)
      *len += 1;
  } else if (spec->o == 1) {
    if (buf[*sdvig] == '0') *sdvig += 1;
    strcpy(buf, *src);
    *len += *sdvig;
    while (buf[*len] != '\0' && s21_strchr("01234567", buf[*len]) != s21_NULL)
      *len += 1;
  }
}
void conv_hex(flgs *f, va_list *args, long long unsigned int *num,
              specif *spec) {
  void *ptr = va_arg(*args, void *);
  if (f->len_l == 1 || spec->p == 1)
    *(unsigned long int *)ptr = (long int)*num;
  else if (f->len_l == 2)
    *(unsigned long long int *)ptr = *num;
  else if (f->len_h == 1)
    *(unsigned short int *)ptr = (short int)*num;
  else
    *(unsigned int *)ptr = (int)*num;
}

long long unsigned int atoi_hex(char *str, specif *spec, int *sign) {
  long long unsigned int num = 0;
  int i = 0;
  int powd = search_nomatch(str, spec) - 1;
  if (s21_strlen(str) > 8 && (spec->x == 1 || spec->X == 1))
    num = ULLONG_MAX;
  else {
    while (i <= search_nomatch(str, spec) && powd != -1) {
      if (spec->x == 1 || spec->X == 1 || spec->p == 1) {
        if (s21_strchr("ABCDEFabcdef", str[i]) != s21_NULL)
          elseif_16(str, &num, &i, &powd);
        else
          divide_16_8(str, &num, &i, &powd, spec);
      } else if (spec->o == 1)
        divide_16_8(str, &num, &i, &powd, spec);
      i++;
    }
  }
  if (*sign == -1) num = ULLONG_MAX - num + 1;
  return num;
}

void scan_u(char **src, flgs *flag, va_list *args, int *count, int *stop_scan,
            int *for_n) {
  char t[512] = {'\0'};
  char b[512] = {'\0'};
  char *temp = t;
  char *buf = b;
  int len = 0;
  if (*stop_scan != -1) {
    skip_space(src);
    strcpy(buf, *src);
    if (buf[0] == '-') len++;
    while (buf[len] != '\0' && is_num(buf[len]) == 1) len++;
    if (len != 0 && (!(len == 1 && buf[0] == '-'))) {
      if (flag->star == 1)
        *src += len;
      else {
        get_char(src, &buf, &temp, &len, flag);
        *for_n = (int)s21_strlen(temp);
        if (flag->star != 1) {
          conv_ubsigned(args, temp, flag);
          *count += 1;
        }
      }
    } else {
      *stop_scan = -1;
      if (s21_strlen(*src) == 0) *count = -1;
    }
  }
}
void conv_ubsigned(va_list *args, char *temp, flgs *flag) {
  void *ptr = va_arg(*args, void *);
  long long unsigned int num = 0;
  if (con_ch_int(temp) < 0)
    num = ULONG_MAX + con_ch_int(temp) + 1;
  else
    num = con_ch_int(temp);
  if (flag->len_l == 1)
    *(unsigned long *)ptr = (long int)num;
  else if (flag->len_l == 2)
    *(unsigned long long *)ptr = num;
  else if (flag->len_h == 1)
    *(unsigned short *)ptr = (short int)num;
  else
    *(unsigned int *)ptr = (int)num;
}

void scan_n(va_list *args, flgs *flag, int *stop_scan, int *for_n) {
  int *ptr = s21_NULL;
  if (*stop_scan != -1) {
    if (flag->star != 1) {
      ptr = va_arg(*args, int *);
      *ptr = *for_n;
    }
  }
}

void divide_16_8(char *str, long long unsigned int *num, int *i, int *powd,
                 specif *spec) {
  int divide = 0;
  if (spec->o == 1)
    divide = 8;
  else if (spec->X == 1 || spec->x == 1)
    divide = 16;
  *num = *num + ((str[*i] - 48) * pow(divide, *powd));
  *powd -= 1;
}

void elseif_16(char *str, long long unsigned int *num, int *i, int *powd) {
  if (str[*i] == 'a' || str[*i] == 'A') {
    *num = *num + (10 * pow(16, *powd));
    *powd -= 1;
  } else if (str[*i] == 'b' || str[*i] == 'B') {
    *num = *num + (11 * pow(16, *powd));
    *powd -= 1;
  } else if (str[*i] == 'c' || str[*i] == 'C') {
    *num = *num + (12 * pow(16, *powd));
    *powd -= 1;
  } else if (str[*i] == 'd' || str[*i] == 'D') {
    *num = *num + (13 * pow(16, *powd));
    *powd -= 1;
  } else if (str[*i] == 'e' || str[*i] == 'E') {
    *num = *num + (14 * pow(16, *powd));
    *powd -= 1;
  } else if (str[*i] == 'f' || str[*i] == 'F') {
    *num = *num + (15 * pow(16, *powd));
    *powd -= 1;
  }
}

int search_nomatch(const char *str, specif *spec) {
  char match[32] = {'\0'};
  if (spec->o == 1)
    strcpy(match, "01234567");
  else if (spec->X == 1 || spec->x == 1)
    strcpy(match, "ABCDEFabcdef0123456789");
  int i = 0;
  while (str[i] != '\0' && s21_strchr(match, str[i]) != s21_NULL) {
    i++;
  }
  return i;
}

char *strcpy(char *dest, const char *src) {
  int i = 0;
  for (; i < (int)s21_strlen(src); i++) {
    dest[i] = src[i];
  }
  dest[i] = '\0';
  return dest;
}
