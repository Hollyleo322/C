#define _GNU_SOURCE
#include <ctype.h>
#include <getopt.h>
#include <pcre.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OVECCOUNT 30

#include "s21_grep.h"

int main(int argc, char *argv[]) {
  FILE *f;
  int notone_file = 0;
  int count_options = 0;
  int flag_args = 0;
  int pcre_option = 0;
  flags res = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char *pattern = NULL;
  correct_flags(argc, argv, &res, &count_options, &pcre_option, &pattern);
  if (res.e == 0 && res.f == 0) {
    free(pattern);
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] != '-') {
        flag_args = 1;
        break;
      }
    }
    if (flag_args == 0) {
      print_usage();
    } else if (argc == count_options) {
      f = fopen(argv[count_options], "r");
      if (f && feof(f) == 0) {
        print_usage();
      } else {
        char *single_pattern = malloc(100 * sizeof(char));
        strcpy(single_pattern, argv[count_options]);
        f = stdin;
        grep(single_pattern, f, res, pcre_option, "(standard input)",
             notone_file);
        free(single_pattern);
      }
    } else {
      char *pattern_noarray = malloc(100 * sizeof(char));
      strcpy(pattern_noarray, argv[count_options]);
      count_options += 1;
      if ((argc - (count_options + 1)) > 1) {
        notone_file = 1;
      }
      for (int i = count_options; i < argc; i++) {
        f = fopen(argv[i], "r");
        if (f && feof(f) == 0) {
          grep(pattern_noarray, f, res, pcre_option, argv[i], notone_file);
          fclose(f);
        } else {
          if (res.s == 0) printf("file isn't opened\n");
          free(pattern_noarray);
          exit(1);
        }
      }
      free(pattern_noarray);
    }
  } else {
    if (argc == count_options) {
      f = stdin;
      grep(pattern, f, res, pcre_option, "(standard input)", notone_file);
    } else {
      for (int i = count_options; i < argc; i++) {
        if ((argc - count_options) > 1) {
          notone_file = 1;
        }
        f = fopen(argv[i], "r");
        if (f && feof(f) == 0) {
          grep(pattern, f, res, pcre_option, argv[i], notone_file);
          fclose(f);
        } else {
          free(pattern);
          if (res.s == 0) printf("file isn't opened\n");
          exit(1);
        }
      }
      free(pattern);
    }
  }
  return 0;
}
void correct_flags(int argc, char *argv[], flags *res, int *count_options,
                   int *pcre_options, char **pattern) {
  int check;
  *pattern = malloc(1000 * sizeof(char));
  char *c = *pattern;
  int count_flag_f = 0;
  char flag_for_pattern = 0;
  while ((check = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (check) {
      case 'e':
        if (flag_for_pattern != 0) {
          memcpy(c, "|", 1);
          c += 1;
          res->two_f_or_e = 1;
        }
        res->e = 1;
        flag_for_pattern = 1;
        strcpy(c, optarg);
        c += strlen(optarg);
        res->count_e += 1;
        break;
      case 'i':
        res->i = 1;
        *pcre_options |= PCRE_CASELESS;
        break;
      case 'v':
        res->v = 1;
        break;
      case 'c':
        res->c = 1;
        break;
      case 'l':
        res->l = 1;
        break;
      case 'n':
        res->n = 1;
        break;
      case 'h':
        res->h = 1;
        break;
      case 's':
        res->s = 1;
        break;
      case 'f':
        res->f = 1;
        char *fline = NULL;
        size_t len_fline;
        FILE *f = fopen(optarg, "r");
        if (f && feof(f) == 0) {
          while (getline(&fline, &len_fline, f) != -1) {
            if (fline[0] == '\n') {
              continue;
            }
            if (flag_for_pattern != 0) {
              memcpy(c, "|", 1);
              c += 1;
              res->two_f_or_e = 1;
            }
            if (fline[0] == '(' || fline[0] == ')') {
              memcpy(c, "\\", 1);
              c += 1;
            }
            flag_for_pattern = 1;
            strcpy(c, fline);
            count_flag_f += 1;
            c += strlen(fline) - 1;
          }
          free(fline);
          fclose(f);
        } else {
          printf("incorrect filename\n");
          exit(1);
        }
        break;
      case 'o':
        res->o = 1;
        break;
      case '?':
        print_usage();
        exit(1);
    }
  }
  if (res->e || res->f) {
    c += 1;
    memcpy(c, "\0", 1);
  }
  res->count_e += count_flag_f;
  *count_options = optind;
}
void grep(char *pattern, FILE *f, flags flags, int pcre_options,
          char *name_of_file, int notone_file) {
  int index_for_flag_n = 0;
  char *string = NULL;  // for getline vars
  size_t n = 0;
  int lenstring = 0;
  pcre *re;  // for pcre vars;
  const char *error;
  int erroffset;
  int ovector[OVECCOUNT];
  int rc;
  int count_for_flag_c = 0;
  char end_str[1000] = {0};
  re = pcre_compile(pattern, pcre_options, &error, &erroffset, NULL);
  if (re) {
    while ((lenstring = getline(&string, &n, f)) != -1) {
      rc = pcre_exec(re, NULL, string, lenstring, 0, 0, ovector, OVECCOUNT);
      index_for_flag_n += 1;
      if (string[0] == '\n') {
        continue;
      }
      if (flags.l == 1) {
        if (rc == 1) {
          printf("%s\n", name_of_file);
          break;
        } else if (flags.v == 1 && rc != 1) {
          printf("%s\n", name_of_file);
          break;
        }
      } else {
        if (rc == 1 && flags.c == 1 && flags.v == 0) {
          count_for_flag_c += 1;
        }
        if (rc == 1 && flags.v == 0 && flags.c != 1) {
          if (notone_file == 1 && flags.h != 1 && flags.o == 0) {
            printf("%s:", name_of_file);
          }
          if (flags.n == 1 && flags.o == 0) {
            printf("%d:", index_for_flag_n);
          }
          if (flags.o == 1) {
            print_o(pattern, string, name_of_file, flags, notone_file,
                    index_for_flag_n);
          }
          if (flags.o == 0) {
            printf("%s", string);
            strcpy(end_str, string);
          }
        }
        if (flags.v == 1 && rc != 1 && flags.c != 1 && flags.o == 0) {
          if (notone_file == 1 && flags.h != 1) {
            printf("%s:", name_of_file);
          }
          if (flags.n == 1) {
            printf("%d:", index_for_flag_n);
          }
          printf("%s", string);
          strcpy(end_str, string);
        }
        if (rc != 1 && flags.c == 1 && flags.v == 1) {
          count_for_flag_c += 1;
        }
      }
    }
    pcre_free(re);
    if (strstr(end_str, string)) {
      printf("\n");
    }
    free(string);
  } else {
    printf("PCRE compilation failed at offset %d : %s\n", erroffset, error);
  }
  if (flags.c == 1 && flags.l != 1) {
    if (notone_file == 1 && flags.h != 1) {
      printf("%s:", name_of_file);
    }
    printf("%d\n", count_for_flag_c);
  }
}
void print_usage() {
  printf(
      "Usage: s21_grep [OPTION]... PATTERNS [FILE]...\nTry 'grep --help' for "
      "more "
      "information\n");
}
void print_o(char *pattern, char *string, char *name_of_file, flags flags,
             int notone_file, int index_for_flag_n) {
  if (flags.i == 1) {
    pattern = to_little_letters(pattern);
    string = to_little_letters(string);
  }
  if (flags.two_f_or_e == 1) {
    char **divided_pattern = malloc(sizeof(char *) * (flags.count_e));
    for (int i = 0; i < flags.count_e; i++) {
      divided_pattern[i] = malloc(sizeof(char) * 100);
    }
    int count_delim = 0;
    char *tmp = malloc(sizeof(char) * 100);
    strcpy(tmp, pattern);
    char *delim = strtok(tmp, "|");
    strcpy(divided_pattern[count_delim], delim);
    count_delim += 1;
    while ((delim = strtok(NULL, "|")) != NULL) {
      strcpy(divided_pattern[count_delim], delim);
      count_delim += 1;
    }
    free(tmp);
    for (int i = 0; i < flags.count_e; i++) {
      output_o(divided_pattern[i], string, flags, name_of_file,
               index_for_flag_n, notone_file);
    }
    for (int i = 0; i < flags.count_e; i++) {
      free(divided_pattern[i]);
    }
    free(divided_pattern);
  } else {
    output_o(pattern, string, flags, name_of_file, index_for_flag_n,
             notone_file);
  }
}
char *to_little_letters(char *string) {
  for (size_t i = 0; i < strlen(string); i++) {
    string[i] = tolower(string[i]);
  }
  return string;
}
void output_o(char *pattern, char *string, flags flags, char *name_of_file,
              int index_for_flag_n, int notone_file) {
  if (strchr(pattern, '\\') || strchr(pattern, '^')) {
    pattern = strtok(pattern, "\\^");
  }
  char *c = strstr(string, pattern);
  int count_matches = 0;
  if (c != NULL) {
    count_matches += 1;
    c += strlen(pattern);
    while (c != NULL) {
      c = strstr(c, pattern);
      if (c == NULL) {
        break;
      }
      count_matches += 1;
      c += strlen(pattern);
    }
  }
  for (int i = 0; i < count_matches; i++) {
    if (notone_file == 1 && flags.h != 1) {
      printf("%s:", name_of_file);
    }
    if (flags.n == 1) {
      printf("%d:", index_for_flag_n);
    }
    printf("%s\n", pattern);
  }
}