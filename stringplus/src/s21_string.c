#include "s21_string.h"
void *s21_memchr(const void *arr, int c, s21_size_t n) {
  const unsigned char *s = arr;
  char what_find = (char)c;
  for (; n && *s != what_find; s++, n--)
    ;
  return n ? (void *)s : 0;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *s_1 = str1;
  const unsigned char *s_2 = str2;
  for (; n && *s_1 == *s_2; s_1++, s_2++, n--)
    ;
  return n ? *s_1 - *s_2 : 0;
}
void *s21_memcpy(void *dst, const void *src, s21_size_t n) {
  unsigned char *s_dst = dst;
  const unsigned char *s_src = src;
  for (; n; n--, s_dst++, s_src++) {
    *s_dst = *s_src;
  }
  return dst;
}
void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *s_str = str;
  unsigned char tmp = c;
  for (; n; n--, s_str++) {
    *s_str = tmp;
  }
  return (void *)str;
}

char *s21_strncpy(char *dest, const char *src, size_t n) {
  size_t i = 0;
  for (; i < n && src[i] != '\0'; i++) {
    dest[i] = src[i];
  }
  while (i < n && dest[i] != '\0') {
    dest[i] = '\0';
    i++;
  }
  return dest;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  const char *i;
  s21_size_t k = 0, nf = 1;
  for (; *str1 != '\0' && nf; str1++) {
    for (i = str2; *i != '\0' && nf; i++) {
      *i == *str1 ? nf = 0 : nf;
    }
    nf ? k++ : k;
  }
  return k;
}

char *s21_strerror(int errnum) {
  char *string = s21_NULL;
#ifdef __linux__
  if (errnum <= linux_errors_count && errnum >= 0) {
    string = linux_errors_array[errnum];
  } else {
    static char res[100];
    s21_sprintf(res, "Unknown error %d", errnum);
    string = res;
  }
#elif __APPLE__
  if (errnum <= mac_errors_count && errnum >= 0) {
    string = linux_errors_array[errnum];
  } else {
    static char res[100];
    s21_sprintf(res, "Unknown error %d", errnum);
    string = res;
  }
#endif
  return string;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  const char *result = s21_NULL;
  while (*str1 != '\0' && result == s21_NULL) {
    const char *i = str2;
    while (*i != '\0' && result == s21_NULL) {
      if (*i == *str1) {
        result = str1;
      }
      i++;
    }
    str1++;
  }
  return (char *)result;
}
s21_size_t s21_strlen(const char *data) {
  s21_size_t res = 0;
  while (data[res] != '\0') {
    res += 1;
  }
  return res;
}

int s21_strcmp(char *a, char *b) {
  int res = 0;
  while (*a == *b && *a != '\0') {
    a++;
    b++;
  }
  if (*a == '\0' && *b == '\0') {
    res = 0;
  } else {
    res = *a - *b;
  }
  return res;
}
char *s21_strcpy(char *dst, const char *src) {
  char *res = dst;
  while (*src != '\0') {
    *dst = *src;
    dst++;
    src++;
  }
  *(dst++) = '\0';
  return res;
}
char *s21_strcat(char *dst, char *src) {
  int len_1 = s21_strlen(dst);
  int len_2 = len_1 + s21_strlen(src);
  for (int i = 0; src[i] != '\0'; i++) {
    dst[len_1 + i] = src[i];
  }
  dst[len_2] = '\0';
  return dst;
}
char *s21_strchr(const char *src, int c) {
  int count = 0;
  while (src[count] && src[count] != c) {
    count += 1;
  }
  return src[count] == c ? (char *)src + count : s21_NULL;
}
char *s21_strstr(const char *str, const char *substr) {
  int len_2 = s21_strlen(substr);
  int j = 0;
  const char *res = s21_NULL;
  char flag_cmp = 0;
  if (s21_strlen(str) > 0 && s21_strlen(substr) > 0) {
    for (s21_size_t i = 0; i < s21_strlen(str); i++) {
      if (str[i] != substr[j] && flag_cmp) {
        j = 0;
        flag_cmp = 0;
      }
      if (str[i] == substr[j]) {
        j++;
        flag_cmp = 1;
        if (j == len_2) {
          res = (str + i - j + 1);
          break;
        }
      }
    }
  } else if (!s21_strlen(substr)) {
    res = str;
  }
  return (char *)res;
}
char *s21_strtok(char *str, const char *delim) {
  static char *ptr;

  if (str)
    for (ptr = str; s21_strchr(delim, *ptr); ++ptr)
      ;

  if (*ptr == '\0') return s21_NULL;

  str = ptr;
  while (*ptr && !s21_strchr(delim, *ptr)) ++ptr;
  while (*ptr && s21_strchr(delim, *ptr)) *ptr++ = '\0';

  return str;
}
char *s21_strrchr(const char *str, int c) {
  const char *p = NULL;
  while (*str != '\0') {
    if (*str == (char)c) {
      p = str;
    }
    str++;
  }
  if (c == 0) {
    p = str;
  }
  return (char *)p;
}
char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *dest_start = dest;

  while (*dest) {
    dest++;
  }

  while (*src && n > 0) {
    *dest = *src;
    dest++;
    src++;
    n--;
  }

  *dest = '\0';

  return dest_start;
}
int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  for (s21_size_t j = 0; j < n; j++) {
    if (str1[j] != str2[j]) {
      return (unsigned char)str1[j] - (unsigned char)str2[j];
    }
    if (str1[j] == '\0' || str2[j] == '\0') {
      return 0;
    }
  }
  return 0;
}