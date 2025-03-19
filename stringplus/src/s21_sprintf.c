#include "s21_string.h"
int s21_sprintf(char *buf, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  char *begunok = buf;
  for (; *format; format++) {
    if (*format == '%') {
      if (*(format + 1) == '%') {
        *begunok = '%';
        begunok++;
        format += 1;
        continue;
      }
      format = dividedstrspec(&begunok, format++, &ap);
    } else {
      *begunok = *format;
      begunok++;
    }
  }
  *begunok = '\0';
  va_end(ap);
  return s21_strlen(buf);
}

const char *dividedstrspec(char **buf, const char *format, va_list *ap) {
  another_spec flags = {0};
  char *a = "cdfsueEgGoxXp";
  const char *ptr_start = format + 1;
  while (!s21_strchr(a, *format)) {
    format++;
  }
  setflags(&flags, ptr_start, format, ap);
  chooseformat(buf, format, &flags, ap);
  return format++;
}
void setflags(another_spec *flags, const char *ptr_start, const char *format,
              va_list *ap) {
  char *a = "cdfsuoxXgGeE";
  char *b = "fgGeE";
  while (ptr_start != format) {
    switch (*ptr_start) {
      case '-':
        flags->minus = 1;
        break;
      case '+':
        flags->plus = 1;
        break;
      case ' ':
        if (!flags->plus)
          flags->space = 1;
        else
          flags->flag_error = 1;
        break;
      case '#':
        flags->flag_sharp = 1;
        break;
      case '*':
        flags->star_width = 1;
        set_star(flags, ap);
        break;
      case '0':
        if (!flags->minus) {
          flags->flag_zero = 1;
        } else {
          flags->flag_error = 1;
        }
        break;
      case '.':
        flags->flag_accuracy = 1;
        ptr_start += 1;
        if (*ptr_start == '*') {
          set_star(flags, ap);
        } else {
          flags->accuracy = setnumber(&ptr_start, format - ptr_start);
        }
        break;
      case 'h':
        if (s21_strchr(a, *(ptr_start + 1)) && !flags->lenght_l &&
            !flags->big_l) {
          flags->lenght_h = 1;
        } else {
          flags->flag_error = 1;
        }
        break;
      case 'L':
        if (s21_strchr(b, *(ptr_start + 1)) && !flags->lenght_l &&
            !flags->lenght_h) {
          flags->big_l = 1;
        } else {
          flags->flag_error = 1;
        }
        break;
      case 'l':
        if (s21_strchr(a, *(ptr_start + 1)) && !flags->lenght_h &&
            !flags->big_l) {
          flags->lenght_l = 1;
        } else {
          flags->flag_error = 1;
        }
        break;
      default:
        probablywidth(&ptr_start, format - ptr_start, flags);
    }
    ptr_start++;
    if (flags->flag_error) {
      printerror();
    }
  }
}
int setnumber(const char **ptr, long int quantity) {
  const char *count_quantity = *ptr;
  int step = -1;
  while ((*count_quantity) >= '0' && (*count_quantity) <= '9') {
    step += 1;
    count_quantity += 1;
  }
  quantity = count_quantity - *ptr;
  int res = 0;
  for (int i = 0; i < quantity; i++, (*ptr)++, step--) {
    res += ((**ptr) - '0') * pow(10, step);
  }
  (*ptr)--;
  return res;
}
void probablywidth(const char **ptr, long int quantity, another_spec *flags) {
  if ((**ptr) >= '1' && (**ptr) <= '9') {
    flags->flag_width = 1;
    flags->width = setnumber(ptr, quantity);
  } else {
    printerror();
  }
}
void printerror() {
  printf("incorrect input of spec\n");
  exit(1);
}
void chooseformat(char **buf, const char *format, another_spec *flags,
                  va_list *ap) {
  switch (*format) {
    case 'c':
      input_c(buf, flags, ap);
      break;
    case 'd':
      input_d(buf, flags, ap);
      break;
    case 'f':
      input_f(buf, flags, ap);
      break;
    case 's':
      input_s(buf, flags, ap);
      break;
    case 'u':
      input_u(buf, flags, ap);
      break;
    case 'e':
      flags->flag_e = 1;
      input_f(buf, flags, ap);
      break;
    case 'E':
      flags->flag_e = 1;
      flags->upper = 1;
      input_f(buf, flags, ap);
      break;
    case 'g':
      flags->flag_g = 1;
      input_f(buf, flags, ap);
      break;
    case 'G':
      flags->flag_g = 1;
      flags->upper = 1;
      input_f(buf, flags, ap);
      break;
    case 'o':
      flags->flag_o = 1;
      input_u(buf, flags, ap);
      break;
    case 'x':
      flags->flag_x = 1;
      input_u(buf, flags, ap);
      break;
    case 'X':
      flags->upper = 1;
      flags->flag_x = 1;
      input_u(buf, flags, ap);
      break;
    case 'p':
      flags->flag_p = 1;
      input_u(buf, flags, ap);
      break;
    default:
      printerror();
  }
}
void input_c(char **buf, another_spec *flags, va_list *ap) {
  char c = va_arg(*ap, int);
  if (flags->width) {
    if (flags->minus) {
      **buf = c;
      *buf += 1;
      write_space_in_str(buf, flags->width - 1, flags);
    } else {
      write_space_in_str(buf, flags->width - 1, flags);
      *((*buf)++) = c;
    }
  } else {
    **buf = c;
    (*buf)++;
  }
}
void input_d(char **buf, another_spec *flags, va_list *ap) {
  long int number = 0;
  int count_lenght_d = 0;
  if (flags->lenght_h) {
    number = (short)va_arg(*ap, int);
  } else if (flags->lenght_l) {
    number = (long int)va_arg(*ap, long int);
  } else {
    number = (int)va_arg(*ap, int);
  }

  char *res = int_to_str(number, &count_lenght_d);
  if (res) {
    if (flags->flag_accuracy && flags->flag_width) {
      if (flags->accuracy - flags->width >= 0) {
        do_accuracy(buf, flags, res, count_lenght_d, number);
      } else {
        do_width(buf, flags, res, count_lenght_d, number);
      }
    } else if (flags->flag_accuracy) {
      do_accuracy(buf, flags, res, count_lenght_d, number);
    } else if (flags->flag_width) {
      do_width(buf, flags, res, count_lenght_d, number);
    } else {
      do_main_d(buf, flags, res, count_lenght_d, number);
    }
    free(res);
  }
}
char *int_to_str(long int number, int *count) {
  char *res = malloc(sizeof(char) * START_LENGTH);
  if (number == 0) {
    res[*count] = '0';
    (*count)++;
  } else {
    if (number < 0) {
      number *= -1;
    }
    char c = 0;
    for (; number > 0; number /= 10, (*count)++) {
      c = number % 10 + '0';
      res[*count] = c;
    }
  }
  return res;
}
void do_accuracy(char **buf, another_spec *flags, char *res, int count_lenght_d,
                 long int number) {
  char min = 0;
  if ((flags->plus || number < 0) && !flags->flag_u) {
    put_signes(buf, flags, number);
  }
  if (!flags->accuracy && !number) {
    return;
  }
  if ((flags->space && !flags->plus && number > 0) && !flags->flag_u) {
    **buf = ' ';
    (*buf)++;
  }
  if (flags->flag_p && number) {
    {
      s21_strcpy(*buf, "0x");
      *buf += 2;
    }
  }
  if (flags->flag_sharp) {
    if (flags->flag_o) {
      min = 1;
    } else if (flags->flag_x) {
      if (flags->upper) {
        s21_strcpy(*buf, "0X");
        *buf += 2;
      } else {
        s21_strcpy(*buf, "0x");
        *buf += 2;
      }
    }
  }
  if (flags->accuracy > count_lenght_d - min) {
    write_zero_in_str(buf, flags->accuracy - (count_lenght_d - min));
    write_some_in_str(buf, res, count_lenght_d);
  } else {
    write_some_in_str(buf, res, count_lenght_d);
  }
}
void do_width(char **buf, another_spec *flags, char *res, int count_lenght_d,
              long int number) {
  if (flags->minus) {
    do_width_minus(buf, flags, res, count_lenght_d, number);
  } else {
    do_width_plus(buf, flags, res, count_lenght_d, number);
  }
}
void do_main_d(char **buf, another_spec *flags, char *res, int count_lenght_d,
               long int number) {
  if (flags->space && !flags->plus && number >= 0) {
    **buf = ' ';
    (*buf)++;
  }
  if ((flags->plus || number < 0) && !flags->flag_u) {
    put_signes(buf, flags, number);
  }
  if ((flags->flag_p && number) ||
      (flags->flag_sharp && flags->flag_x && !flags->upper && number)) {
    s21_strcpy(*buf, "0x");
    *buf += 2;
  }
  if (flags->flag_sharp && flags->flag_x && flags->upper && number) {
    s21_strcpy(*buf, "0X");
    *buf += 2;
  }
  write_some_in_str(buf, res, count_lenght_d);
}

