#include "s21_decimal.h"

int get_sign(s21_decimal number)
{
  int res = (number.bits[3] >> 31) & 1;
  return res;
}
void set_sign(s21_decimal *number) { number->bits[3] |= sign_mask; }
void zero_sign(s21_decimal *number) { number->bits[3] &= 2147483647; }
int get_exp(s21_decimal number)
{
  int res = (number.bits[3] >> 16) & 255;
  return res;
}
void set_exp(s21_decimal *number, int exp)
{
  if (!get_sign(*number))
  {
    number->bits[3] = 0;
  }
  else
  {
    number->bits[3] = 0;
    set_sign(number);
  }
  number->bits[3] |= (exp << 16);
}
int get_bit(s21_decimal number, int index)
{
  int res = 0;
  if (index > 31 && index < 64)
  {
    res = (number.bits[1] >> (index % 32)) & 1;
  }
  else if (index > 63)
  {
    res = (number.bits[2] >> (index % 64)) & 1;
  }
  else
  {
    res = (number.bits[0] >> index) & 1;
  }
  return res;
}
int get_bit_tmp_decimal(tmp_decimal number, int index)
{
  int res = 0;
  if (index > 31 && index < 64)
  {
    res = (number.bits[1] >> (index % 32)) & 1;
  }
  else if (index > 63 && index < 96)
  {
    res = (number.bits[2] >> (index % 64)) & 1;
  }
  else if (index > 95 && index < 128)
  {
    res = (number.bits[3] >> (index % 96)) & 1;
  }
  else if (index > 127 && index < 160)
  {
    res = (number.bits[4] >> (index % 128)) & 1;
  }
  else if (index > 159 && index < 192)
  {
    res = (number.bits[5] >> (index % 160)) & 1;
  }
  else
  {
    res = (number.bits[0] >> index) & 1;
  }
  return res;
}
void set_bit(s21_decimal *number, int index)
{
  int value = 1;
  if (index > 31 && index < 64)
  {
    number->bits[1] |= (value << (index % 32));
  }
  else if (index > 63)
  {
    number->bits[2] |= (value << (index % 64));
  }
  else
  {
    number->bits[0] |= (value << (index));
  }
}
void set_bit_tmp_decimal(tmp_decimal *number, int index)
{
  int value = 1;
  if (index > 31 && index < 64)
  {
    number->bits[1] |= (value << (index % 32));
  }
  else if (index > 63 && index < 96)
  {
    number->bits[2] |= (value << (index % 64));
  }
  else if (index > 95 && index < 128)
  {
    number->bits[3] |= (value << (index % 96));
  }
  else if (index > 127 && index < 160)
  {
    number->bits[4] |= (value << (index % 128));
  }
  else if (index > 159 && index < 192)
  {
    number->bits[5] |= (value << (index % 160));
  }
  else
  {
    number->bits[0] |= (value << (index));
  }
}
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
  int res = 0;
  if (result && get_exp(value_1) >= 0 && get_exp(value_1) <= 28 &&
      get_exp(value_2) >= 0 && get_exp(value_2) <= 28)
  {
    zero_decimal(result);
    if (get_sign(value_1) != get_sign(value_2))
    {
      if (get_sign(value_2))
      {
        zero_sign(&value_2);
      }
      else
      {
        set_sign(&value_2);
      }
      res = s21_sub(value_1, value_2, result);
    }
    else
    {
      tmp_decimal number_1 = get_tmp_decimal(value_1),
                  number_2 = get_tmp_decimal(value_2), res_sum;
      zero_tmp_decimal(&res_sum);
      if (get_exp(value_1) != get_exp(value_2))
      {
        int check_equ = before_equalize_tmp_decimal(&value_1, &value_2,
                                                    &number_1, &number_2);
        if (check_equ)
        {
          res = 1;
        }
      }
      if (!res)
      {
        int check_round = 0;
        sum_tmp_decimal(number_1, number_2, &res_sum);
        if (check_overflow(res_sum))
        {
          check_round = round_tmp_decimal(&res_sum, &value_1);
        }
        set_exp(result, get_exp(value_1));
        if (get_exp(value_1) >= 0 && !check_round)
        {
          from_tmp_to_decimal(res_sum, result);
          if (get_sign(value_1))
          {
            set_sign(result);
          }
        }
        else
        {
          if (get_sign(value_1))
          {
            res = 2;
            zero_decimal(result);
          }
          else
          {
            res = 1;
            zero_decimal(result);
          }
        }
      }
      else
      {
        printf("error equalize exp\n");
      }
    }
  }
  else
  {
    printf("incorrect data");
  }
  return res;
}
int round_tmp_decimal(tmp_decimal *tmp_value, s21_decimal *value_original)
{
  char array_of_frac_part[40] = {0};
  int frac_character, i = 0, res = 0;
  while ((check_overflow(*tmp_value) && get_exp(*value_original) > 0) ||
         get_exp(*value_original) > 28)
  {
    frac_character = shift_right_tmp_decimal(tmp_value);
    array_of_frac_part[i] = frac_character + '0';
    set_exp(value_original, get_exp(*value_original) - 1);
    i++;
  }
  i--;
  tmp_decimal cpy = *tmp_value;
  int number_before_dot = shift_right_tmp_decimal(&cpy);
  if (i > -1)
  {
    if (array_of_frac_part[i] == '5')
    {
      char round_it_baby = 0;
      for (int j = 0; j < i; j++)
      {
        if (array_of_frac_part[j] > '0')
        {
          round_it_baby = 1;
          break;
        }
      }
      if (round_it_baby)
      {
        sum_tmp_decimal(*tmp_value, tmp_one(), tmp_value);
      }
      else
      {
        if (number_before_dot % 2 != 0)
        {
          sum_tmp_decimal(*tmp_value, tmp_one(), tmp_value);
        }
      }
    }
    else if (array_of_frac_part[i] > '5')
    {
      sum_tmp_decimal(*tmp_value, tmp_one(), tmp_value);
    }
  }
  if (check_overflow(*tmp_value) && get_exp(*value_original) > 0)
  {
    shift_right_tmp_decimal(tmp_value);
    sum_tmp_decimal(*tmp_value, tmp_one(), tmp_value);
    if (check_overflow(*tmp_value))
    {
      res = 1;
    }
  }
  else if (check_overflow(*tmp_value) && get_exp(*value_original) == 0)
  {
    res = 1;
  }
  return res;
}
tmp_decimal tmp_one()
{
  tmp_decimal res;
  zero_tmp_decimal(&res);
  res.bits[0] = 1;
  return res;
}
void from_tmp_to_decimal(tmp_decimal sum, s21_decimal *res)
{
  for (int i = 0; i < quantity_bits; i++)
  {
    res->bits[i] = sum.bits[i];
  }
}
tmp_decimal get_tmp_decimal(s21_decimal number)
{
  tmp_decimal res;
  zero_tmp_decimal(&res);
  for (int i = 0; i < 3; i++)
  {
    res.bits[i] = number.bits[i];
  }
  return res;
}
int less_tmp_decimal(tmp_decimal value_1, tmp_decimal value_2,
                     s21_decimal number_1, s21_decimal number_2)
{
  int res = 0;
  for (int i = 191; i >= 0; i--)
  {
    if (get_bit_tmp_decimal(value_1, i) != get_bit_tmp_decimal(value_2, i))
    {
      if (get_bit_tmp_decimal(value_1, i) && !get_sign(number_1) &&
          !get_sign(number_2))
      {
        res = 2;
      }
      else if (!get_bit_tmp_decimal(value_1, i) && !get_sign(number_1) &&
               !get_sign(number_2))
      {
        res = 1;
      }
      else if (get_bit_tmp_decimal(value_1, i) && get_sign(number_1) &&
               get_sign(number_2))
      {
        res = 1;
      }
      else
      {
        res = 2;
      }
      break;
    }
  }
  return res;
}
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2)
{
  int res = 0;
  if (get_sign(value_1) == get_sign(value_2))
  {
    tmp_decimal number_1 = get_tmp_decimal(value_1),
                number_2 = get_tmp_decimal(value_2);
    if (get_exp(value_1) != get_exp(value_2))
    {
      before_equalize_tmp_decimal(&value_1, &value_2, &number_1, &number_2);
    }
    res = less_tmp_decimal(number_1, number_2, value_1, value_2);
    if (res)
    {
      res = 1;
    }
  }

  else
  {
    if (check_zero(value_1) && check_zero(value_2))
    {
      res = 1;
    }
  }
  return res;
}
int s21_is_equal(s21_decimal value_1, s21_decimal value_2)
{
  int res = 0;
  if (get_sign(value_1) == get_sign(value_2))
  {
    tmp_decimal number_1 = get_tmp_decimal(value_1),
                number_2 = get_tmp_decimal(value_2);
    if (get_exp(value_1) != get_exp(value_2))
    {
      before_equalize_tmp_decimal(&value_1, &value_2, &number_1, &number_2);
    }
    res = less_tmp_decimal(number_1, number_2, value_1, value_2);
    if (!res)
    {
      res = 1;
    }
    else
    {
      res = 0;
    }
  }
  else
  {
    if (!check_zero(value_1) && !check_zero(value_2))
    {
      res = 1;
    }
  }
  return res;
}
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2)
{
  int res = 0;
  if (get_sign(value_1) == get_sign(value_2))
  {
    tmp_decimal number_1 = get_tmp_decimal(value_1),
                number_2 = get_tmp_decimal(value_2);
    if (get_exp(value_1) != get_exp(value_2))
    {
      before_equalize_tmp_decimal(&value_1, &value_2, &number_1, &number_2);
    }
    res = less_tmp_decimal(number_1, number_2, value_1, value_2);
    if (res == 2 || !res)
    {
      res = 1;
    }
    else
    {
      res = 0;
    }
  }
  else
  {
    if (get_sign(value_2))
    {
      res = 1;
    }
    if (!check_zero(value_1) && !check_zero(value_2))
    {
      res = 1;
    }
  }
  return res;
}
int s21_is_greater(s21_decimal value_1, s21_decimal value_2)
{
  int res = 0;
  if (get_sign(value_1) == get_sign(value_2))
  {
    tmp_decimal number_1 = get_tmp_decimal(value_1),
                number_2 = get_tmp_decimal(value_2);
    if (get_exp(value_1) != get_exp(value_2))
    {
      before_equalize_tmp_decimal(&value_1, &value_2, &number_1, &number_2);
    }
    res = less_tmp_decimal(number_1, number_2, value_1, value_2);
    if (res != 2)
    {
      res = 0;
    }
    else
    {
      res = 1;
    }
  }
  else
  {
    if (!get_sign(value_1) && get_sign(value_2))
    {
      res = 1;
    }
    if (!check_zero(value_1) && !check_zero(value_2))
    {
      res = 0;
    }
  }
  return res;
}
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2)
{
  int res = 0;
  if (get_sign(value_1) == get_sign(value_2))
  {
    tmp_decimal number_1 = get_tmp_decimal(value_1),
                number_2 = get_tmp_decimal(value_2);
    if (get_exp(value_1) != get_exp(value_2))
    {
      before_equalize_tmp_decimal(&value_1, &value_2, &number_1, &number_2);
    }
    res = less_tmp_decimal(number_1, number_2, value_1, value_2);
    if (!res || res == 1)
    {
      res = 1;
    }
    else
    {
      res = 0;
    }
  }
  else
  {
    if (get_sign(value_1))
    {
      res = 1;
    }
    if (!check_zero(value_1) && !check_zero(value_2))
    {
      res = 1;
    }
  }
  return res;
}
int s21_is_less(s21_decimal value_1, s21_decimal value_2)
{
  int res = 0;
  if (get_sign(value_1) == get_sign(value_2))
  {
    tmp_decimal number_1 = get_tmp_decimal(value_1),
                number_2 = get_tmp_decimal(value_2);
    if (get_exp(value_1) != get_exp(value_2))
    {
      before_equalize_tmp_decimal(&value_1, &value_2, &number_1, &number_2);
    }
    res = less_tmp_decimal(number_1, number_2, value_1, value_2);
    if (res != 1)
    {
      res = 0;
    }
  }
  else
  {
    if (get_sign(value_1) && !get_sign(value_2))
    {
      res = 1;
    }
    if (!check_zero(value_1) && !check_zero(value_2))
    {
      res = 0;
    }
  }
  return res;
}
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
  int res = 0;
  if (result && get_exp(value_1) >= 0 && get_exp(value_1) <= 28 &&
      get_exp(value_2) >= 0 && get_exp(value_2) <= 28)
  {
    zero_decimal(result);
    if ((get_sign(value_1) && !get_sign(value_2)) ||
        (!get_sign(value_1) && get_sign(value_2)))
    {
      set_sign(result);
    }
    if (!check_zero(value_1))
    {
      *result = value_2;
    }
    else if (!check_zero(value_2))
    {
      *result = value_1;
    }
    else
    {
      set_exp(result, get_exp(value_1) + get_exp(value_2));
      tmp_decimal number_1 = get_tmp_decimal(value_1), res_tmp_decimal, sum;
      zero_tmp_decimal(&res_tmp_decimal), zero_tmp_decimal(&sum);
      for (int i = 0; i < 96; i++)
      {
        if (get_bit(value_2, i))
        {
          shift_left_tmp_decimal(&number_1, i);
          sum_tmp_decimal(res_tmp_decimal, number_1, &sum);
          res_tmp_decimal = sum;
          zero_tmp_decimal(&sum);
          number_1 = get_tmp_decimal(value_1);
        }
      }
      int check_round = 0;
      if (check_overflow(res_tmp_decimal))
      {
        check_round = round_tmp_decimal(&res_tmp_decimal, result);
      }
      if (get_exp(*result) >= 0 && !check_round)
      {
        from_tmp_to_decimal(res_tmp_decimal, result);
      }
      else
      {
        if (get_sign(*result))
        {
          res = 2;
          zero_decimal(result);
        }
        else
        {
          res = 1;
          zero_decimal(result);
        }
      }
    }
  }
  else
  {
    printf("incorrect data\n");
  }
  return res;
}
int before_equalize_tmp_decimal(s21_decimal *value_1, s21_decimal *value_2,
                                tmp_decimal *number_1, tmp_decimal *number_2)
{
  int res = 0;
  if (get_exp(*value_1) < get_exp(*value_2))
  {
    res = equalize_exp_tmp_decimal(value_1, value_2, number_1);
  }
  else
  {
    res = equalize_exp_tmp_decimal(value_2, value_1, number_2);
  }
  return res;
}
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
  int res = 0;
  if (result && get_exp(value_1) >= 0 && get_exp(value_1) <= 28 &&
      get_exp(value_2) >= 0 && get_exp(value_2) <= 28)
  {
    zero_decimal(result);
    if (get_sign(value_2))
    {
      zero_sign(&value_2);
    }
    else if (!get_sign(value_2))
    {
      set_sign(&value_2);
    }
    if (get_sign(value_1) == get_sign(value_2))
    {
      res = s21_add(value_1, value_2, result);
    }
    else
    {
      tmp_decimal number_1 = get_tmp_decimal(value_1),
                  number_2 = get_tmp_decimal(value_2), res_sum;
      zero_tmp_decimal(&res_sum);
      if (get_exp(value_1) != get_exp(value_2))
      {
        int check_equ = before_equalize_tmp_decimal(&value_1, &value_2,
                                                    &number_1, &number_2);
        if (check_equ)
        {
          res = 1;
        }
      }
      if (!res)
      {
        int sign1 = get_sign(value_1), sign2 = get_sign(value_2);
        zero_sign(&value_1);
        zero_sign(&value_2);
        if (less_tmp_decimal(number_1, number_2, value_1, value_2) == 2)
        {
          number_2 = reverse(number_2);
          if (sign1)
          {
            set_sign(result);
          }
        }
        else if (less_tmp_decimal(number_1, number_2, value_1, value_2) ==
                 0)
        {
          number_2 = reverse(number_2);
        }
        else
        {
          number_1 = reverse(number_1);
          if (sign2)
          {
            set_sign(result);
          }
        }
        int check_round = 0;
        sum_tmp_decimal(number_1, number_2, &res_sum);
        if (check_overflow(res_sum))
        {
          check_round = round_tmp_decimal(&res_sum, &value_1);
        }
        set_exp(result, get_exp(value_1));
        if (get_exp(value_1) >= 0 && !check_round)
        {
          from_tmp_to_decimal(res_sum, result);
        }
        else
        {
          if (get_sign(value_1))
          {
            res = 2;
            zero_decimal(result);
          }
          else
          {
            res = 1;
            zero_decimal(result);
          }
        }
      }
      else
      {
        printf("error equalize exp\n");
      }
    }
  }
  else
  {
    printf("incorrect data");
  }
  return res;
}
tmp_decimal reverse(tmp_decimal number)
{
  tmp_decimal mask = make_mask();
  tmp_decimal one = {0}, res = {0};
  one.bits[0] |= 1;
  for (int i = 0; i < quantity_bits_tmp_decimal; i++)
  {
    number.bits[i] = (~number.bits[i]) & mask.bits[i];
  }
  sum_tmp_decimal(number, one, &res);
  return res;
}
tmp_decimal make_mask()
{
  tmp_decimal res = {0};
  for (int i = 191; i >= 0; i--)
  {
    set_bit_tmp_decimal(&res, i);
  }
  return res;
}
int decrease_exp(s21_decimal *number)
{
  int res = 0;
  sign_and_exp value_4th_bit = {get_sign(*number), get_exp(*number)};
  if (!value_4th_bit.exp)
  {
    res = 1;
  }
  else
  {
    shift_right(number);
    set_exp(number, value_4th_bit.exp - 1);
  }
  return res;
}
void zero_decimal(s21_decimal *number)
{
  for (int i = 0; i < 4; i++)
  {
    number->bits[i] = 0;
  }
}
int shift_left(s21_decimal *number, int bits_shift)
{
  s21_decimal copy_number = *number;
  unsigned int mask = 0, res = 0, shift_mask = 0, previous_mask = 0;
  shift_mask = count_mask(bits_shift);
  for (int i = 0; i < quantity_bits; i++)
  {
    mask = number->bits[i] & (shift_mask << (32 - bits_shift));
    mask >>= (32 - bits_shift);
    number->bits[i] <<= bits_shift;
    number->bits[i] |= previous_mask;
    previous_mask = mask;
  }
  if (mask)
  {
    res = 1;
    *number = copy_number;
  }
  return res;
}
int sum_decimal(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
                char flag_sub)
{
  int sum_bits = 0, memo = 0, res = 0;
  for (int i = 0; i < 96; i++)
  {
    sum_bits = get_bit(value_1, i) + get_bit(value_2, i);
    if (memo)
    {
      sum_bits += memo;
      memo = 0;
    }
    if (sum_bits > 1)
    {
      memo = 1;
    }
    if (sum_bits % 2)
    {
      set_bit(result, i);
    }
  }
  if (memo && get_sign(value_1) && !flag_sub)
  {
    res = 2;
  }
  else if (memo && !get_sign(value_1) && !flag_sub)
  {
    res = 1;
  }
  return res;
}
void zero_tmp_decimal(tmp_decimal *number)
{
  for (int i = 0; i < 6; i++)
  {
    number->bits[i] = 0;
  }
}
int shift_left_tmp_decimal(tmp_decimal *number, int bits_shift)
{
  tmp_decimal copy_number = *number;
  unsigned int res = 0;
  unsigned int mask = 0, shift_mask = 0, previous_mask = 0;
  if (bits_shift > 32 && bits_shift <= 64)
  {
    cycle_for_shift_left_tmp_decimal(number, mask, count_mask(bits_of_int),
                                     previous_mask, bits_of_int);
    bits_shift -= 32;
    cycle_for_shift_left_tmp_decimal(number, 0, count_mask(bits_shift), 0,
                                     bits_shift);
  }
  else if (bits_shift > 64)
  {
    for (int i = 0; i < 2; i++)
    {
      cycle_for_shift_left_tmp_decimal(number, mask, count_mask(bits_of_int),
                                       previous_mask, bits_of_int);
      mask = 0, previous_mask = 0;
    }
    bits_shift -= 64;
    cycle_for_shift_left_tmp_decimal(number, 0, count_mask(bits_shift), 0,
                                     bits_shift);
  }
  else
  {
    shift_mask = count_mask(bits_shift);
    cycle_for_shift_left_tmp_decimal(number, mask, shift_mask, previous_mask,
                                     bits_shift);
  }
  if (mask)
  {
    res = 1;
    *number = copy_number;
  }
  return res;
}
void cycle_for_shift_left_tmp_decimal(tmp_decimal *number, unsigned int mask,
                                      unsigned int shift_mask,
                                      unsigned int previous_mask,
                                      unsigned int bits_shift)
{
  if (bits_shift != 32)
  {
    for (int i = 0; i < quantity_bits_tmp_decimal; i++)
    {
      mask = number->bits[i] & (shift_mask << (32 - bits_shift));
      mask >>= (32 - bits_shift);
      number->bits[i] <<= bits_shift;
      number->bits[i] |= previous_mask;
      previous_mask = mask;
    }
  }
  else
  {
    for (int i = 0; i < quantity_bits_tmp_decimal; i++)
    {
      mask = number->bits[i] & (shift_mask << (32 - bits_shift));
      mask >>= (32 - bits_shift);
      number->bits[i] &= 0;
      number->bits[i] |= previous_mask;
      previous_mask = mask;
    }
  }
}
int shift_right(s21_decimal *number)
{
  s21_decimal res = {0};
  char flag_while = 0;
  int mask = 6, divident, index = find_one(*number), remainder,
      index_for_res = 95;
  divident = start_divident_for_shift_right(*number, &index);
  if (index >= 0)
  {
    if (divident >= ten)
    {
      remainder = (divident + mask) & 15;
      set_bit(&res, index_for_res);
      index_for_res -= 1;
    }
    else
    {
      divident = add_bit_shift_right(*number, divident, &index);
      remainder = (divident + mask) & 15;
      set_bit(&res, index_for_res);
      if (index >= 0)
      {
        index_for_res -= 1;
      }
    }
    while (!flag_while)
    {
      if (remainder < ten && index >= 0)
      {
        remainder = add_bit_shift_right(*number, remainder, &index);
      }
      if (remainder >= ten)
      {
        remainder = (remainder + mask) & 15;
        set_bit(&res, index_for_res);
        if (index >= 0)
        {
          index_for_res -= 1;
        }
      }
      else
      {
        if (index >= 0)
        {
          index_for_res -= 1;
        }
      }
      if (!index)
      {
        remainder = add_bit_shift_right(*number, remainder, &index);
        if (remainder >= ten)
        {
          remainder = (remainder + mask) & 15;
          set_bit(&res, index_for_res);
        }
        flag_while = 1;
      }
      if (index < 0)
      {
        flag_while = 1;
      }
    }
    zero_decimal(number);
    for (int i = index_for_res, j = 0; i < 96; i++, j++)
    {
      if (get_bit(res, i))
      {
        set_bit(number, j);
      }
    }
  }
  else if (index == -1)
  {
    if (divident >= 10)
    {
      remainder = (divident + mask) & 15;
      zero_decimal(number);
      set_bit(number, 0);
    }
    else
    {
      remainder = divident;
      zero_decimal(number);
    }
  }
  else
  {
    remainder = divident;
    zero_decimal(number);
  }
  return remainder;
}
int count_mask(int bits_shift)
{
  int res = 0;
  if (bits_shift == 32)
  {
    res = -1;
  }
  else
  {
    for (int i = 0; i < bits_shift; i++)
    {
      res += pow(2, i);
    }
  }
  return res;
}
int shift_right_tmp_decimal(tmp_decimal *number)
{
  tmp_decimal res = {0};
  char flag_while = 0;
  int mask = 6, divident, index = find_one_tmp_decimal(*number), remainder,
      index_for_res = 191;
  divident = start_divident_for_shift_right_tmp_decimal(*number, &index);
  if (index >= 0)
  {
    if (divident >= ten)
    {
      remainder = (divident + mask) & 15;
      set_bit_tmp_decimal(&res, index_for_res);
      index_for_res -= 1;
    }
    else
    {
      divident = add_bit_shift_right_tmp_decimal(*number, divident, &index);
      remainder = (divident + mask) & 15;
      set_bit_tmp_decimal(&res, index_for_res);
      if (index >= 0)
      {
        index_for_res -= 1;
      }
    }
    while (!flag_while)
    {
      if (remainder < ten && index >= 0)
      {
        remainder = add_bit_shift_right_tmp_decimal(*number, remainder, &index);
      }
      if (remainder >= ten)
      {
        remainder = (remainder + mask) & 15;
        set_bit_tmp_decimal(&res, index_for_res);
        if (index >= 0)
        {
          index_for_res -= 1;
        }
      }
      else
      {
        if (index >= 0)
        {
          index_for_res -= 1;
        }
      }
      if (!index)
      {
        remainder = add_bit_shift_right_tmp_decimal(*number, remainder, &index);
        if (remainder >= ten)
        {
          remainder = (remainder + mask) & 15;
          set_bit_tmp_decimal(&res, index_for_res);
        }
        flag_while = 1;
      }
      if (index < 0)
      {
        flag_while = 1;
      }
    }
    zero_tmp_decimal(number);
    for (int i = index_for_res, j = 0; i < 192; i++, j++)
    {
      if (get_bit_tmp_decimal(res, i))
      {
        set_bit_tmp_decimal(number, j);
      }
    }
  }
  else if (index == -1)
  {
    if (divident >= 10)
    {
      remainder = (divident + mask) & 15;
      zero_tmp_decimal(number);
      set_bit_tmp_decimal(number, 0);
    }
    else
    {
      remainder = divident;
      zero_tmp_decimal(number);
    }
  }
  else
  {
    remainder = divident;
    zero_tmp_decimal(number);
  }
  return remainder;
}
int find_one_tmp_decimal(tmp_decimal number)
{
  int res = 0, bit;
  for (int i = 191; i >= 0; i--)
  {
    bit = get_bit_tmp_decimal(number, i);
    if (bit)
    {
      res = i;
      break;
    }
  }
  return res;
}
int check_overflow(tmp_decimal number)
{
  int res = 0;
  for (int i = 5; i != 2; i--)
  {
    if (number.bits[i] != 0)
    {
      res = 1;
      break;
    }
  }
  return res;
}

