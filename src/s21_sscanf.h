#ifndef S21_SSCANF_H_
#define S21_SSCANF_H_

#include "s21_string.h"

typedef struct {
  int c;
  int d;
  int i;
  int e;
  int E;
  int f;
  int g;
  int G;
  int o;
  int s;
  int u;
  int x;
  int X;
  int p;
  int n;
} specif;

typedef struct {
  int star;
  int width;
  int len_h;
  int len_l;
  int len_L;
} flgs;

int s21_sscanf(const char *str, const char *format, ...);
void scan_specifier(const char *format, const char *src, int *result,
                    va_list *args);
int pasrer(const char *format, int *index, specif *spec, flgs *fl, int *stop);
int searchwidth(int *begin, const char *format, flgs *fl);
int searchlen(int *begin, const char *format, flgs *fl);
int searchspec(int *index, const char *format, specif *spec);
long long int con_ch_int(const char *str);
void flags_scan(flgs *fl, const char *ch, int i);
void specifier(specif *spec, char ch);
void scan_i(char **src, specif *spec);
void scan_d(char **src, flgs *flag, va_list *args, int *count, int *stop_scan,
            int *for_n);
int is_num(char c);
void skip_space(char **src);
void get_char(char **src, char **buf, char **temp, int *len, flgs *f);
void convert_int(long long int num, void *ptr, flgs *f);
void scan_s(char **str, va_list *args, flgs *flag, int *count, specif *s,
            int *stop_scan, int *skip, int *for_n);
void copy_string(va_list *args, flgs *flag, char *t1, specif *s);
int ischar(char *str);
void scan_f(char **src, va_list *args, flgs *flag, int *count, int *stop_scan,
            int *for_n);
int is_float(char *str);
long double atoi_f(char *str);
void convert_float(long double num, void *ptr, flgs *f);
void scan_hex(char **src, flgs *f, va_list *args, specif *spec, int *result,
              int *stop_scan, int *for_n);
void only_hex_o(char **src, char *buf, specif *spec, int *len, int *sdvig);
long long unsigned int atoi_hex(char *str, specif *spec, int *sign);
void conv_hex(flgs *f, va_list *args, long long unsigned int *num,
              specif *spec);
int search_nomatch(const char *str, specif *spec);
void divide_16_8(char *str, long long unsigned int *num, int *i, int *powd,
                 specif *spec);
void elseif_16(char *str, long long unsigned int *num, int *i, int *powd);
void scan_u(char **src, flgs *flag, va_list *args, int *count, int *stop_scan,
            int *for_n);
void conv_ubsigned(va_list *args, char *temp, flgs *flag);
void scan_n(va_list *args, flgs *flag, int *stop_scan, int *for_n);
char *strcpy(char *dest, const char *src);

#endif
