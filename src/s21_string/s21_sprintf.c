#include "../s21_sprintf.h"

int s21_sprintf(char *str, const char *format, ...) {
  flags flag = {0};
  va_list ap;
  va_start(ap, format);
  char *str_start = str;

  while (*format) {
    if (*format == '%') {
      format++;
      s21_memset(&flag, 0, sizeof(flags));
    } else {
      *str++ = *format++;
      continue;
    }
    format = Flags(format, &flag);
    format = Width(format, &flag, ap);
    format = Precision(format, &flag, ap);
    format = Length(format, &flag);

    flag.specifier = *format;
    format++;

    char buf[BUFF_SIZE] = {'\0'};
    Parser_Process(flag, buf, ap);
    for (int i = 0; buf[i]; i++, str++) *str = buf[i];

    if (flag.specifier == 'n') {
      int *ret = va_arg(ap, int *);
      *ret = str - str_start;
    }
  }
  *str = '\0';
  va_end(ap);

  return str - str_start;
}

const char *Flags(const char *format,
                  flags *flag) {  // Извлечениие флагов из форматной строки.
  while (*format == '-' || *format == '+' || *format == ' ' || *format == '#' ||
         *format == '0') {
    switch (*format) {
      case '-':
        flag->minus = 1;
        break;
      case '+':
        flag->plus = 1;
        break;
      case ' ':
        flag->space = 1;
        break;
      case '#':
        flag->hash = 1;
        break;
      case '0':
        flag->zero = 1;
        break;
    }
    format++;
  }
  return format;
}

const char *Width(const char *format, flags *flag,
                  va_list ap) {  // Извлечение ширины из форматной строки.
  if (*format == '*') {
    format++;
    flag->width = va_arg(ap, int);
  } else if (s21_isdigit(*format)) {
    char tmp[BUFF_SIZE] = {'\0'};
    for (int i = 0; s21_isdigit(*format); i++, format++) {
      tmp[i] = *format;
    }
    flag->width = s21_atoi(tmp);
  }
  return format;
}

const char *Precision(const char *format, flags *flag,
                      va_list ap) {  // Извлечение точности из форматной строки.
  if (*format == '.') {
    flag->is_precision_set = 1;
    format++;
  }
  if (*format == '*') {
    format++;
    flag->precision = va_arg(ap, int);
  }
  if (s21_isdigit(*format)) {
    char tmp[BUFF_SIZE] = {'\0'};
    for (int i = 0; s21_isdigit(*format); i++, format++) tmp[i] = *format;
    flag->precision = s21_atoi(tmp);
  }
  return format;
}

const char *Length(const char *format,
                   flags *flag) {  // Извлечение длины из форматной строки.
  switch (*format) {
    case 'h':
      flag->length = 'h';
      format++;
      break;
    case 'l':
      flag->length = 'l';
      format++;
      break;
    case 'L':
      flag->length = 'L';
      format++;
      break;
  }
  return format;
}

void Parser_Process(
    flags flag, char *buf,
    va_list ap) {  // Обработка значения с учетом спецификатора формата.
  if (flag.specifier == 'd' || flag.specifier == 'i')
    Parser_integer(
        flag, buf,
        ap);  // Обработка целочисленного значения для спецификаторов 'd' и 'i'.
  else if (flag.specifier == 'u')
    Parser_unsigned(flag, buf, ap);  // Обработка беззнакового целочисленного
                                     // значения для спецификатора 'u'
  else if (flag.specifier == 'o')
    Parser_octalS(flag, buf, ap);  // Обработка восьмеричного целочисленного
                                   // значения для спецификатора 'o'
  else if (flag.specifier == 'x' || flag.specifier == 'X')
    Parser_hexadecimal(flag, buf,
                       ap);  // Обработка шестнадцатеричного целочисленного
                             // значения для спецификаторов 'х' и 'Х'
  else if (flag.specifier == '%')
    buf[0] = '%';
  else if (flag.specifier == 'c')
    Parser_char(flag, buf,
                ap);  // Обработка символьного значения для спецификатора 'c'
  else if (flag.specifier == 's')
    Parser_stroke(flag, buf,
                  ap);  // Обработка строки символов для спецификатора 's'
  else if (flag.specifier == 'p')
    Parser_pointer(flag, buf, ap);  // Обработка указателя для спецификатора 'p'
  else if (flag.specifier == 'f')
    Parser_float(
        flag, buf,
        ap);  // Обработка числа с плавающей точкой для спецификатора 'f'
  else if (flag.specifier == 'e' || flag.specifier == 'E')
    Parser_mantiss(flag, buf, ap);  // Обработка мантиссы числа с плавающей
                                    // точкой для спецификатора 'e' или 'E'
  else if (flag.specifier == 'g' || flag.specifier == 'G')
    Parser_float_Gg(flag, buf, ap);  // Обработка числа с плавающей точкой для
                                     // спецификатора 'g' или 'G'
  if (flag.specifier == 'X' || flag.specifier == 'G' || flag.specifier == 'E')
    to_upper(buf);
}