int s21_div(s21_decimal dec1, s21_decimal dec2, s21_decimal *res)
{
  int err = 0;
  if (is_null(dec2))
  {
    err = 3;
  }
  else
  {
    cleardec(res);
    fract(&dec1, &dec2);

    s21_decimal quotient, remainder;
    cleardec(&quotient);
    cleardec(&remainder);
    remainder = remdec(dec1, dec2, &quotient);
    s21_decimal temp_quotient;
    int shift_count = 0;
    int scale_overflow = 0;
    while (!is_null(remainder))
    {
      scale_overflow = scaleten(&remainder, 1);
      if (scale_overflow)
      {
        descaleten(&dec2, 1);
      }
      cleardec(&temp_quotient);

      remainder = remdec(remainder, dec2, &temp_quotient);
      if (scaleten(&quotient, 1) != 0)
        break;
      shift_count++;
      if (adds(quotient, temp_quotient, &quotient) != 0)
        break;
    }

    *res = quotient;
    set_power(res, shift_count);
    if (gs(dec1) * gs(dec2) < 0)
    {
      ss(res, 1);
    }
  }
  return err;
}

int is_null(s21_decimal dec)
{
  int res = 1;
  if (dec.bits[0] != 0 || dec.bits[1] != 0 || dec.bits[2] != 0)
    res = 0;
  return res;
}

