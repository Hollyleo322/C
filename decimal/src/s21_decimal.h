#pragma once
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define sign_mask 2147483648
#define S21_MAX_UINT 4294967295
#define quantity_bits_tmp_decimal 6
#define quantity_bits 3
#define ten 10
#define bits_of_int 32

typedef struct s21_decimal {
  int bits[4];
} s21_decimal;
typedef struct tmp_decimal {
  int bits[6];
} tmp_decimal;
typedef struct sign_and_exp {
  int sign;
  int exp;
} sign_and_exp;
typedef union dst_bit_3 {
  int mask;
  struct {
    uint32_t empty_2 : 16;
    uint32_t degree : 8;
    uint32_t empty_1 : 7;
    uint32_t sign : 1;
  } parts;
} dst_bit_3;
void zero_sign(s21_decimal *number);
int get_sign(s21_decimal number);
void set_sign(s21_decimal *number);
int get_exp(s21_decimal number);
void set_exp(s21_decimal *number, int exp);
int get_bit(s21_decimal number, int index);
int get_bit_tmp_decimal(tmp_decimal number, int index);
void set_bit(s21_decimal *number, int index);
void set_bit_tmp_decimal(tmp_decimal *number, int index);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
tmp_decimal get_tmp_decimal(s21_decimal number);
int less_tmp_decimal(tmp_decimal value_1, tmp_decimal value_2,
                     s21_decimal number_1, s21_decimal number_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int shift_left_tmp_decimal(tmp_decimal *number, int bits_shift);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
void take_ints_from_str(char array[], int *exp, int *mantiss, char *sign,
                        char *sign_exp);
int find_one(s21_decimal number);
int start_divident_for_shift_right(s21_decimal number, int *index);
int add_bit_shift_right(s21_decimal number, int divident, int *index);
int shift_right(s21_decimal *number);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
tmp_decimal reverse(tmp_decimal number);
tmp_decimal make_mask();
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int decrease_exp(s21_decimal *number);
void zero_decimal(s21_decimal *number);
void zero_tmp_decimal(tmp_decimal *number);
int shift_right(s21_decimal *number);
int count_mask(int bits_shift);
int before_equalize_tmp_decimal(s21_decimal *value_1, s21_decimal *value_2,
                                tmp_decimal *number_1, tmp_decimal *number_2);
int increase_exp_tmp_decimal(s21_decimal *value, tmp_decimal *number);
int shift_right_tmp_decimal(tmp_decimal *number);
int find_one_tmp_decimal(tmp_decimal number);
int check_overflow(tmp_decimal number);
int find_one(s21_decimal number);
int start_divident_for_shift_right(s21_decimal number, int *index);
int start_divident_for_shift_right_tmp_decimal(tmp_decimal number, int *index);
int add_bit_shift_right(s21_decimal number, int divident, int *index);
int add_bit_shift_right_tmp_decimal(tmp_decimal number, int divident,
                                    int *index);
int check_low_or_max_float(float number);
int check_zero(s21_decimal number);
int equalize_exp_tmp_decimal(s21_decimal *value_1, s21_decimal *value_2,
                             tmp_decimal *number_1);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
void take_ints_from_str(char array[], int *exp, int *mantiss, char *sign,
                        char *sign_exp);
char *reverse_float_array(char *array, int dot, int size);
void sum_tmp_decimal(tmp_decimal value_1, tmp_decimal value_2,
                     tmp_decimal *res);
void from_tmp_to_decimal(tmp_decimal sum, s21_decimal *res);
tmp_decimal tmp_one();
int round_tmp_decimal(tmp_decimal *tmp_value, s21_decimal *value_original);
void cycle_for_shift_left_tmp_decimal(tmp_decimal *number, unsigned int mask,
                                      unsigned int shift_mask,
                                      unsigned int previous_mask,
                                      unsigned int bits_shift);
void print_binary_tmp_decimal(tmp_decimal number);
void print_binary_int(int number);
char *array_of_decimal(s21_decimal value, int mul);
void init_decimal(s21_decimal *number, int exp, int mantiss, int sign);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
s21_decimal *setBit(s21_decimal *value, int pos, int bit);
int getSign(s21_decimal value);
s21_decimal *setSign(s21_decimal *value, int bit);
s21_decimal *setScale(s21_decimal *value, int scale);
int increase_exp(s21_decimal *number);
int shift_left(s21_decimal *number, int bits_shift);
int sum_decimal(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
                char flag_sub);

int s21_div(s21_decimal dec1, s21_decimal dec2, s21_decimal *res);
int is_null(s21_decimal dec);
void cleardec(s21_decimal *decimal);
int fract(s21_decimal *dec1, s21_decimal *dec2);
int scaleten(s21_decimal *dec, int tens_count);
int descaleten(s21_decimal *dec, int tens_count);
int set_power(s21_decimal *dec, int power);
int get_power(s21_decimal dec);
int get_start_bit_num(s21_decimal dec);
int compare(s21_decimal dec1, s21_decimal dec2);
int remint(s21_decimal dec1, s21_decimal dec2, s21_decimal *res);
int divs(s21_decimal dec1, s21_decimal dec2, s21_decimal *res);
int adds(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int muls(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int subs(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
s21_decimal remdec(s21_decimal dec1, s21_decimal dec2, s21_decimal *res);

int gb(s21_decimal dec, int bit_num);
int gs(s21_decimal dec);
int sl(s21_decimal *dec, int shift);
int sr(s21_decimal *dec, int shift);
void ss(s21_decimal *dst, int sign);
int sb(s21_decimal *dec, int bit_num, int bit);