void Parser_integer(flags flag, char *buf,
                    va_list ap) {  // Обработка целочисленного значения для
                                   // спецификаторов 'd' и 'i'
  int64_t val = va_arg(ap, int64_t);

  switch (flag.length) {
    case 0:
      val = (int32_t)val;
      break;
    case 'h':
      val = (int16_t)val;
  }
  Convert_Num_to_Str(val, buf, 10);
  Precision_check(buf, flag);  // Форматирование строки с учетом точности. Эта
                               // функция обрабатывает строку, учитывая
                               // точность, и обновляет её содержимое
  Flags_check(buf, flag);  // Форматирование строки с учетом флагов
}

void Convert_Num_to_Str(
    int64_t val, char *val_res,
    int base) {  // Преобразование целого числа в строку. helper
  char tmp[BUFF_SIZE] = {'\0'};
  int ind = BUFF_SIZE - 2;

  int neg = val < 0 ? 1 : 0;
  val = neg ? -val : val;

  if (val == 0) tmp[ind] = '0';

  while (val > 0) {
    ind--;
    tmp[ind] = "0123456789abcdef"[val % base];
    val /= base;
  }
  for (int j = 0; tmp[ind]; ind++, j++) {
    if (neg && j == 0) val_res[j++] = '-';

    val_res[j] = tmp[ind];
  }
}

void Precision_check(char *buf,
                     flags flag) {  // Форматирование строки с учетом точности.
                                    // Эта функция обрабатывает строку, учитывая
                                    // точность, и обновляет её содержимое
  char tmp[BUFF_SIZE] = {'\0'};

  int sign = 0;
  int len = s21_strlen(buf);

  if (buf[0] == '-') {
    tmp[0] = '-';
    len--;
    sign = 1;
  }

  if (flag.precision > len) {
    int ind;
    for (ind = sign; ind < flag.precision - len + sign; ind++) tmp[ind] = '0';

    for (int i = sign; buf[i]; i++, ind++) tmp[ind] = buf[i];

    s21_strcpy(buf, tmp);
  }

  if (flag.is_precision_set && flag.precision == 0 &&
      Check_int(flag.specifier) && buf[0] == '0')
    buf[0] = '\0';
}

int Check_int(char c) {  // Проверка, является ли символ спецификатором
                         // целочисленного типа
  char specs[] = {'d', 'i', 'o', 'u', 'x', 'X'};
  int res = 0;
  for (s21_size_t i = 0; i < sizeof(specs); i++) {
    if (specs[i] == c) {
      res = 1;
      break;
    }
  }
  return res;
}

void Flags_check(char *buf,
                 flags flag) {  // Форматирование строки с учетом флагов
  char tmp[BUFF_SIZE + 1] = {'\0'};
  if (flag.plus && flag.specifier != 'u') {
    tmp[0] = buf[0] == '-' ? buf[0] : '+';
    s21_strcpy(tmp + 1, buf[0] == '-' ? buf + 1 : buf);
    s21_strcpy(buf, tmp);
  }

  else if (flag.space && buf[0] != '-' && flag.specifier != 'u') {
    tmp[0] = ' ';
    s21_strcpy(tmp + 1, buf);
    s21_strcpy(buf, tmp);
  }

  if (flag.width > (int)s21_strlen(buf)) {
    int ind = flag.width - s21_strlen(buf);
    if (!flag.minus) {
      s21_memset(tmp, flag.zero ? '0' : ' ', ind);
      s21_strcpy(tmp + ind, buf);
    } else {
      s21_strcpy(tmp, buf);
      s21_memset(tmp + s21_strlen(tmp), ' ', ind);
    }
    s21_strcpy(buf, tmp);
  }
}

