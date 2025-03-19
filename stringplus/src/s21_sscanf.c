#include "s21_string.h"

int s21_sscanf(const char *buf, const char *format, ...) {
  va_list ap;
  int res = 0;
  int res_star = 0;
  char flag_space = 0;
  const char *start = buf;
  if (checkeof(buf)) {
    va_start(ap, format);
    for (; *format; format++) {
      if (*format == '%' && *(format + 1) == '%') {
        format++;
        move_for_percent(&buf);
        continue;
      } else if (*format == '%' && (*(format + 1) != '%')) {
        format = dividestrspec(format, &ap, &res, &buf, &res_star, &flag_space,
                               start);
      } else if (*format == *buf) {
        buf += 1;
      }
    }
    va_end(ap);
  } else {
    res = -1;
  }
  return res;
}

const char *dividestrspec(const char *format, va_list *ap, int *res,
                          const char **buf, int *res_star, char *flag_space,
                          const char *start) {
  specs flags = {0};
  char *a = "cdfsueEgGoxXpni";
  const char *ptr_start = format + 1;
  while (!s21_strchr(a, *format)) {
    format++;
  }
  setflags_scanf(&flags, ptr_start, format);
  chooseformat_scanf(buf, format, &flags, ap, res, res_star, flag_space, start);
  return format++;
}
void setflags_scanf(specs *flags, const char *ptr_start, const char *format) {
  char *a = "cdfsuoxXgGeEli";
  char *b = "fgGeE";
  while (ptr_start != format) {
    switch (*ptr_start) {
      case '*':
        flags->flag_star = 1;
        break;
      case 'h':
        if (s21_strchr(a, *(ptr_start + 1)) && !flags->lenght_l &&
            !flags->big_l) {
          flags->lenght_h = 1;
        }
        break;
      case 'L':
        if (s21_strchr(b, *(ptr_start + 1)) && !flags->lenght_l &&
            !flags->lenght_h) {
          flags->big_l = 1;
        }
        break;
      case 'l':
        if (s21_strchr(a, *(ptr_start + 1)) && !flags->lenght_h &&
            !flags->big_l) {
          flags->lenght_l = 1;
        }
        break;
      default:
        probablywidth_scanf(&ptr_start, flags);
        ptr_start--;
    }
    ptr_start++;
  }
}
void probablywidth_scanf(const char **ptr, specs *flags) {
  if ((**ptr) >= '0' && (**ptr) <= '9') {
    flags->flag_width = 1;
    flags->width = setnumber_scanf(ptr, flags);
  }
}
long setnumber_scanf(const char **ptr, specs *flags) {
  char end = '9';
  char multy = 10;
  if (flags->flag_o) {
    end = '7';
    multy = 8;
  }
  const char *count_quantity = *ptr;
  int step = -1;
  while ((*count_quantity) >= '0' && (*count_quantity) <= end) {
    step += 1;
    count_quantity += 1;
  }
  long int quantity = count_quantity - *ptr;
  long res = 0;
  for (int i = 0; i < quantity; i++, (*ptr)++, step--) {
    res += ((**ptr) - '0') * pow(multy, step);
  }
  return res;
}
void printerror_scanf() {
  printf("incorrect input of spec\n");
  // exit(1);
}
void chooseformat_scanf(const char **buf, const char *format, specs *flags,
                        va_list *ap, int *res, int *res_star, char *flag_space,
                        const char *start) {
  switch (*format) {
    case 'c':
      if (*flag_space) {
        move_buf_for_c(buf);
      }
      if (!flags->flag_star) {
        write_c(buf, ap, flag_space, flags);
        *res += 1;
      } else {
        *res_star += 1;
        *buf += 1;
      }
      break;
    case 'd':
      move_buf_for_d(buf, flags);
      if (!flags->flag_do) {
        long d_res = do_d(buf, flags);
        if (!flags->flag_star) {
          write_d(d_res, ap, flags);
          *res += 1;
        } else {
          *res_star += 1;
        }
      }
      break;
    case 'u':
      move_buf_for_d(buf, flags);
      if (!flags->flag_do) {
        long u_res = do_d(buf, flags);
        if (!flags->flag_star) {
          write_u(u_res, ap, flags);
          *res += 1;
        } else {
          *res_star += 1;
        }
      }
      break;
    case 's':
      move_buf_for_s(buf);
      if (!flags->flag_star) {
        write_s(buf, ap, flags);
        *res += 1;
      } else {
        *res_star += 1;
      }
      break;
    case 'o':
      flags->flag_o = 1;
      move_buf_for_d(buf, flags);
      if (!flags->flag_do) {
        long o_res = do_d(buf, flags);
        if (!flags->flag_star) {
          write_u(o_res, ap, flags);
          flags->flag_o = 0;
          *res += 1;
        } else {
          *res_star += 1;
        }
      } else {
        flags->flag_o = 0;
      }
      break;
    case 'x':
    case 'X':
      move_buf_for_x(buf, flags);
      if (!flags->flag_do) {
        long x_res = do_x(buf, flags);
        if (!flags->flag_star) {
          write_u(x_res, ap, flags);
          *res += 1;
        } else {
          *res_star += 1;
        }
      }
      break;
    case 'p':
      flags->flag_p = 1;
      if (!checknull_p(*buf)) {
        long int **null_res = va_arg(*ap, long int **);
        *null_res = s21_NULL;
        *res += 1;
      } else {
        move_buf_for_x(buf, flags);
        if (!flags->flag_do) {
          long p_res = do_x(buf, flags);
          if (!flags->flag_star) {
            write_u(p_res, ap, flags);
            *res += 1;
          } else {
            *res_star += 1;
          }
        } else {
          flags->flag_p = 0;
        }
      }
      break;
    case 'f':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
      move_buf_for_float(buf, flags);
      long double f_res;
      if (flags->nan) {
        f_res = NAN;
        flags->nan = 0;
      } else if (flags->inf) {
        f_res = INFINITY;
        flags->inf = 0;
      } else {
        f_res = do_float(buf, flags);
      }
      if (!flags->flag_star) {
        write_double(f_res, ap, flags);
        *res += 1;
      } else {
        *res_star += 1;
      }
      break;
    case 'n':
      write_n(buf, ap, start);
      break;
    case 'i':
      move_buf_for_d(buf, flags);
      if (!flags->flag_do) {
        long d_res;
        if (**buf == '0' && (*(*buf + 1) == 'x' || (*(*buf + 1) == 'X'))) {
          d_res = do_x(buf, flags);
        } else if (**buf == '0' && (*(*buf + 1) >= '1' && *(*buf + 1) <= '9')) {
          flags->flag_o = 1;
          d_res = do_d(buf, flags);
          flags->flag_o = 0;
        } else {
          d_res = do_d(buf, flags);
        }
        if (!flags->flag_star) {
          write_d(d_res, ap, flags);
          *res += 1;
        } else {
          *res_star += 1;
        }
      }
      break;
  }
}
void write_c(const char **buf, va_list *ap, char *flag_space, specs *flags) {
  if (!flags->lenght_l) {
    char *res = (char *)va_arg(*ap, char *);
    *res = **buf;
    *flag_space = 1;
    *buf += 1;
  } else {
    wchar_t *res = (wchar_t *)va_arg(*ap, wchar_t *);
    *res = (wchar_t) * *buf;
    *flag_space = 1;
    *buf += 1;
  }
}
void write_d(long number, va_list *ap, specs *flags) {
  if (flags->negative) {
    number *= -1;
  }
  if (flags->lenght_h) {
    short *res = (short *)va_arg(*ap, short *);
    *res = (short)number;
  } else if (flags->lenght_l) {
    long *res = (long *)va_arg(*ap, long *);
    *res = number;
  } else {
    int *res = (int *)va_arg(*ap, int *);
    *res = (int)number;
  }
  flags->negative = 0;
}
long setnumber_width(const char **ptr, unsigned long width, specs *flags) {
  long res = 0;
  if (width) {
    char end = '9';
    char multy = 10;
    if (flags->flag_o) {
      end = '7';
      multy = 8;
    }
    int step = width - 1;
    step = check_step(*ptr, step);
    unsigned long i = 0;
    for (; i < width; i++, (*ptr)++, step--) {
      if (**ptr >= '0' && **ptr <= end) {
        res += ((**ptr) - '0') * pow(multy, step);
      } else {
        break;
      }
    }
    if (i < width) {
      while (i < width || **ptr) {
        i++;
        *ptr += 1;
      }
    }
  } else {
    flags->flag_star = 1;
    while (**ptr) {
      *ptr += 1;
    }
  }
  return res;
}
long do_d(const char **buf, specs *flags) {
  long int d_res = 0;
  if (!flags->flag_width) {
    d_res = setnumber_scanf((const char **)buf, flags);
  } else {
    d_res = setnumber_width((const char **)buf, flags->width, flags);
  }
  return d_res;
}
void move_buf_for_d(const char **buf, specs *flags) {
  char end = '9';
  char start = '0';
  if (**buf == '0' && *(*buf + 1) == '0') {
    while (**buf == '0' && **buf != '\0') {
      *buf += 1;
    }
  }
  if (flags->flag_o) {
    end = '7';
  }
  while (**buf < start || **buf > end) {
    if ((**buf == '-' || **buf == '+') &&
        (flags->negative || *(*buf + 1) == '+')) {
      while (**buf != '\0') {
        *buf += 1;
      }
    } else if (**buf == '-') {
      if (flags->flag_width) {
        flags->width -= 1;
      }
      flags->negative = 1;
      *buf += 1;
    } else if (**buf == '\0' ||
               ((**buf == 'x' || **buf == 'X') && *(*buf - 1) == '0')) {
      if (**buf == '\0') {
        flags->flag_do = 1;
      }
      break;
    } else if (**buf == ' ' || **buf == '+') {
      if (**buf == '+') {
        flags->width -= 1;
      }
      *buf += 1;
      continue;
    } else {
      while (**buf != '\0') {
        *buf += 1;
      }
    }
  }
}
void write_u(long number, va_list *ap, specs *flags) {
  if (flags->negative) {
    number *= -1;
  }
  if (flags->lenght_h) {
    unsigned short *res = (unsigned short *)va_arg(*ap, unsigned short *);
    *res = (unsigned short)number;
  } else if (flags->lenght_l || flags->flag_p) {
    unsigned long *res = (unsigned long *)va_arg(*ap, unsigned long *);
    *res = (unsigned long)number;
  } else {
    unsigned int *res = (unsigned int *)va_arg(*ap, unsigned int *);
    *res = (unsigned int)number;
  }
  flags->negative = 0;
}
void write_s(const char **buf, va_list *ap, specs *flags) {
  if (!flags->lenght_l) {
    char *res = (char *)va_arg(*ap, char *);
    if (flags->flag_width) {
      for (unsigned long i = 0; i < flags->width; i++, *buf += 1, res += 1) {
        if (**buf <= ' ') {
          break;
        }
        *res = **buf;
      }
      *res = '\0';
    } else {
      while (**buf > ' ') {
        *res = **buf;
        res += 1;
        *buf += 1;
      }
      *res = '\0';
    }
  } else {
    wchar_t *res = (wchar_t *)va_arg(*ap, wchar_t *);
    if (flags->flag_width) {
      for (unsigned long i = 0; i < flags->width; i++, *buf += 1, res += 1) {
        if (**buf <= ' ') {
          break;
        }
        *res = (wchar_t) * *buf;
      }
      *res = L'\0';
    } else {
      while (**buf > ' ') {
        *res = (wchar_t) * *buf;
        res += 1;
        *buf += 1;
      }
      *res = L'\0';
    }
  }
}
void move_buf_for_s(const char **buf) {
  while (**buf <= ' ') {
    *buf += 1;
  }
}
long setnumber_x(const char **ptr) {
  char array[5];
  s21_strncpy(array, *ptr, 2);
  array[2] = '\0';
  if (s21_strstr(array, "0x") || s21_strstr(array, "0X")) {
    *ptr += 2;
  }
  const char *count_quantity = *ptr;
  int step = -1;
  while (((*count_quantity) >= '0' && (*count_quantity) <= '9') ||
         ((*count_quantity) >= 'A' && (*count_quantity) <= 'F') ||
         ((*count_quantity) >= 'a' && (*count_quantity) <= 'f')) {
    step += 1;
    count_quantity += 1;
  }
  long int quantity = count_quantity - *ptr;
  long res = 0;
  for (int i = 0; i < quantity; i++, (*ptr)++, step--) {
    if (**ptr >= '0' && **ptr <= '9') {
      res += ((**ptr) - '0') * pow(16, step);
    } else if ((**ptr) >= 'A' && (**ptr) <= 'F') {
      res += ((**ptr) - '7') * pow(16, step);
    } else if ((**ptr) >= 'a' && (**ptr) <= 'f') {
      res += ((**ptr) - 'W') * pow(16, step);
    }
  }
  return res;
}
long setnumber_width_x(const char **ptr, unsigned long width) {
  char array[5];
  s21_strncpy(array, *ptr, 2);
  array[2] = '\0';
  if (s21_strstr(array, "0x") || s21_strstr(array, "0X")) {
    *ptr += 2;
    width -= 2;
  }
  int step = width - 1;
  long res = 0;
  for (unsigned long i = 0; i < width; i++, (*ptr)++, step--) {
    if (**ptr >= '0' && **ptr <= '9') {
      res += ((**ptr) - '0') * pow(16, step);
    } else if (**ptr >= 'A' && **ptr <= 'F') {
      res += ((**ptr) - '7') * pow(16, step);
    } else if ((**ptr) >= 'a' && (**ptr) <= 'f') {
      res += ((**ptr) - 'W') * pow(16, step);
    } else {
      break;
    }
  }
  return res;
}
void move_buf_for_x(const char **buf, specs *flags) {
  while (**buf < '0' || (**buf > '9' && **buf < 'A') ||
         (**buf > 'F' && **buf < 'a') || **buf > 'f')
    if ((**buf == '-' || **buf == '+') &&
        (flags->negative || *(*buf + 1) == '+')) {
      while (**buf != '\0') {
        *buf += 1;
      }
    } else if (**buf == '-') {
      if (flags->flag_width) {
        flags->width -= 1;
      }
      flags->negative = 1;
      *buf += 1;
    } else if (**buf == '\0' || **buf == '(') {
      flags->flag_do = 1;
      break;
    } else if (**buf <= ' ' || **buf == '+') {
      if (**buf == '+') {
        flags->width -= 1;
      }
      *buf += 1;
      continue;
    } else {
      while (**buf != '\0') {
        *buf += 1;
      }
    }
}
long do_x(const char **buf, specs *flags) {
  long int x_res = 0;
  if (!flags->flag_width) {
    x_res = setnumber_x(buf);
  } else {
    x_res = setnumber_width_x(buf, flags->width);
  }
  return x_res;
}
void sci_to_normal(const char *sci, char *normal) {
  int i = 0, j = 0, exp = 0, sign = 1, point_pos = -1, digits = 0, negative = 0;
  if (sci[i] == '-') {
    negative = 1;
    i++;
  } else if (sci[i] == '+') {
    i++;
  }
  while (sci[i] && sci[i] != 'e' && sci[i] != 'E') {
    if (sci[i] == '.') {
      point_pos = digits;
    } else {
      normal[j++] = sci[i];
      digits++;
    }
    i++;
  }
  normal[j] = '\0';
  if (sci[i] == 'e' || sci[i] == 'E') {
    i++;
    if (sci[i] == '-') {
      sign = -1;
      i++;
    } else if (sci[i] == '+') {
      i++;
    }
    while (sci[i]) {
      exp = exp * 10 + (sci[i] - '0');
      i++;
    }
    exp *= sign;
  }
  if (point_pos == -1) point_pos = digits;
  point_pos += exp;
  char result[1000];
  int k = 0, l = 0;
  if (negative) result[k++] = '-';
  if (point_pos <= 0) {
    result[k++] = '0';
    result[k++] = '.';
    for (l = 0; l < -point_pos; l++) result[k++] = '0';
    for (l = 0; l < digits; l++) result[k++] = normal[l];
  } else {
    for (l = 0; l < digits; l++) {
      if (l == point_pos) result[k++] = '.';
      result[k++] = normal[l];
    }
    while (l++ < point_pos) result[k++] = '0';
  }
  result[k] = '\0';
  s21_strncpy(normal, result, k);
  normal[k] = '\0';
}
long double str_to_float(const char *str) {
  long double result = 0.0;
  int str_len = 0, dot_index = -1, leading_zeros = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] == '.')
      dot_index = i;
    else
      str_len++;
  }
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] == '0')
      leading_zeros++;
    else if (str[i] != '.')
      break;
  }
  for (int j = 0; str[j] != '\0'; j++) {
    if (str[j] != '.') {
      int digit = str[j] - '0';
      result = result * 10.0 + digit;
    }
  }
  int new_len = 0;
  long double temp = result;
  while (temp > 1) {
    new_len++;
    temp /= 10;
  }
  if (leading_zeros != 0) {
    leading_zeros = new_len + leading_zeros - 1;
    while (leading_zeros > 0) {
      result /= 10;
      leading_zeros--;
    }
  } else if (dot_index != -1) {
    dot_index = str_len - dot_index;
    while (dot_index > 0) {
      result /= 10;
      dot_index--;
    }
  }
  return result;
}
void move_buf_for_float(const char **buf, specs *flags) {
  char *infornan = s21_NULL;
  while (**buf < '0' || **buf > '9') {
    if (**buf == '-') {
      flags->negative = 1;
    }
    if (**buf == 'i' || **buf == 'I') {
      infornan = s21_to_lower(*buf);
      if (s21_strstr(infornan, "infinity")) {
        flags->inf = 1;
        *buf += 8;
      } else if (s21_strstr(infornan, "inf")) {
        flags->inf = 1;
        *buf += 3;
      }
      free(infornan);
    }
    if (**buf == 'n' || **buf == 'N') {
      infornan = s21_to_lower(*buf);
      if (s21_strstr(infornan, "nan")) {
        flags->nan = 1;
        *buf += 3;
      }
      free(infornan);
    }
    if (**buf == '\0' || flags->nan || flags->inf) {
      break;
    }
    *buf += 1;
  }
}
long double do_float(const char **buf, specs *flags) {
  long double res = 0;
  char e_do = 0;
  char array[100] = {0};
  unsigned long int i = 0;
  if (!flags->flag_width) {
    for (; (**buf >= '0' && **buf <= '9') || **buf == '.' || **buf == 'e' ||
           **buf == '+' || **buf == '-';
         i++, *buf += 1) {
      if (**buf == '\0') {
        break;
      }
      if (i == 0 && **buf == '-') {
        flags->negative = 1;
      }
      array[i] = **buf;
      if (array[i] == 'e' || array[i] == 'E') {
        e_do = 1;
      }
    }
    array[i] = '\0';
  } else {
    for (; i < flags->width; i++, *buf += 1) {
      if (**buf == '\0') {
        break;
      }
      if (i == 0 && **buf == '-') {
        flags->negative = 1;
      }
      if ((**buf >= '0' && **buf <= '9') || **buf == '.' || **buf == 'e' ||
          **buf == '+' || **buf == '-' || **buf == 'E') {
        if ((**buf == 'e' || **buf == 'E') && e_do) {
          break;
        } else if ((**buf == 'e' || **buf == 'E') && !e_do) {
          e_do = 1;
        }
        array[i] = **buf;
      } else {
        break;
      }
    }
    array[i] = '\0';
  }
  if (e_do) {
    char normal[100];
    sci_to_normal((const char *)array, normal);
    res = str_to_float(normal);
  } else {
    res = str_to_float(array);
  }
  return res;
}
void write_double(long double number, va_list *ap, specs *flags) {
  if (flags->negative) {
    number *= -1;
  }
  if (flags->lenght_l) {
    double *res = (double *)va_arg(*ap, double *);
    *res = (double)number;
  } else if (flags->big_l) {
    long double *res = (long double *)va_arg(*ap, long double *);
    *res = number;
  } else {
    {
      float *res = (float *)va_arg(*ap, float *);
      *res = (float)number;
    }
  }
  flags->negative = 0;
}
void move_buf_for_c(const char **buf) {
  while (**buf == ' ') {
    *buf += 1;
  }
}
int check_step(const char *ptr, int step) {
  int res = -1;
  while (*ptr >= '0' && *ptr <= '9') {
    res += 1;
    ptr += 1;
  }
  if (res < step) {
    step = res;
  }
  return step;
}
int check_start_d(const char *buf) {
  int res = 0;
  while (*buf != '\0') {
    if (*buf >= '1' && *buf <= '9') {
      res = '1';
      break;
    }
    buf += 1;
  }
  return res;
}
void move_for_percent(const char **buf) {
  while (**buf != '%') {
    *buf += 1;
  }
  *buf += 1;
}
int checkeof(const char *buf) {
  int res = 0;
  while (*buf) {
    if (*buf > ' ') {
      res = 1;
      break;
    }
    buf += 1;
  }
  return res;
}
void write_n(const char **buf, va_list *ap, const char *start) {
  int *res = va_arg(*ap, int *);
  *res = *buf - start;
}
int checknull_p(const char *buf) {
  int res = 0;
  if (s21_strncmp(buf, "(nil)", 5)) {
    res = 1;
  }
  return res;
}