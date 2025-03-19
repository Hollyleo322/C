#pragma once

typedef struct flags {
  char e;
  char i;
  char v;
  char c;
  char l;
  char n;
  char h;
  char s;
  char f;
  char o;
  char two_f_or_e;
  int count_e;
} flags;

void grep(char* pattern, FILE* f, flags flags, int pcre_options,
          char* name_of_file, int notone_file);
void correct_flags(int argc, char* argv[], flags* res, int* count_options,
                   int* pcre_options, char** pattern);
void print_usage();
void print_o(char* pattern, char* string, char* name_of_file, flags flags,
             int notone_file, int index_for_flag_n);
char* to_little_letters(char* string);
void output_o(char* pattern, char* string, flags flags, char* name_of_file,
              int index_for_flag_n, int notone_file);