void cleardec(s21_decimal *decimal)
{
  decimal->bits[0] = 0;
  decimal->bits[1] = 0;
  decimal->bits[2] = 0;
  decimal->bits[3] = 0;
}

int fract(s21_decimal *dec1, s21_decimal *dec2)
{
  int pow1 = get_power(*dec1);
  int pow2 = get_power(*dec2);
  int overflow;

  if (pow1 < pow2)
  {
    overflow = scaleten(dec1, (pow2 - pow1));
    if (overflow)
    {
      descaleten(dec2, 1);
      fract(dec1, dec2);
    }
  }
  else if (pow1 > pow2)
  {
    overflow = scaleten(dec2, (pow1 - pow2));
    if (overflow)
    {
      descaleten(dec1, 1);
      fract(dec1, dec2);
    }
  }

  return 0;
}

int scaleten(s21_decimal *dec, int tens_count)
{
  int pow = get_power(*dec);
  int err;
  s21_decimal dec_backup = *dec;
  for (int tens = 1; tens <= tens_count; tens++)
  {
    s21_decimal temp1 = *dec;
    s21_decimal temp2 = *dec;
    if ((err = sl(&temp1, 3) != 0))
      break;
    sl(&temp2, 1);

    if ((err = adds(temp1, temp2, dec) != 0))
      break;
    pow++;
  }
  err += (set_power(dec, pow) != 0);
  if (err)
    *dec = dec_backup;
  return err;
}

