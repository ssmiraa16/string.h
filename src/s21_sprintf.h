#ifndef SRC_S21_SPRINTF_H_
#define SRC_S21_SPRINTF_H_

#include "s21_string.h"

typedef struct {
  int minus;
  int plus;
  int space;
  int zero;
  int hash;
  int precision;
  int is_precision_set;
  int width;
  char length;
  char specifier;
} flags;

int s21_sprintf(char *str, const char *format, ...);
const char *Flags(const char *format, flags *f);
const char *Width(const char *format, flags *f, va_list ap);
const char *Precision(const char *format, flags *f, va_list ap);
const char *Length(const char *format, flags *f);
void Parser_Process(flags f, char *buf, va_list ap);
void Parser_integer(flags f, char *buf, va_list ap);
void Parser_unsigned(flags f, char *buf, va_list ap);
void Parser_octalS(flags f, char *buf, va_list ap);
void Parser_hexadecimal(flags f, char *buf, va_list ap);
void Parser_char(flags f, char *buf, va_list ap);
void Parser_stroke(flags f, char *buf, va_list ap);
void Parser_pointer(flags f, char *buf, va_list ap);
void Parser_float(flags f, char *buf, va_list ap);
void Parser_mantiss(flags f, char *buf, va_list ap);
void Parser_float_Gg(flags f, char *buf, va_list ap);
void Precision_check(char *buf, flags f);
void Flags_check(char *buf, flags f);
void Wide_Char_Check(flags f, char *buf, wchar_t w_c);
void Char_Check(flags f, char *buf, char c);
void Check_stroke(flags f, char *buf, char *str);
void Wide_Stroke_Check(flags f, char *buf, wchar_t *wstr);
void Check_gG_precision(char *buf, int precision);
void Convert_Num_to_Str(int64_t val, char *val_res, int base);
int Check_int(char c);
void Unsigned_Num_to_Str(uint64_t val, char *val_res, int base);
void to_upper(char *str);
int Check_zeroes(char *buf);
void ADD_ox(char *buf, flags f);
void Format_string(long double val, char *val_res, flags f);
void ADD_mantiss(char *str, int pow, char sign);
void Remove_Zeroes(char *buf);
int s21_isdigit(char c);

#endif  //  SRC_S21_SPRINTF_H_
