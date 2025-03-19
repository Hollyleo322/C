#pragma once
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_errors.h"
typedef unsigned long s21_size_t;
#define START_LENGTH 20
#define s21_NULL (void *)0
#define S21_NULL (void *)0
#define N 20

void *s21_memchr(const void *arr, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dst, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
char *s21_strpbrk(const char *str1, const char *str2);
s21_size_t s21_strlen(const char *data);
int s21_strcmp(char *a, char *b);
char *s21_strcpy(char *dst, const char *src);
char *s21_strcat(char *dst, char *src);
char *s21_strchr(const char *src, int c);
char *s21_strstr(const char *str, const char *substr);
char *s21_strtok(char *str, const char *substr);
void *s21_to_upper(const char *str);
char *s21_strrchr(const char *str, int c);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
char *s21_trim(const char *src, const char *trim_chars);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
typedef struct another_spec {
  char minus;
  char plus;
  char space;
  int width;
  int accuracy;
  char flag_accuracy;
  char flag_width;
  char lenght_h;
  char lenght_l;
  char flag_e;
  int count_e;
  char flag_e_sign;
  char upper;
  char flag_g;
  char flag_g_e;
  char flag_x;
  char flag_o;
  char flag_p;
  char flag_error;
  char flag_sharp;
  char flag_zero;
  char big_l;
  char star_width;
  char flag_u;
  char flag_minus_f;
} another_spec;
int s21_sprintf(char *buf, const char *format, ...);
const char *dividedstrspec(char **buf, const char *format, va_list *ap);
void setflags(another_spec *flags, const char *ptr_start, const char *format,
              va_list *ap);
int setnumber(const char **ptr, long int quantity);
void probablywidth(const char **ptr, long int quantity, another_spec *flags);
void printerror();
void chooseformat(char **buf, const char *format, another_spec *flags,
                  va_list *ap);
void input_c(char **buf, another_spec *flags, va_list *ap);
void input_d(char **buf, another_spec *flags, va_list *ap);
char *int_to_str(long int number, int *count);
void do_accuracy(char **buf, another_spec *flags, char *res, int count_lenght_d,
                 long int number);
void do_width(char **buf, another_spec *flags, char *res, int count_lenght_d,
              long int number);
void write_some_in_str(char **buf, char *res, int lenght);
void write_space_in_str(char **buf, int lenght, another_spec *flags);
void put_signes(char **buf, another_spec *flags, long int number);
void do_main_d(char **buf, another_spec *flags, char *res, int count_lenght_d,
               long int number);
void input_f(char **buf, another_spec *flags, va_list *ap);
char *divide_float_number(another_spec *flags, long double number,
                          long int *before_dot, int lenght, int *count_after);
void write_zero_in_str(char **buf, int lenght);
void do_width_f(char **buf, another_spec *flags, char *array_after_dot,
                char *array_before_dot, long double number, int count_after,
                int count_before);
void do_main_f(char **buf, another_spec *flags, char *array_after_dot,
               char *array_before_dot, long double number, int count_after,
               int count_before);
void input_s(char **buf, another_spec *flags, va_list *ap);
void do_width_s(char **buf, another_spec *flags, char *res);
void write_some_in_str_s(char **buf, char *res, int lenght);
void input_u(char **buf, another_spec *flags, va_list *ap);
void put_signes_f(char **buf, another_spec *flags, long double number);
void write_exp_of_e(char **buf, another_spec *flags);
int realize_e(long double *number, another_spec *flags);
void write_some_g_flag(char **buf, another_spec *flags, char *array_before_dot,
                       char *array_after_dot, int count_after,
                       int count_before);
void check_what_to_do(long double *number, another_spec *flags);
char *get_8_system(unsigned long int number, int *count_leght,
                   another_spec *flags);
char *get_16_system(unsigned long int number, int *count_leght,
                    another_spec *flags);
void set_star(another_spec *flags, va_list *ap);
void no_end_number(char **buf, double number, another_spec *flags);
char *int_to_str_u(unsigned long number, int *count);
void round_g(char *res, int count_after);
int counting_space(int accuracy, int before, int after, char *array);
void do_width_minus(char **buf, another_spec *flags, char *res,
                    int count_lenght_d, long int number);
void do_width_plus(char **buf, another_spec *flags, char *res,
                   int count_lenght_d, long int number);
void do_width_f_plus(char **buf, another_spec *flags, char *array_after_dot,
                     char *array_before_dot, long double number,
                     int count_after, int count_before);
void do_width_f_minus(char **buf, another_spec *flags, char *array_after_dot,
                      char *array_before_dot, long double number,
                      int count_after, int count_before);
char *get_null_p(int *length);

typedef struct specs {
  unsigned long width;
  char flag_o;
  char flag_x;
  char negative;
  char flag_width;
  char flag_star;
  char lenght_h;
  char lenght_l;
  char big_l;
  char flag_error;
  char flag_p;
  char flag_do;
  char flag_space;
  char inf;
  char nan;
} specs;

int s21_sscanf(const char *buf, const char *format, ...);
const char *dividestrspec(const char *format, va_list *ap, int *res,
                          const char **buf, int *res_star, char *flag_space,
                          const char *start);
void setflags_scanf(specs *flags, const char *ptr_start, const char *format);
void probablywidth_scanf(const char **ptr, specs *flags);
long setnumber_scanf(const char **ptr, specs *flags);
void chooseformat_scanf(const char **buf, const char *format, specs *flags,
                        va_list *ap, int *res, int *res_star, char *flag_space,
                        const char *start);
void write_c(const char **buf, va_list *ap, char *flag_space, specs *flags);
void write_d(long number, va_list *ap, specs *flags);
long do_d(const char **buf, specs *flags);
void move_buf_for_d(const char **buf, specs *flags);
void write_u(long number, va_list *ap, specs *flags);
void move_buf_for_s(const char **buf);
void write_s(const char **buf, va_list *ap, specs *flags);
void move_buf_for_x(const char **buf, specs *flags);
long setnumber_width_x(const char **ptr, unsigned long width);
long setnumber_x(const char **ptr);
long do_x(const char **buf, specs *flags);
void write_double(long double number, va_list *ap, specs *flags);
long double do_float(const char **buf, specs *flags);
void move_buf_for_float(const char **buf, specs *flags);
long double str_to_float(const char *str);
void sci_to_normal(const char *sci, char *normal);
void printerror_scanf();
void move_buf_for_c(const char **buf);
int check_step(const char *ptr, int step);
int check_start_d(const char *buf);
void move_for_percent(const char **buf);
int checkeof(const char *buf);
void write_n(const char **buf, va_list *ap, const char *start);
int checknull_p(const char *buf);