int descaleten(s21_decimal *dec, int tens_count)
{
  int err = 0;
  const s21_decimal dec10 = {.bits = {10, 0, 0, 0}};
  s21_decimal trash;
  int pow = get_power(*dec);

  for (int tens = 1; tens <= tens_count; tens++)
  {
    s21_decimal round_unit = {.bits = {0, 0, 0, 0}};
    int remainder = remint(*dec, dec10, dec);
    if (remainder == 5)
    {
      int next_digit = remint(*dec, dec10, &trash);
      if (next_digit % 2 != 0)
      {
        round_unit.bits[0] = 1;
      }
    }
    else if (remainder > 5)
    {
      round_unit.bits[0] = 1;
    }
    adds(*dec, round_unit, dec);
    pow--;
  }

  if (pow >= 0)
  {
    set_power(dec, pow);
  }
  else
  {
    set_power(dec, 0);
    err = 1;
  }
  return err;
}

int get_power(s21_decimal dec) { return (dec.bits[3] & 16711680) >> 16; }
int set_power(s21_decimal *dec, int power)
{
  int res = 0;

  if ((0 <= power) && (power <= 28))
  {
    dec->bits[3] &= ~16711680;
    dec->bits[3] |= power << 16;
  }
  else
  {
    res = 1;
  }
  return res;
}

