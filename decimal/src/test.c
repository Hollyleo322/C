#include "s21_decimal.h"

int main(void)
{
    float a = 6666666666666666666666666.3;
    s21_decimal b;
    zero_decimal(&b);
    s21_from_float_to_decimal(a, &b);
    printf("%s\n", array_of_decimal(b, 0));
    printf("%d\n", get_exp(b));
}