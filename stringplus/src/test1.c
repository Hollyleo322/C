/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "test.check" instead.
 */

#include <check.h>

#line 1 "test.check"
#include <string.h>

#include "s21_string.h"

START_TEST(memchr_test) {
#line 5
  char *test_array = "School 21";
  int test_number1 = 67;
  int test_number2 = 100;
  int test_number3 = 50;
  ck_assert_ptr_eq(s21_memchr(test_array, test_number1, 9),
                   memchr(test_array, test_number1, 9));
  ck_assert_ptr_eq(s21_memchr(test_array, test_number2, 9),
                   memchr(test_array, test_number2, 9));
  ck_assert_ptr_eq(s21_memchr(test_array, test_number3, 9),
                   memchr(test_array, test_number3, 9));
}
END_TEST

START_TEST(memcmp_test) {
#line 14
  char *test_array1 = "School 21";
  char *test_array2 = "School 21";
  ck_assert_int_eq(s21_memcmp(test_array1, test_array2, 9),
                   memcmp(test_array1, test_array2, 9));
  ck_assert_int_eq(s21_memcmp(test_array1, test_array2, 5),
                   memcmp(test_array1, test_array2, 5));
}
END_TEST

START_TEST(memcpy_test) {
#line 20
  char array1[100] = {0};
  char array2[100] = {0};
  char *test_array1 = "School 21";
  char *test_array2 = "";
  char *test_array3 = "school";
  ck_assert_mem_eq(s21_memcpy(array1, test_array1, 8),
                   memcpy(array2, test_array1, 8), 8);
  ck_assert_mem_eq(s21_memcpy(array1, test_array2, 8),
                   memcpy(array2, test_array2, 8), 8);
  ck_assert_mem_eq(s21_memcpy(array1, test_array3, 5),
                   memcpy(array2, test_array3, 5), 5);
}
END_TEST

START_TEST(memset_test) {
#line 30
  char array1[100] = {0};
  char array2[100] = {0};
  int test_number1 = 50;
  int test_number2 = 60;
  int test_number3 = 65;
  ck_assert_mem_eq(s21_memset(array1, test_number1, 9),
                   memset(array2, test_number1, 9), 9);
  ck_assert_mem_eq(s21_memset(array1, test_number2, 9),
                   memset(array2, test_number2, 9), 9);
  ck_assert_mem_eq(s21_memset(array1, test_number3, 9),
                   memset(array2, test_number3, 9), 9);
}
END_TEST

START_TEST(strcspn_test) {
#line 40
  char *main_array = "School 21";
  char *test_array1 = "ool";
  char *test_array2 = "Scho";
  char *test_array3 = "1";
  ck_assert_uint_eq(s21_strcspn(main_array, test_array1),
                    strcspn(main_array, test_array1));
  ck_assert_uint_eq(s21_strcspn(main_array, test_array2),
                    strcspn(main_array, test_array2));
  ck_assert_uint_eq(s21_strcspn(main_array, test_array3),
                    strcspn(main_array, test_array3));
}
END_TEST

START_TEST(strerror_test) {
#line 49
  int test_number1 = 20;
  int test_number2 = 0;
  int test_number3 = 100;
  int test_number4 = 200;
  ck_assert_str_eq(s21_strerror(test_number1), strerror(test_number1));
  ck_assert_str_eq(s21_strerror(test_number2), strerror(test_number2));
  ck_assert_str_eq(s21_strerror(test_number3), strerror(test_number3));
  ck_assert_str_eq(s21_strerror(test_number4), strerror(test_number4));
}
END_TEST

START_TEST(strpbrk_test) {
#line 59
  char *main_array = "School 21";
  char *test_array1 = "sc";
  char *test_array2 = "21";
  char *test_array3 = " ";
  ck_assert_ptr_eq(s21_strpbrk(main_array, test_array1),
                   strpbrk(main_array, test_array1));
  ck_assert_ptr_eq(s21_strpbrk(main_array, test_array2),
                   strpbrk(main_array, test_array2));
  ck_assert_ptr_eq(s21_strpbrk(main_array, test_array3),
                   strpbrk(main_array, test_array3));
}
END_TEST