int get_start_bit_num(s21_decimal dec)
{
  int res = 0;
  for (int i = 95; i > 0; i--)
  {
    if (gb(dec, i) == 1)
    {
      res = i;
      break;
    }
  }
  return res;
}

int compare(s21_decimal dec1, s21_decimal dec2)
{
  int res = 0;
  int i;
  for (i = 95; i >= 0; i--)
  {
    if (gb(dec1, i) > gb(dec2, i))
    {
      res = 1;
      break;
    }
    else if (gb(dec1, i) < gb(dec2, i))
    {
      res = -1;
      break;
    }
  }
  return res;
}

s21_decimal remdec(s21_decimal dec1, s21_decimal dec2, s21_decimal *res)
{
  s21_decimal remainder;
  cleardec(res);
  cleardec(&remainder);

  if (!is_null(dec2))
  {
    s21_decimal temp_res;
    cleardec(&temp_res);
    divs(dec1, dec2, res);
    muls(*res, dec2, &temp_res);
    subs(dec1, temp_res, &remainder);
  }
  if (gs(dec1) * gs(dec2) < 0)
    ss(res, 1);
  return remainder;
}

int remint(s21_decimal dec1, s21_decimal dec2, s21_decimal *res)
{
  s21_decimal dec_rem = remdec(dec1, dec2, res);
  int int_rem = dec_rem.bits[0];
  return int_rem;
}