void Unsigned_Num_to_Str(
    uint64_t val, char *val_res,
    int base) {  // Преобразование беззнакового числа в строку
  char buf[BUFF_SIZE + 1] = {'\0'};
  int ind = BUFF_SIZE - 1;
  if (val == 0) {
    buf[ind] = '0';
    ind--;
  }

  for (; val && ind; --ind, val /= base)
    buf[ind] = "0123456789abcdef"[val % base];

  for (int j = 0; buf[ind + 1]; ind++, j++) val_res[j] = buf[ind + 1];
}

void Parser_unsigned(flags flag, char *buf,
                     va_list ap) {  // Обработка беззнакового целочисленного
                                    // значения для спецификатора 'u'.
  uint64_t val = va_arg(ap, uint64_t);
  switch (flag.length) {
    case 'h':
      val = (uint16_t)val;
      break;
    case 'l':
      val = (uint64_t)val;
      break;
    case 0:
      val = (uint32_t)val;
      break;
  }
  Unsigned_Num_to_Str(val, buf, 10);
  Precision_check(buf, flag);
  Flags_check(buf, flag);
}

void Parser_octalS(flags flag, char *buf,
                   va_list ap) {  // Обработка восьмеричного целочисленного
                                  // значения для спецификатора 'o'
  buf[0] = '0';
  Convert_Num_to_Str(va_arg(ap, int64_t), buf + flag.hash, 8);
  Precision_check(buf, flag);
  Flags_check(buf, flag);
}

void to_upper(char *str) {  // Преобразование символа в верхний регистр
  while (*str) {
    if (*str >= 'a' && *str <= 'z') *str = *str - 'a' + 'A';
    str++;
  }
}

int Check_zeroes(char *buf) {  // Проверка, состоит ли строка только из нулей
  int k = 1;
  for (int i = 0; buf[i]; i++)
    if (buf[i] != '0') {
      k = 0;
      break;
    }
  return k;
}

void Parser_hexadecimal(
    flags flag, char *buf,
    va_list ap) {  // Обработка шестнадцатеричного целочисленного значения для
                   // спецификаторов 'х' и 'Х'
  uint64_t val = va_arg(ap, uint64_t);
  switch (flag.length) {
    case 0:
      val = (uint32_t)val;
      break;
    case 'h':
      val = (uint16_t)val;
      break;
    case 'l':
      val = (uint64_t)val;
      break;
  }
  Unsigned_Num_to_Str(val, buf, 16);
  Precision_check(buf, flag);
  if (flag.hash) {
    ADD_ox(buf, flag);
  }
  Flags_check(buf, flag);
}

void ADD_ox(
    char *buf,
    flags flag) {  // Добавление префикса '0x' к строке, если это необходимо
  if (!Check_zeroes(buf) || flag.specifier == 'p') {
    s21_memmove(buf + 2, buf, s21_strlen(buf));
    buf[0] = '0';
    buf[1] = 'x';
  }
}

void Parser_char(
    flags flag, char *buf,
    va_list ap) {  // Обработка символьного значения для спецификатора 'c'
  if (flag.length == 'l') {
    wchar_t w_c;
    w_c = va_arg(ap, wchar_t);
    Wide_Char_Check(flag, buf, w_c);
  } else {
    char c;
    c = va_arg(ap, int);
    Char_Check(flag, buf, c);
  }
}

void Wide_Char_Check(
    flags flag, char *buf,
    wchar_t w_c) {  // Форматирование широкого символа для спецификатора 'lc'.
  if (!flag.minus && flag.width) {
    char tmp[BUFF_SIZE] = {'\0'};

    wcstombs(tmp, &w_c, BUFF_SIZE);
    for (s21_size_t i = 0; i < flag.width - s21_strlen(tmp); i++) buf[i] = ' ';
    s21_strcat(buf, tmp);
  } else if (flag.width) {
    wcstombs(buf, &w_c, BUFF_SIZE);
    for (int i = s21_strlen(buf); i < flag.width; i++) buf[i] = ' ';
  } else {
    wcstombs(buf, &w_c, BUFF_SIZE);
  }
}