START_TEST(strlen_test) {
#line 68
  char *test_array1 = "School 21";
  char *test_array2 = "stanislave";
  char *test_array3 = "";
  ck_assert_uint_eq(s21_strlen(test_array1), strlen(test_array1));
  ck_assert_uint_eq(s21_strlen(test_array2), strlen(test_array2));
  ck_assert_uint_eq(s21_strlen(test_array3), strlen(test_array3));
}
END_TEST

START_TEST(strcmp_test) {
#line 76
  char *test_array1 = "School 21";
  char *test_array2 = "School 21";
  char *test_array3 = "school 21";
  ck_assert_int_eq(s21_strcmp(test_array1, test_array2),
                   strcmp(test_array1, test_array2));
  ck_assert_int_eq(s21_strcmp(test_array1, test_array3),
                   strcmp(test_array1, test_array3));
  ck_assert_int_eq(s21_strcmp(test_array1, test_array2),
                   strcmp(test_array1, test_array2));
}
END_TEST

START_TEST(strcpy_test) {
#line 84
  char array1[10] = {0};
  char array2[10] = {0};
  char *test_array1 = "School 21";
  char *test_array2 = "";
  char *test_array3 = "school";
  ck_assert_str_eq(s21_strcpy(array1, test_array1),
                   strcpy(array2, test_array1));
  ck_assert_str_eq(s21_strcpy(array1, test_array2),
                   strcpy(array2, test_array2));
  ck_assert_str_eq(s21_strcpy(array1, test_array3),
                   strcpy(array2, test_array3));
}
END_TEST

START_TEST(strchr_test) {
#line 94
  char *test_array = "School 21";
  int test_number1 = 52;
  int test_number2 = 59;
  int test_number3 = 73;
  ck_assert_ptr_eq(s21_strchr(test_array, test_number1),
                   strchr(test_array, test_number1));
  ck_assert_ptr_eq(s21_strchr(test_array, test_number2),
                   strchr(test_array, test_number2));
  ck_assert_ptr_eq(s21_strchr(test_array, test_number3),
                   strchr(test_array, test_number3));
}
END_TEST

START_TEST(strcat_test) {
#line 103
  char array1[20] = "School";
  char *test_array1 = " 21";
  char *test_array2 = "321";
  char *test_array3 = " 32";
  ck_assert_str_eq(s21_strcat(array1, test_array1),
                   strcat(array1, test_array1));
  ck_assert_str_eq(s21_strcat(array1, test_array2),
                   strcat(array1, test_array2));
  ck_assert_str_eq(s21_strcat(array1, test_array3),
                   strcat(array1, test_array3));
}
END_TEST

START_TEST(strstr_test) {
#line 112
  char *array = "School 21";
  char *test_array1 = "cho";
  char *test_array2 = " 2";
  char *test_array3 = "xexe";
  ck_assert_ptr_eq(s21_strstr(array, test_array1), strstr(array, test_array1));
  ck_assert_ptr_eq(s21_strstr(array, test_array2), strstr(array, test_array2));
  ck_assert_ptr_eq(s21_strstr(array, test_array3), strstr(array, test_array3));
  ck_assert_str_eq(s21_strstr(array, test_array1), strstr(array, test_array1));
  ck_assert_str_eq(s21_strstr(array, test_array2), strstr(array, test_array2));
  ck_assert_pstr_eq(s21_strstr(array, test_array3), strstr(array, test_array3));
}
END_TEST

