#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"

void *s21_to_upper(const char *str) {
  char *res = s21_NULL;
  if (str) {
    res = malloc(sizeof(char) * (s21_strlen((char *)str) + 1));
  }
  if (res) {
    for (s21_size_t i = 0; i < s21_strlen((char *)str); i++) {
      res[i] = str[i];
      if (res[i] >= 'a' && res[i] <= 'z') {
        res[i] -= ' ';
      }
    }
    res[s21_strlen((char *)str)] = '\0';
  }
  return (void *)res;
}
void *s21_to_lower(const char *str) {
  char *res = s21_NULL;
  if (str) {
    res = malloc(sizeof(char) * (s21_strlen((char *)str) + 1));
  }
  if (res) {
    for (s21_size_t i = 0; i < s21_strlen((char *)str); i++) {
      res[i] = str[i];
      if (res[i] >= 'A' && res[i] <= 'Z') {
        res[i] += ' ';
      }
    }
    res[s21_strlen((char *)str)] = '\0';
  }
  return (void *)res;
}
void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *res = s21_NULL;
  if (src && str) {
    if (start_index <= s21_strlen((char *)src)) {
      s21_size_t lenght = s21_strlen((char *)str) + s21_strlen((char *)src) + 1;
      res = malloc(sizeof(char) * lenght);
      if (res) {
        for (s21_size_t i = 0, j = 0; i <= s21_strlen((char *)src); i++, j++) {
          if (i == start_index) {
            while (*str) {
              res[j] = *str;
              str++;
              j++;
            }
          }
          res[j] = src[i];
        }
        res[lenght - 1] = '\0';
      }
    }
  }
  return (void *)res;
}
int is_trim_char(char c, const char *trim_chars) {
  int res = 0;
  while (*trim_chars) {
    if (c == *trim_chars) {
      res = 1;
    }
    trim_chars++;
  }
  return res;
}

char *s21_trim(const char *src, const char *trim_chars) {
  if (!src || !trim_chars) return s21_NULL;
  const char *start = src;
  const char *end =
      src + s21_strlen((char *)src) - 1;  // заменить на s21_strlen
  while (*start && is_trim_char(*start, trim_chars)) {
    start++;
  }
  if (*start == '\0') {
    char *empty_str = (char *)malloc(1);
    if (!empty_str) return s21_NULL;
    empty_str[0] = '\0';
    return empty_str;
  }
  while (end > start && is_trim_char(*end, trim_chars)) {
    end--;
  }
  size_t len = end - start + 1;
  char *trimmed_str = (char *)malloc(len + 1);
  if (!trimmed_str) return s21_NULL;
  s21_strncpy(trimmed_str, start, len);
  trimmed_str[len] = '\0';

  return trimmed_str;
}