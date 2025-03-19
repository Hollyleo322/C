#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "cat.h"

flags correct_flags(int argc, char *argv[]);
void print_help();
void catfile(flags a, FILE *file, int *index_str);

int main(int argc, char *argv[]) {
  FILE *file;
  flags res = {0, 0, 0, 0, 0, 0};
  char flag_args = 0;
  int index_args;
  int index_str = 1;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      flag_args = 1;
      index_args = i;
      break;
    }
  }
  res = correct_flags(argc, argv);
  if (flag_args == 0) {
    file = stdin;
    catfile(res, file, &index_str);
    return 0;
  } else if (flag_args == 1 && index_args == argc - 1) {
    file = fopen(argv[index_args], "r");
    if (file && feof(file) == 0) {
      catfile(res, file, &index_str);
      fclose(file);
    } else {
      printf("incorrect filename\n");
    }
  } else {
    for (int i = index_args; i < argc; i++) {
      file = fopen(argv[i], "r");
      if (file && feof(file) == 0) {
        catfile(res, file, &index_str);
        fclose(file);
      } else {
        printf("incorrect filename\n");
      }
    }
  }
  return 0;
}
flags correct_flags(int argc, char *argv[]) {
  int check;
  flags res = {0, 0, 0, 0, 0, 0};
  const struct option long_options[] = {{"number-nonblank)", 0, NULL, 'b'},
                                        {"number", 0, NULL, 'n'},
                                        {"squeeze-blank", 0, NULL, 's'}};
  while ((check = getopt_long(argc, argv, "bevEnstT", long_options, NULL)) !=
         -1) {
    switch (check) {
      case 'b':
        res.num_str_nonempty = 1;
        break;
      case 'e':
        res.cr_like_dollar = 1;
        res.non_printable = 1;
        break;
      case 'E':
        res.cr_like_dollar = 1;
        break;
      case 'n':
        res.num_all_str = 1;
        break;
      case 's':
        res.squezze_empty = 1;
        break;
      case 't':
        res.change_tab = 1;
        res.non_printable = 1;
        break;
      case 'T':
        res.change_tab = 1;
        break;
      case 'v':
        res.non_printable = 1;
        break;
      case '?':
        print_help();
        exit(1);
    }
  }
  return res;
}
void print_help() {
  printf(
      "USAGE: ./s21_cat [options] [file]\navailaible flags -e, -E, -n, -s,-t, "
      "-T, -v, full description like man cat in linux terminal =)\n");
}
void catfile(flags a, FILE *file, int *index_str) {
  char prev = '\n';
  int c = 0;
  char flag_for_s = 0;
  while ((c = fgetc(file)) != EOF) {
    if (a.squezze_empty == 1 && prev == '\n' && c == '\n' && flag_for_s == 1) {
      continue;
    } else {
      if (a.num_str_nonempty == 1 && prev == '\n' && c != '\n') {
        printf("%6d\t", *index_str);
        *index_str += 1;
      }
      if (a.num_all_str == 1 && a.num_str_nonempty == 0 && prev == '\n') {
        printf("%6d\t", *index_str);
        *index_str += 1;
      }
      if (a.squezze_empty == 1 && prev == '\n' && c == '\n') {
        flag_for_s = 1;
      } else {
        flag_for_s = 0;
      }
      if (a.cr_like_dollar == 1 && c == '\n') {
        printf("$");
      }
      if (a.change_tab == 1 && c == '\t') {
        printf("^I");
        prev = c;
        continue;
      }
      if (a.non_printable == 1) {
        if (c < 32 && c != '\n' && c != '\t') {
          printf("^%c", c + 64);
        } else if (c == 127) {
          printf("^%c", c - 64);
        } else if (c > 127 && c < 160) {
          printf("M-^%c", c - 64);
        } else if (c > 159) {
          printf("M-%c", c - 128);
        } else {
          printf("%c", c);
        }
      } else {
        printf("%c", c);
      }
      prev = c;
    }
  }
}