START_TEST(strtok_test) {
#line 124
  char test_array1[30] = "Sch/ool/21/heh";
  char test_array1_2[30] = "Sch/ool/21/heh";
  char *delim1 = "/ ";
  char test_array2[30] = "fantazer,ty,menya,nazyvala";
  char test_array2_2[30] = "fantazer,ty,menya,nazyvala";
  char *delim2 = ",.!";
  char test_array3[30] = "fish.of.my.dream";
  char test_array3_2[30] = "fish.of.my.dream";
  char *delim3 = ",.!?";
  char *ptr1 = NULL;
  char *ptr2 = NULL;
  ck_assert_ptr_eq(ptr1 = s21_strtok(test_array1, delim1),
                   ptr2 = strtok(test_array1, delim1));
  ptr2 = strtok(test_array1_2, delim1);
  while (ptr1 && ptr2) {
    ck_assert_str_eq(ptr1, ptr2);
    ptr1 = s21_strtok(NULL, delim1);
    ptr2 = strtok(NULL, delim1);
  }
  ck_assert_ptr_eq(ptr1 = s21_strtok(test_array2, delim2),
                   ptr2 = strtok(test_array2, delim2));
  ptr2 = strtok(test_array2_2, delim2);
  while (ptr1 && ptr2) {
    ck_assert_str_eq(ptr1, ptr2);
    ptr1 = s21_strtok(NULL, delim2);
    ptr2 = strtok(NULL, delim2);
  }
  ck_assert_ptr_eq(ptr1 = s21_strtok(test_array3, delim3),
                   ptr2 = strtok(test_array3, delim3));
  ptr2 = strtok(test_array3_2, delim3);
  while (ptr1 && ptr2) {
    ck_assert_str_eq(ptr1, ptr2);
    ptr1 = s21_strtok(NULL, delim3);
    ptr2 = strtok(NULL, delim3);
  }
}
END_TEST

START_TEST(strrchr_test) {
#line 157
  char *test_array = "School 21 21";
  int test_number1 = 52;
  int test_number2 = 59;
  int test_number3 = 73;
  ck_assert_ptr_eq(s21_strrchr(test_array, test_number1),
                   strrchr(test_array, test_number1));
  ck_assert_ptr_eq(s21_strrchr(test_array, test_number2),
                   strrchr(test_array, test_number2));
  ck_assert_ptr_eq(s21_strrchr(test_array, test_number3),
                   strrchr(test_array, test_number3));
}
END_TEST

START_TEST(strncat_test) {
#line 166
  char array1[20] = "School";
  char *test_array1 = " 21";
  char *test_array2 = "321";
  char *test_array3 = " 32";
  ck_assert_str_eq(s21_strncat(array1, test_array1, 3),
                   strncat(array1, test_array1, 3));
  ck_assert_str_eq(s21_strncat(array1, test_array2, 2),
                   strncat(array1, test_array2, 2));
  ck_assert_str_eq(s21_strncat(array1, test_array3, 5),
                   strncat(array1, test_array3, 5));
}
END_TEST

START_TEST(strncmp_test) {
#line 175
  char *test_array1 = "School 21";
  char *test_array2 = "School 21";
  char *test_array3 = "school 21";
  ck_assert_int_eq(s21_strncmp(test_array1, test_array2, 20),
                   strncmp(test_array1, test_array2, 20));
  ck_assert_int_eq(s21_strncmp(test_array1, test_array3, 3),
                   strncmp(test_array1, test_array3, 3));
  ck_assert_int_eq(s21_strncmp(test_array1, test_array2, 4),
                   strncmp(test_array1, test_array2, 4));
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, memchr_test);
  tcase_add_test(tc1_1, memcmp_test);
  tcase_add_test(tc1_1, memcpy_test);
  tcase_add_test(tc1_1, memset_test);
  tcase_add_test(tc1_1, strcspn_test);
  tcase_add_test(tc1_1, strerror_test);
  tcase_add_test(tc1_1, strpbrk_test);
  tcase_add_test(tc1_1, strlen_test);
  tcase_add_test(tc1_1, strcmp_test);
  tcase_add_test(tc1_1, strcpy_test);
  tcase_add_test(tc1_1, strchr_test);
  tcase_add_test(tc1_1, strcat_test);
  tcase_add_test(tc1_1, strstr_test);
  tcase_add_test(tc1_1, strtok_test);
  tcase_add_test(tc1_1, strrchr_test);
  tcase_add_test(tc1_1, strncat_test);
  tcase_add_test(tc1_1, strncmp_test);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