void Char_Check(flags flag, char *buf,
                char c) {  // Форматирование символа для спецификатора 'c'
  if (!flag.minus && flag.width) {
    for (int i = 0; i < flag.width; i++) {
      buf[i] = ' ';
      if (i == flag.width - 1) buf[i] = c;
    }
  } else if (flag.width) {
    buf[0] = c;
    for (int i = 1; i < flag.width; i++) buf[i] = ' ';
  } else {
    buf[0] = c;
  }
}

void Parser_stroke(
    flags flag, char *buf,
    va_list ap) {  // Обработка строки символов для спецификатора 's'
  if (flag.length == 'l') {
    wchar_t *wstr = va_arg(ap, wchar_t *);
    Wide_Stroke_Check(flag, buf, wstr);
  } else {
    char *str = va_arg(ap, char *);
    Check_stroke(flag, buf, str);
  }
}

void Check_stroke(
    flags flag, char *buf,
    char *str) {  // Форматирование строки символов для спецификатора 's'
  char tmp[BUFF_SIZE] = {'\0'};
  s21_strcpy(tmp, str);
  if (flag.is_precision_set) tmp[flag.precision] = '\0';

  int shift = flag.width - s21_strlen(tmp);
  int len = s21_strlen(tmp);

  if (flag.minus && shift > 0) {
    s21_strcpy(buf, tmp);
    s21_memset(buf + len, ' ', shift);
  } else if (shift > 0) {
    s21_memset(buf, ' ', shift);
    s21_strcpy(buf + shift, tmp);
  } else {
    s21_strcpy(buf, tmp);
  }
}

void Wide_Stroke_Check(flags flag, char *buf,
                       wchar_t *wstr) {  // Форматирование широкой строки
                                         // символов для спецификатора 'ls'
  char tmp[BUFF_SIZE] = {'\0'};
  char str[BUFF_SIZE] = {'\0'};

  wcstombs(str, wstr, BUFF_SIZE);
  s21_strcpy(tmp, str);
  if (flag.is_precision_set) tmp[flag.precision] = '\0';

  int shift = flag.width - s21_strlen(tmp);
  int len = s21_strlen(tmp);

  if (flag.minus && shift > 0) {
    s21_strcpy(buf, tmp);
    s21_memset(buf + len, ' ', shift);
  } else if (shift > 0) {
    s21_memset(buf, ' ', shift);
    s21_strcpy(buf + shift, tmp);
  } else {
    s21_strcpy(buf, tmp);
  }
}

void Parser_pointer(flags flag, char *buf,
                    va_list ap) {  // Обработка указателя для спецификатора 'p'
  Unsigned_Num_to_Str(va_arg(ap, uint64_t), buf, 16);
  Precision_check(buf, flag);
  ADD_ox(buf, flag);
  Flags_check(buf, flag);
}

void Parser_float(
    flags flag, char *buf,
    va_list ap) {  // Обработка числа с плавающей точкой для спецификатора 'f'
  long double val = 0;
  if (flag.length == 'L') {
    val = va_arg(ap, long double);
  } else {
    val = va_arg(ap, double);
  }

  if (!flag.is_precision_set) {
    flag.precision = 6;
  }

  Format_string(val, buf, flag);
  Flags_check(buf, flag);
}

void Format_string(long double val, char *val_res,
                   flags flag) {  // Форматирование числа с плавающей точкой для
                                  // спецификатора 'e' или 'E
  char temp[BUFF_SIZE] = {
      '\0'};  // временный массив для хранения отформатированного числа
  int index = BUFF_SIZE - 2;
  int neg = val < 0 ? 1 : 0;
  val = neg ? val * -1 : val;
  long double l = 0, r = modfl(val, &l);
  if (flag.precision == 0) {
    l = roundl(val);
    r = 0;
  }
  char fractions[BUFF_SIZE] = {
      '\0'};  // don not understand for what this's here
  for (int i = 0; i < flag.precision; i++) {
    r = r * 10;
    fractions[i] = (int)r + '0';
  }
  long long right = roundl(r), left = l;
  if (!right) {
    for (int i = 0; i < flag.precision; index--, i++) temp[index] = '0';
  } else {
    for (int i = s21_strlen(fractions); right || i > 0;
         right /= 10, index--, i--)
      temp[index] = (int)(right % 10 + 0.05) + '0';
  }

  if ((flag.is_precision_set && flag.precision != 0) || (int)r ||
      (!flag.is_precision_set && val == 0) || s21_strlen(fractions)) {
    temp[index--] = '.';
  }

  if (!left) {
    temp[index] = '0';
    index--;
  } else {
    for (; left; left /= 10, index--) temp[index] = (int)(left % 10) + '0';
  }
  for (int i = 0; temp[index + 1]; index++, i++) {
    if (neg && i == 0) {
      val_res[i] = '-';
      i++;
    }
    val_res[i] = temp[index + 1];
  }
}