int divs(s21_decimal dec1, s21_decimal dec2, s21_decimal *res)
{
  cleardec(res);
  int err = 0;
  if (is_null(dec2))
  {
    err = -1;
  }
  else
  {
    int dividend_start_bit_num = get_start_bit_num(dec1);
    int divider_start_bit_num = get_start_bit_num(dec2);
    if (dividend_start_bit_num >= divider_start_bit_num)
    {
      s21_decimal divider = dec2;
      s21_decimal dividend = dec1;

      int shift_count = dividend_start_bit_num - divider_start_bit_num;
      sl(&divider, shift_count);

      for (int i = shift_count; i >= 0; i--)
      {
        if (compare(divider, dividend) <= 0)
        {
          subs(dividend, divider, &dividend);
          sb(res, i, 1);
        }
        sr(&divider, 1);
      }
    }
  }
  return err;
}

int muls(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
  cleardec(result);
  int err;
  int start_val2_bit = get_start_bit_num(value_2);
  for (int i = 0; i <= start_val2_bit; i++)
  {
    if (gb(value_2, i) == 1)
    {
      if ((err = adds(*result, value_1, result) != 0))
        break;
    }
    if (shift_left(&value_1, 1) != 0 && i != start_val2_bit)
    {
      err = 1;
      break;
    }
  }
  return err;
}

int subs(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
  if (compare(value_1, value_2) < 0)
  {
    s21_decimal temp = value_1;
    value_1 = value_2;
    value_2 = temp;
    ss(result, 1);
  }
  int mem = 0;
  int bit_res;
  for (int i = 0; i < 96; i++)
  {
    int bit1 = gb(value_1, i);
    int bit2 = gb(value_2, i) + mem;

    if (bit1 >= bit2)
    {
      bit_res = bit1 - bit2;
      mem = 0;
    }
    else
    {
      bit_res = (bit1 + 2) - bit2;
      mem = 1;
    }
    sb(result, i, bit_res);
  }

  return 0;
}

int adds(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
  int mem = 0;
  int err = 0;
  for (int i = 0; i < 96; i++)
  {
    int bit1 = gb(value_1, i) + mem;
    int bit2 = gb(value_2, i);
    int addition = bit1 + bit2;
    int bit_res = addition & 0x00000001;
    mem = (addition & 0x00000002) >> 1;
    sb(result, i, bit_res);
  }
  if (mem != 0)
    err = 1;

  return err;
}

int gb(s21_decimal dec, int bit_num)
{
  int block_num = bit_num / 32;
  bit_num %= 32;

  return (dec.bits[block_num] >> bit_num) & 1;
}

int sb(s21_decimal *dec, int bit_num, int bit)
{
  int err = 0;
  if (bit_num >= 0 && bit_num < 96)
  {
    int block_num = bit_num / 32;
    bit_num %= 32;

    uint32_t mask = (1 << bit_num);
    dec->bits[block_num] &= ~mask;
    dec->bits[block_num] |= (bit << bit_num);
  }
  else
  {
    err = 1;
  }
  return err;
}

int sr(s21_decimal *dec, int shift)
{
  for (int i = 0; i < shift; i++)
  {
    int borrow = 0;
    for (int j = 3; j >= 0; j--)
    {
      unsigned int new_borrow = (dec->bits[j] & 1) ? 0x80000000 : 0;
      dec->bits[j] >>= 1;
      dec->bits[j] |= borrow;
      borrow = new_borrow;
    }
  }
  return 0;
}

int sl(s21_decimal *dec, int shift)
{
  int err = 0;
  int available_space = 96 - get_start_bit_num(*dec) - 1;
  if (shift <= available_space)
  {
    for (int i = 0; i < shift; i++)
    {
      int carry = 0;
      for (int j = 0; j < 3; j++)
      {
        unsigned int new_carry = (dec->bits[j] & 0x80000000) ? 1 : 0;
        dec->bits[j] <<= 1;
        dec->bits[j] |= carry;
        carry = new_carry;
      }
    }
  }
  else
  {
    err = 1;
  }
  return err;
}

int gs(s21_decimal dec) { return (dec.bits[3] >> 31) == 1 ? -1 : 1; }

void ss(s21_decimal *dst, int sign) { dst->bits[3] |= sign << 31; }