void write_some_in_str(char **buf, char *res, int lenght) {
  for (int i = 0, j = lenght - 1; i < lenght; i++, j--, (*buf)++) {
    **buf = res[j];
  }
}
void write_space_in_str(char **buf, int lenght, another_spec *flags) {
  char res = ' ';
  if (flags->flag_zero) {
    res = '0';
  }
  for (int i = 0; i < lenght; i++, (*buf)++) {
    **buf = res;
  }
}
void put_signes(char **buf, another_spec *flags, long int number) {
  if (number < 0) {
    **buf = '-';
    *buf += 1;
  } else if (flags->plus && number >= 0) {
    **buf = '+';
    *buf += 1;
  }
}
void input_f(char **buf, another_spec *flags, va_list *ap) {
  long double number;
  if (!flags->flag_accuracy) {
    flags->accuracy = 6;
  }
  if (flags->big_l) {
    number = (long double)va_arg(*ap, long double);
  } else {
    number = (double)va_arg(*ap, double);
  }
  if (number < 0) {
    number *= -1;
    flags->flag_minus_f = 1;
  }
  if (!isfinite(number)) {
    no_end_number(buf, number, flags);
  } else {
    if (flags->flag_g) {
      check_what_to_do(&number, flags);
    }
    if (flags->flag_e) {
      flags->count_e = realize_e(&number, flags);
      flags->width -= 4;
    }
    long int before_dot = 0;
    int count_after = 0, count_before = 0;
    char *array_after_dot = divide_float_number(flags, number, &before_dot,
                                                flags->accuracy, &count_after);
    char *array_before_dot = int_to_str(before_dot, &count_before);
    if (array_after_dot && array_before_dot) {
      if (flags->width > (flags->accuracy + count_before + 1) &&
          flags->flag_width) {
        do_width_f(buf, flags, array_after_dot, array_before_dot, number,
                   count_after, count_before);
      } else {
        do_main_f(buf, flags, array_after_dot, array_before_dot, number,
                  count_after, count_before);
      }
      free(array_after_dot);
      free(array_before_dot);
    }
  }
}
char *divide_float_number(another_spec *flags, long double number,
                          long int *before_dot, int lenght, int *count_after) {
  (void)flags;
  char *res = malloc(sizeof(char) * lenght + 1);
  long double res_1;
  number = modfl(number, &res_1);
  *before_dot = (long int)res_1;
  for (int i = 0; i < lenght + 1; i++, (*count_after)++) {
    number *= 10;
    res[i] = (int)number + '0';
    number = modfl(number, &res_1);
  }

  (*count_after) -= 1;
  if (res[lenght] >= '5') {
    res[lenght - 1] += 1;
    if (res[lenght - 1] == ':') {
      res[lenght - 1] = '0';
      round_g(res, lenght - 1);
    } else if (res[lenght - 1] == '0') {
      round_g(res, lenght - 1);
    }
  }
  return res;
}