void Parser_mantiss(flags flag, char *buf,
                    va_list ap) {  // Обработка мантиссы числа с плавающей
                                   // точкой для спецификатора 'e' или 'E'
  long double val = 0;
  if (flag.length == 'L') {
    val = va_arg(ap, long double);
  } else {
    val = va_arg(ap, double);
  }
  int pow = 0;
  char sign = (int)val == 0 ? '-' : '+';

  if ((int)val - val) {
    while ((int)val == 0) {
      pow++;
      val *= 10;
    }
  } else {
    sign = '+';
  }
  while ((int)val / 10 != 0) {
    pow++;
    val /= 10;
  }

  if (!flag.is_precision_set) flag.precision = 6;
  Format_string(val, buf, flag);
  ADD_mantiss(buf, pow, sign);
  Flags_check(buf, flag);
}

void ADD_mantiss(char *str, int pow,
                 char sign) {  // Добавление мантиссы к отформатированному числу
                               // с плавающей запятой
  int len = s21_strlen(str);
  str[len] = 'e';
  str[len + 1] = sign;
  str[len + 3] = pow % 10 + '0';
  pow /= 10;
  str[len + 2] = pow % 10 + '0';
  str[len + 4] = '\0';
}

void Parser_float_Gg(flags flag, char *buf,
                     va_list ap) {  // Обработка числа с плавающей точкой для
                                    // спецификатора 'g' или 'G'
  long double val = 0;
  if (flag.length == 'L') {
    val = va_arg(ap, long double);
  } else {
    val = va_arg(ap, double);
  }

  if (!flag.is_precision_set) {
    flag.precision = 6;
  }
  if (flag.precision == 0) flag.precision = 1;
  int precision = flag.precision;
  long double m_val = val;
  int pow = 0;
  if ((int)val - val) {
    while ((int)m_val == 0) {
      pow++;
      m_val *= 10;
    }
  }
  if (pow > 4) {
    flag.precision = 0;
    Format_string(m_val, buf, flag);
  } else {
    flag.precision = 10;
    Format_string(val, buf, flag);
  }
  Check_gG_precision(buf, precision);
  if (pow > 4) ADD_mantiss(buf, pow, '-');
  Remove_Zeroes(buf);
  Flags_check(buf, flag);
}

void Remove_Zeroes(char *buf) {  // Удаление завершающих нулей из строки
  int len = s21_strlen(buf);
  char *dot = s21_strchr(buf, '.');
  if (dot) {
    for (int i = len - 1; buf[i] != '.'; i--) {
      if (buf[i] == '0')
        buf[i] = '\0';
      else
        break;
    }
    if (dot[1] == '\0') dot[0] = '\0';
  }
}

void Check_gG_precision(
    char *buf,
    int precision) {  // Форматирование числа для спецификатора 'g' или 'G'
  int sig_digs = 0;
  s21_size_t len = s21_strlen(buf);
  int not_zero_found = 0;

  for (s21_size_t i = 0; i < s21_strlen(buf); i++) {
    if ((buf[i] == '0' && !not_zero_found) || buf[i] == '.')
      continue;
    else
      not_zero_found = 1;

    if (s21_isdigit(buf[i]) && not_zero_found) {
      sig_digs++;
    }
    if (sig_digs == precision && i + 1 < len) {
      int next = buf[i + 1] == '.' ? 2 : 1;
      buf[i] = buf[i + next] - '0' > 5 ? (char)(buf[i] + 1) : buf[i];
      buf[i + 1] = '\0';
      break;
    }
  }
}

int s21_isdigit(char c) {
  return (c >= '0' && c <= '9');
}  // Проверка, является ли символ цифрой