int find_one(s21_decimal number)
{
  int res = 0, bit;
  for (int i = 95; i >= 0; i--)
  {
    bit = get_bit(number, i);
    if (bit)
    {
      res = i;
      break;
    }
  }
  return res;
}
int start_divident_for_shift_right(s21_decimal number, int *index)
{
  int res = 0;
  for (int i = 0; i < 4; i++, (*index)--)
  {
    if (*index < 0)
    {
      break;
    }
    res |= get_bit(number, *index);
    if (i != 3 && *index)
    {
      res <<= 1;
    }
  }
  return res;
}
int start_divident_for_shift_right_tmp_decimal(tmp_decimal number, int *index)
{
  int res = 0;
  for (int i = 0; i < 4; i++, (*index)--)
  {
    if (*index < 0)
    {
      break;
    }
    res |= get_bit_tmp_decimal(number, *index);
    if (i != 3 && *index)
    {
      res <<= 1;
    }
  }
  return res;
}
int add_bit_shift_right(s21_decimal number, int divident, int *index)
{
  divident <<= 1;
  divident |= get_bit(number, *index);
  *index -= 1;
  return divident;
}
int add_bit_shift_right_tmp_decimal(tmp_decimal number, int divident,
                                    int *index)
{
  divident <<= 1;
  divident |= get_bit_tmp_decimal(number, *index);
  *index -= 1;
  return divident;
}
int check_zero(s21_decimal number)
{
  int res = 0;
  for (int i = 0; i < quantity_bits; i++)
  {
    if (number.bits[i] != 0)
    {
      res = 1;
      break;
    }
  }
  return res;
}
int equalize_exp_tmp_decimal(s21_decimal *value_1, s21_decimal *value_2,
                             tmp_decimal *number_1)
{
  int res = 0;
  if (check_zero(*value_1) && check_zero(*value_2))
  {
    while (get_exp(*value_1) < get_exp(*value_2))
    {
      int check_increase = increase_exp_tmp_decimal(value_1, number_1);
      if (check_increase)
      {
        res = 1;
        break;
      }
    }
  }
  return res;
}
int increase_exp(s21_decimal *number)
{
  sign_and_exp value_4th_bit = {get_sign(*number), get_exp(*number)};
  int res = 0;
  s21_decimal cpy_1 = *number, cpy_2 = *number;
  int check_1 = shift_left(&cpy_1, 1);
  int check_2 = shift_left(&cpy_2, 3);
  if (!check_1 && !check_2)
  {
    s21_decimal sum = {0};
    int check_sum = sum_decimal(cpy_1, cpy_2, &sum, 0);
    *number = sum;
    if (check_sum)
    {
      res = 1;
    }
    if (value_4th_bit.sign)
    {
      set_sign(number);
    }
    set_exp(number, value_4th_bit.exp + 1);
  }
  else
  {
    res = 1;
  }
  return res;
}
int increase_exp_tmp_decimal(s21_decimal *value, tmp_decimal *number)
{
  int res = 0;
  tmp_decimal cpy_1 = *number, cpy_2 = *number, sum;
  zero_tmp_decimal(&sum);
  int check_1 = shift_left_tmp_decimal(&cpy_1, 1);
  int check_2 = shift_left_tmp_decimal(&cpy_2, 3);
  if (!check_1 && !check_2)
  {
    sum_tmp_decimal(cpy_1, cpy_2, &sum);
    set_exp(value, get_exp(*value) + 1);
    *number = sum;
  }
  else
  {
    res = 1;
  }
  return res;
}
void sum_tmp_decimal(tmp_decimal value_1, tmp_decimal value_2,
                     tmp_decimal *res)
{
  zero_tmp_decimal(res);
  int sum_bits = 0, memo = 0;
  for (int i = 0; i < 192; i++)
  {
    sum_bits =
        get_bit_tmp_decimal(value_1, i) + get_bit_tmp_decimal(value_2, i);
    if (memo)
    {
      sum_bits += memo;
      memo = 0;
    }
    if (sum_bits > 1)
    {
      memo = 1;
    }
    if (sum_bits % 2)
    {
      set_bit_tmp_decimal(res, i);
    }
  }
}
int s21_from_int_to_decimal(int src, s21_decimal *dst)
{
  int res = 0;
  if (dst)
  {
    zero_decimal(dst);
    if (src < 0)
    {
      src *= -1;
      set_sign(dst);
    }
    dst->bits[0] |= src;
  }
  else
  {
    res = 1;
  }
  return res;
}
int s21_from_decimal_to_int(s21_decimal src, int *dst)
{
  int res = 0;
  *dst = 0;
  if ((src.bits[1] != 0 && src.bits[2] != 0) || !dst)
  {
    res = 1;
  }
  else
  {
    int sign = get_sign(src);
    if (get_exp(src))
    {
      int quantity_iteration = get_exp(src);
      for (int i = 0; i < quantity_iteration; i++)
      {
        decrease_exp(&src);
      }
    }
    *dst = src.bits[0];
    if (sign)
    {
      *dst *= -1;
    }
  }
  return res;
}
int check_low_or_max_float(float number)
{
  int res = 0;
  if (number < 0)
  {
    number *= -1;
  }
  if (number < 1e-28)
  {
    res = 1;
  }
  if (number > 1e+28)
  {
    res = 1;
  }
  return res;
}
int s21_from_float_to_decimal(float src, s21_decimal *dst)
{
  int res = 0;
  zero_decimal(dst);
  int check_float = check_low_or_max_float(src);
  if (src == INFINITY || src == -INFINITY || isnan(src) || check_float ||
      !dst)
  {
    res = 1;
  }
  else
  {
    zero_decimal(dst);
    char float_str[100], sign = 0, sign_exp = 0;
    int exp = 0, mantiss = 0;
    sprintf(float_str, "%.7g", src);
    printf("%s\n", float_str);
    take_ints_from_str(float_str, &exp, &mantiss, &sign, &sign_exp);
    dst->bits[0] = mantiss;
    if (sign)
    {
      set_sign(dst);
    }
    if (sign_exp == '+')
    {
      for (int i = 0; i < exp; i++)
      {
        int check_increase = increase_exp(dst);
        if (check_increase)
        {
          res = 1;
          break;
        }
      }
      set_exp(dst, 0);
    }
    if (sign_exp == '-' && exp <= 28)
    {
      set_exp(dst, exp);
    }
    else if (sign_exp == '-' && exp > 28)
    {
      res = 1;
    }
    if (!sign_exp)
    {
      set_exp(dst, exp);
    }
  }
  return res;
}
void take_ints_from_str(char array[], int *exp, int *mantiss, char *sign,
                        char *sign_exp)
{
  if (array[0] == '-')
  {
    *sign = 1;
    array += 1;
  }
  char array_mantiss[100];
  char array_exp[10];
  char flag_for_exp = 0, flag_after_dot = 0;
  int index_mantiss = 0, index_exp = 0;
  for (int i = 0; array[i]; i++)
  {
    if (flag_after_dot)
    {
      *exp += 1;
    }
    if (array[i] == 'e')
    {
      flag_after_dot = 0, index_exp = 0;
      *sign_exp = array[i + 1];
      flag_for_exp = 1;
      i += 2;
    }
    if (array[i] == '.')
    {
      flag_after_dot = 1;
      continue;
    }
    if (!flag_for_exp)
    {
      array_mantiss[index_mantiss] = array[i];
      index_mantiss += 1;
    }
    else if (flag_for_exp)
    {
      array_exp[index_exp] = array[i];
      index_exp += 1;
    }
  }
  array_mantiss[index_mantiss] = '\0';
  array_exp[index_exp] = '\0';
  sscanf(array_mantiss, "%d", mantiss);
  if (flag_for_exp)
  {
    sscanf(array_exp, "%d", exp);
    *exp -= strlen(array_mantiss) - 1;
  }
}
int s21_from_decimal_to_float(s21_decimal src, float *dst)
{
  int res = 0;
  char float_array[100];
  if (!check_zero(src))
  {
    *dst = 0;
  }
  else
  {
    if (dst && (get_exp(src) >= 0 && get_exp(src) <= 28))
    {
      int i = 0;
      int exp = get_exp(src), sign = get_sign(src);
      while (check_zero(src))
      {
        float_array[i] = shift_right(&src) + '0';
        i++;
      }
      float_array[i] = '\0';
      char *res_array = reverse_float_array(float_array, exp, i);
      sscanf(res_array, "%f", dst);
      if (sign)
      {
        *dst *= -1;
      }
    }
    else
    {
      res = 1;
    }
  }
  return res;
}
char *reverse_float_array(char *array, int dot, int size)
{
  static char res[100];
  int j = 0;
  if (size <= dot)
  {
    res[0] = '0';
    res[1] = '.';
    j += 2;
    if (size < dot)
    {
      for (int i = 0; i < dot - size; i++)
      {
        res[j] = '0';
        j++;
      }
    }
  }
  for (int i = size - 1; i >= 0; i--)
  {
    res[j] = array[i];
    if (size - (j + 1) == dot)
    {
      res[j + 1] = '.';
      j += 2;
      continue;
    }
    j++;
  }
  res[j] = '\0';
  return res;
}
char *array_of_decimal(s21_decimal value, int mul)
{
  static char array[100];
  char tmp_array[100];
  int i = 0, k = 0, exp = get_exp(value);
  if (get_sign(value))
  {
    array[k] = '-';
    k++;
  }
  while (check_zero(value))
  {
    tmp_array[i] = shift_right(&value) + '0';
    i++;
    if (i == exp && mul == 0)
    {
      tmp_array[i] = '.';
      i++;
    }
  }
  for (int j = i - 1; j >= 0; j--)
  {
    array[k] = tmp_array[j];
    k++;
  }
  array[k] = '\0';
  return array;
}
void init_decimal(s21_decimal *number, int exp, int mantiss, int sign)
{
  zero_decimal(number);
  for (int i = 0; i < 3; i++)
  {
    number->bits[i] = mantiss;
  }
  if (exp)
  {
    set_exp(number, exp);
  }
  if (sign)
  {
    set_sign(number);
  }
}
int s21_floor(s21_decimal value, s21_decimal *result)
{
  for (int x = 0; x < 4; x += 1)
    result->bits[x] = 0;
  int sign = !!(value.bits[3] & (1u << 31));
  if (!s21_truncate(value, result) && sign && !s21_is_equal(*result, value))
    s21_sub(*result, ((s21_decimal){{1, 0, 0, 0}}), result);
  return 0;
}
int s21_negate(s21_decimal value, s21_decimal *result)
{
  int conversion_result = 0;

  if (!result)
  {
    conversion_result = 1;
  }
  else
  {
    *result = value;

    dst_bit_3 bits3;
    bits3.mask = value.bits[3];
    int sign = bits3.parts.sign;
    if (sign == 0)
    {
      bits3.parts.sign = 1;
    }
    else
    {
      bits3.parts.sign = 0;
    }
    result->bits[3] = bits3.mask;
    if (bits3.parts.empty_1 != 0 || bits3.parts.empty_2 != 0)
    {
      conversion_result = 1;
    }
    else
    {
      int power = bits3.parts.degree;
      if (power < 0 || power > 28)
      {
        conversion_result = 1;
      }
    }
  }

  return conversion_result;
}
int s21_round(s21_decimal value, s21_decimal *result)
{
  if (!s21_truncate(value, result) && !s21_is_equal(value, *result))
  {
    int sign = !!(value.bits[3] & (1u << 31));
    if (sign)
    {
      s21_sub(value, ((s21_decimal){{5, 0, 0, 65536}}), &value);
    }
    else
    {
      s21_add(value, ((s21_decimal){{5, 0, 0, 65536}}), &value);
      s21_truncate(value, result);
    }
  }
  return 0;
}
int s21_truncate(s21_decimal value, s21_decimal *result)
{
  int fun_result = 0;

  int scale = (char)(value.bits[3] >> 16);
  if (scale)
  {
    for (int y = 0; y < scale; y += 1)
    {
      unsigned long long overflow = value.bits[2];
      for (int x = 2; x >= 0; x -= 1)
      {
        value.bits[x] = overflow / 10;
        overflow =
            (overflow % 10) * (S21_MAX_UINT + 1) + value.bits[x ? x - 1 : x];
      }
    }
    scale = scale - scale;
    if (scale >= 0 && scale <= 28)
    {
      int sign = !!(value.bits[3] & (1u << 31));
      value.bits[3] &= ~(0xFF << 16);
      value.bits[3] |= scale << 16;
      if (sign)
      {
        value.bits[3] = (value.bits[3] | (1u << 31));
      }
    }
    for (int x = 0; x < 4; x += 1)
      result->bits[x] = 0;
    for (int x = 0; x < 4; x += 1)
      result->bits[x] = value.bits[x];
  }
  else
  {
    for (int x = 0; x < 4; x += 1)
      result->bits[x] = 0;
    for (int x = 0; x < 4; x += 1)
      result->bits[x] = value.bits[x];
  }
  return fun_result;
}
s21_decimal *setBit(s21_decimal *value, int pos, int bit)
{
  if (pos / 32 < 4 && bit)
    value->bits[pos / 32] |= (1u << (pos % 32));
  else if (pos / 32 < 4 && !bit)
    value->bits[pos / 32] &= ~(1u << (pos % 32));
  return value;
}
int getSign(s21_decimal value)
{
  int result = !!(value.bits[3] & (1u << 31));
  return result;
}
s21_decimal *setSign(s21_decimal *value, int bit)
{
  value->bits[3] =
      (bit) ? (value->bits[3] | (1u << 31)) : (value->bits[3] & ~(1u << 31));
  return value;
}
s21_decimal *setScale(s21_decimal *value, int scale)
{
  if (scale >= 0 && scale <= 28)
  {
    int sign = getSign(*value);
    value->bits[3] &= ~(0xFF << 16);
    value->bits[3] |= scale << 16;
    sign ? setSign(value, 1) : value;
  }
  return value;
}