void do_width_f(char **buf, another_spec *flags, char *array_after_dot,
                char *array_before_dot, long double number, int count_after,
                int count_before) {
  if (flags->minus) {
    do_width_f_minus(buf, flags, array_after_dot, array_before_dot, number,
                     count_after, count_before);
  } else {
    do_width_f_plus(buf, flags, array_after_dot, array_before_dot, number,
                    count_after, count_before);
  }
}
void write_zero_in_str(char **buf, int lenght) {
  for (int i = 0; i < lenght; i++, (*buf)++) {
    **buf = '0';
  }
}
void do_main_f(char **buf, another_spec *flags, char *array_after_dot,
               char *array_before_dot, long double number, int count_after,
               int count_before) {
  if (flags->space && !flags->plus && number >= 0) {
    **buf = ' ';
    (*buf)++;
  }
  if (flags->plus || flags->flag_minus_f) {
    put_signes_f(buf, flags, number);
  }
  if (flags->flag_g) {
    write_some_g_flag(buf, flags, array_before_dot, array_after_dot,
                      count_after, count_before);
  } else {
    write_some_in_str(buf, array_before_dot, count_before);
    if (flags->accuracy || flags->flag_sharp) {
      **buf = '.';
      (*buf)++;
    }
    write_some_in_str_s(buf, array_after_dot, count_after);
    if (flags->accuracy > count_after) {
      write_zero_in_str(buf, flags->accuracy - count_after);
    }
  }
  if (flags->flag_e) {
    write_exp_of_e(buf, flags);
  }
}
void input_s(char **buf, another_spec *flags, va_list *ap) {
  char *res = (char *)va_arg(*ap, char *);
  s21_size_t count = s21_strlen(res);
  if (flags->flag_accuracy && (s21_size_t)flags->accuracy < s21_strlen(res)) {
    count = flags->accuracy;
  }
  if (count < (s21_size_t)flags->width) {
    do_width_s(buf, flags, res);
  } else {
    write_some_in_str_s(buf, res, count);
  }
}
void do_width_s(char **buf, another_spec *flags, char *res) {
  s21_size_t count = s21_strlen(res);
  if (flags->flag_accuracy && (s21_size_t)flags->accuracy < s21_strlen(res)) {
    count = flags->accuracy;
  }
  if (flags->minus) {
    write_some_in_str_s(buf, res, count);
    write_space_in_str(buf, flags->width - count, flags);
  } else {
    write_space_in_str(buf, flags->width - count, flags);
    write_some_in_str_s(buf, res, count);
  }
}
void write_some_in_str_s(char **buf, char *res, int lenght) {
  for (int i = 0; i < lenght; i++, (*buf)++) {
    **buf = res[i];
  }
}
void input_u(char **buf, another_spec *flags, va_list *ap) {
  flags->flag_u = 1;
  flags->space = 0;
  flags->plus = 0;
  unsigned long int number = 0;
  int count_lenght_d = 0;
  if (flags->lenght_h) {
    number = (unsigned short)va_arg(*ap, unsigned int);
  } else if (flags->lenght_l || flags->flag_p) {
    number = (unsigned long int)va_arg(*ap, unsigned long int);
  } else {
    number = (unsigned int)va_arg(*ap, unsigned int);
  }
  char *res = NULL;
  if (flags->flag_o) {
    res = get_8_system(number, &count_lenght_d, flags);
  } else if (flags->flag_x || flags->flag_p) {
    if (flags->flag_p && !number) {
      res = get_null_p(&count_lenght_d);
    } else {
      res = get_16_system(number, &count_lenght_d, flags);
    }
  } else {
    res = int_to_str_u(number, &count_lenght_d);
  }
  if (res) {
    if (flags->flag_accuracy && flags->flag_width) {
      if (flags->accuracy - flags->width >= 0) {
        do_accuracy(buf, flags, res, count_lenght_d, number);
      } else {
        do_width(buf, flags, res, count_lenght_d, number);
      }
    } else if (flags->flag_accuracy) {
      do_accuracy(buf, flags, res, count_lenght_d, number);
    } else if (flags->flag_width) {
      do_width(buf, flags, res, count_lenght_d, number);
    } else {
      do_main_d(buf, flags, res, count_lenght_d, number);
    }
    free(res);
  }
}
void put_signes_f(char **buf, another_spec *flags, long double number) {
  if (flags->flag_minus_f) {
    **buf = '-';
    *buf += 1;
  } else if (flags->plus && number >= 0) {
    **buf = '+';
    *buf += 1;
  }
}
int realize_e(long double *number, another_spec *flags) {
  long double cpy = *number;
  char flag_minus = 0;
  int res = 0;
  if (*number < 0) {
    *number *= -1;
    flag_minus = 1;
  } else if (*number == 0) {
    flags->flag_e_sign = 1;
  } else if (*number >= 1) {
    flags->flag_e_sign = 1;
    for (; *number > 10; *number /= 10, res++) {
      ;
    }
  } else {
    flags->flag_e_sign = 0;
    for (; *number < 0.999999999999999; *number *= 10, res++) {
      ;
    }
  }
  if (flag_minus) {
    *number *= -1;
  }
  if (flags->flag_g && !flags->flag_g_e) {
    *number = cpy;
  }
  return res;
}
void write_exp_of_e(char **buf, another_spec *flags) {
  if (flags->upper) {
    **buf = 'E';
    *buf += 1;
  } else {
    **buf = 'e';
    *buf += 1;
  }
  if (flags->flag_e_sign) {
    **buf = '+';
    *buf += 1;
  } else {
    **buf = '-';
    *buf += 1;
  }
  if (flags->count_e <= 9) {
    **buf = '0';
    *buf += 1;
    **buf = flags->count_e + '0';
    *buf += 1;
  } else {
    int count_chars = 0;
    char *array = int_to_str(flags->count_e, &count_chars);
    if (array) {
      write_some_in_str(buf, array, count_chars);
      free(array);
    }
  }
}
void check_what_to_do(long double *number, another_spec *flags) {
  int res = realize_e(number, flags);
  if ((!flags->flag_e_sign && res > 4) || res >= flags->accuracy) {
    flags->flag_g_e = 1;
    flags->flag_e = 1;
  }
}
void write_some_g_flag(char **buf, another_spec *flags, char *array_before_dot,
                       char *array_after_dot, int count_after,
                       int count_before) {
  (void)count_after;
  int count = flags->accuracy;
  for (int i = 0, j = count_before - 1; i < count_before;
       count--, i++, j--, *buf += 1) {
    **buf = array_before_dot[j];
  }
  flags->width -= count_before;
  if (!flags->flag_sharp && count) {
    for (int i = count - 1; i >= 0; i--) {
      if (array_after_dot[i] == '0') {
        count -= 1;
      } else {
        break;
      }
    }
  }
  if (count > 0 || flags->flag_sharp) {
    **buf = '.';
    *buf += 1;
    flags->width -= 1;
  }
  for (int i = 0; i < count; i++, *buf += 1, flags->width--) {
    **buf = array_after_dot[i];
  }
}
char *get_8_system(unsigned long int number, int *count_leght,
                   another_spec *flags) {
  char *res = malloc(sizeof(char) * START_LENGTH);
  unsigned long check = number;
  if (!check) {
    *res = '0';
    *count_leght += 1;
  } else {
    for (; number > 0; number /= 8, *count_leght += 1) {
      res[*count_leght] = number % 8 + '0';
    }
    if (flags->flag_sharp && check) {
      res[*count_leght] = '0';
      *count_leght += 1;
    }
  }
  return res;
}
char *get_16_system(unsigned long int number, int *count_leght,
                    another_spec *flags) {
  char *res = malloc(sizeof(char) * START_LENGTH);
  int res_int = 0;
  unsigned long check = number;
  if (!check) {
    *res = '0';
    *count_leght += 1;
  } else {
    for (; number > 0; number /= 16, *count_leght += 1) {
      res_int = number % 16;
      switch (res_int) {
        case 10:
          if (flags->upper) {
            res_int = 'A';
          } else {
            res_int = 'a';
          }
          break;
        case 11:
          if (flags->upper) {
            res_int = 'B';
          } else {
            res_int = 'b';
          }
          break;
        case 12:
          if (flags->upper) {
            res_int = 'C';
          } else {
            res_int = 'c';
          }
          break;
        case 13:
          if (flags->upper) {
            res_int = 'D';
          } else {
            res_int = 'd';
          }
          break;
        case 14:
          if (flags->upper) {
            res_int = 'E';
          } else {
            res_int = 'e';
          }
          break;
        case 15:
          if (flags->upper) {
            res_int = 'F';
          } else {
            res_int = 'f';
          }
          break;
        default:
          res_int += '0';
          break;
      }
      res[*count_leght] = res_int;
    }
  }
  return res;
}
void set_star(another_spec *flags, va_list *ap) {
  if (!flags->flag_accuracy) {
    flags->flag_width = 1;
    flags->width = (unsigned int)va_arg(*ap, unsigned int);
  } else {
    flags->accuracy = (unsigned int)va_arg(*ap, unsigned int);
  }
}
void no_end_number(char **buf, double number, another_spec *flags) {
  char *array = malloc(sizeof(char) * 5);
  if (number != number) {
    s21_strcpy(array, "nan");
  } else if (number == -INFINITY) {
    s21_strcpy(array, "-inf");
  } else if (number == INFINITY) {
    s21_strcpy(array, "inf");
  }
  if (flags->flag_width) {
    do_width_s(buf, flags, array);
  } else {
    write_some_in_str_s(buf, array, s21_strlen(array));
  }
  free(array);
}
char *int_to_str_u(unsigned long number, int *count) {
  char *res = malloc(sizeof(char) * START_LENGTH);
  if (number == 0) {
    res[*count] = '0';
    (*count)++;
  } else {
    char c = 0;
    for (; number > 0; number /= 10, (*count)++) {
      c = number % 10 + '0';
      res[*count] = c;
    }
  }
  return res;
}
int counting_space(int accuracy, int before, int after, char *array) {
  int res = 0;
  if (accuracy - (before + 1) > 0) {
    accuracy -= (before + 1);
    if (accuracy < after) {
      after = accuracy;
    }
    for (int i = after - 1; i >= 0; i--) {
      if (array[i] == '0') {
        res += 1;
      } else {
        break;
      }
    }
  }
  return res;
}
void do_width_minus(char **buf, another_spec *flags, char *res,
                    int count_lenght_d, long int number) {
  if (flags->space && !flags->plus && number >= 0) {
    **buf = ' ';
    (*buf)++;
    flags->width -= 1;
  }
  if ((flags->plus || number < 0) && !flags->flag_u) {
    put_signes(buf, flags, number);
    flags->width -= 1;
  }
  if (flags->width > count_lenght_d) {
    if ((flags->flag_p && number) ||
        (flags->flag_sharp && flags->flag_x && !flags->upper && number)) {
      s21_strcpy(*buf, "0x");
      *buf += 2;
      flags->width -= 2;
    }
    if (flags->flag_sharp && flags->flag_x && flags->upper && number) {
      s21_strcpy(*buf, "0x");
      *buf += 2;
      flags->width -= 2;
    }
    write_some_in_str(buf, res, count_lenght_d);
    write_space_in_str(buf, flags->width - count_lenght_d, flags);
  } else {
    if ((flags->flag_p && number) ||
        (flags->flag_sharp && flags->flag_x && !flags->upper && number)) {
      s21_strcpy(*buf, "0x");
      *buf += 2;
    }
    if (flags->flag_sharp && flags->flag_x && flags->upper && number) {
      s21_strcpy(*buf, "0x");
      *buf += 2;
    }
    write_some_in_str(buf, res, count_lenght_d);
  }
}
void do_width_plus(char **buf, another_spec *flags, char *res,
                   int count_lenght_d, long int number) {
  if (flags->space && !flags->plus && number >= 0) {
    **buf = ' ';
    (*buf)++;
    flags->width -= 1;
  }
  if (flags->width > count_lenght_d) {
    if (flags->plus || number < 0) {
      flags->width -= 1;
    }
    if (flags->flag_p || (flags->flag_sharp && flags->flag_x && number)) {
      flags->width -= 2;
    }
    if ((flags->plus || number < 0) && flags->flag_zero && !flags->flag_u) {
      put_signes(buf, flags, number);
    }
    write_space_in_str(buf, flags->width - count_lenght_d, flags);
    if ((flags->plus || number < 0) && !flags->flag_zero && !flags->flag_u) {
      put_signes(buf, flags, number);
    }
    if ((flags->flag_p && number) ||
        (flags->flag_sharp && flags->flag_x && !flags->upper && number)) {
      s21_strcpy(*buf, "0x");
      *buf += 2;
    }
    if (flags->flag_sharp && flags->flag_x && flags->upper && number) {
      s21_strcpy(*buf, "0X");
      *buf += 2;
    }
    write_some_in_str(buf, res, count_lenght_d);
  } else {
    if ((flags->plus || number < 0) && !flags->flag_u) {
      put_signes(buf, flags, number);
    }
    if ((flags->flag_p && number) ||
        (flags->flag_sharp && flags->flag_x && !flags->upper && number)) {
      s21_strcpy(*buf, "0x");
      *buf += 2;
    }
    if (flags->flag_sharp && flags->flag_x && flags->upper && number) {
      s21_strcpy(*buf, "0X");
      *buf += 2;
    }
    write_some_in_str(buf, res, count_lenght_d);
  }
}
void do_width_f_minus(char **buf, another_spec *flags, char *array_after_dot,
                      char *array_before_dot, long double number,
                      int count_after, int count_before) {
  {
    if (flags->space && !flags->plus && number >= 0) {
      **buf = ' ';
      (*buf)++;
      flags->width -= 1;
    }
    if (flags->plus || flags->flag_minus_f) {
      put_signes_f(buf, flags, number);
      if (flags->accuracy || flags->flag_g) {
        flags->width -= 1;
      }
    }
    if (flags->flag_g) {
      write_some_g_flag(buf, flags, array_before_dot, array_after_dot,
                        count_after, count_before);
    } else {
      write_some_in_str(buf, array_before_dot, count_before);
      if (flags->accuracy || flags->flag_sharp) {
        **buf = '.';
        (*buf)++;
      }
      write_some_in_str_s(buf, array_after_dot, count_after);
      if (flags->accuracy > count_after) {
        write_zero_in_str(buf, flags->accuracy - count_after);
      }
    }
    if (flags->flag_e) {
      write_exp_of_e(buf, flags);
    }
    if (flags->flag_g) {
      write_space_in_str(buf, flags->width, flags);
    } else {
      write_space_in_str(
          buf, flags->width - (count_before + flags->accuracy + 1), flags);
    }
  }
}
void do_width_f_plus(char **buf, another_spec *flags, char *array_after_dot,
                     char *array_before_dot, long double number,
                     int count_after, int count_before) {
  if (flags->space && !flags->plus && number >= 0) {
    **buf = ' ';
    (*buf)++;
    flags->width -= 1;
  }
  if (((flags->plus || number < 0) && flags->accuracy) || flags->flag_g) {
    flags->width -= 1;
  }
  if ((flags->plus || flags->flag_minus_f) && flags->flag_zero) {
    put_signes_f(buf, flags, number);
  }
  if (flags->flag_g) {
    if (flags->flag_sharp || flags->accuracy <= count_before) {
      if (!flags->accuracy) {
        write_space_in_str(buf, flags->width - 1, flags);
      } else {
        write_space_in_str(buf, flags->width - flags->accuracy, flags);
      }
    } else {
      flags->width += 1;
      int count_space = counting_space(flags->accuracy, count_before,
                                       count_after, array_after_dot);
      write_space_in_str(buf, flags->width - (flags->accuracy - count_space),
                         flags);
    }
  } else {
    write_space_in_str(buf, flags->width - (count_before + flags->accuracy + 1),
                       flags);
  }
  if ((flags->plus || flags->flag_minus_f) && !flags->flag_zero) {
    put_signes_f(buf, flags, number);
  }
  if (flags->flag_g) {
    write_some_g_flag(buf, flags, array_before_dot, array_after_dot,
                      count_after, count_before);
  } else {
    write_some_in_str(buf, array_before_dot, count_before);
    if (flags->accuracy || flags->flag_sharp) {
      **buf = '.';
      (*buf)++;
    }
    write_some_in_str_s(buf, array_after_dot, count_after);
    if (flags->accuracy > count_after) {
      write_zero_in_str(buf, flags->accuracy - count_after);
    }
  }
  if (flags->flag_e) {
    write_exp_of_e(buf, flags);
  }
}
char *get_null_p(int *length) {
  char *res = malloc(sizeof(char) * 5);
  s21_strcpy(res, ")lin(");
  *length = 5;
  return res;
}
void round_g(char *res, int count_after) {
  char edinica = 1;
  for (int i = count_after - 1; i >= 0; i--) {
    if (res[i + 1] == '9' || edinica) {
      res[i] += 1;
      if (res[i + 1] == '9') {
        res[i + 1] = '0';
      }
      if (edinica) {
        edinica = 0;
      }
      if (res[i] == ':') {
        res[i] = '0';
        edinica = 1;
      }
    } else {
      break;
    }
  }
}
