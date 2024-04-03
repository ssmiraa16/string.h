#include "../s21_sscanf.h"

#include "s21_tests_main.h"

START_TEST(sscanf_test_e_1) {
  float e1 = 0, e2 = 0;
  int orig_result = 0, our_result = 0;

  orig_result = sscanf("42e21", "%e", &e1);
  our_result = s21_sscanf("42e21", "%e", &e2);
  ck_assert_msg(e1 == e2, "not equal");
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("42.215e-21", "%9e", &e1);
  our_result = s21_sscanf("42.215e-21", "%9e", &e2);
  ck_assert_int_eq(orig_result, our_result);
  ck_assert_float_eq(e1, e2);
  ck_assert_msg(e1 == e2, "not equal");

  orig_result = sscanf("1.423422423", "%e", &e1);
  our_result = s21_sscanf("1.423422423", "%e", &e2);
  ck_assert_float_eq(e1, e2);
  ck_assert_msg(e1 == e2, "not equal");
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_l_1) {
  int orig_result = 0, our_result = 0;
  unsigned long lu1 = 0, lu2 = 0;
  long ld1 = 0, ld2 = 0;
  wchar_t lc1, lc2;
  wchar_t lsuite[100], ls2[100];

  orig_result = sscanf("a", "%lc", &lc1);
  our_result = s21_sscanf("a", "%lc", &lc2);
  ck_assert_int_eq(lc1, lc2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("4245", "%lu", &lu1);
  our_result = s21_sscanf("4245", "%lu", &lu2);
  ck_assert_int_eq(lu1, lu2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("4245", "%ld", &ld1);
  our_result = s21_sscanf("4245", "%ld", &ld2);
  ck_assert_int_eq(ld1, ld2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("amazing", "%9ls", lsuite);
  our_result = s21_sscanf("amazing", "%9ls", ls2);
  ck_assert_int_eq(orig_result, our_result);
  ck_assert_int_eq(wcscmp(ls2, lsuite), 0);
}
END_TEST

START_TEST(sscanf_test_L_1) {
  int orig_result = 0, our_result = 0;
  long double Lf1 = 0, Lf2 = 0;

  orig_result = sscanf("42.45", "%Lf", &Lf1);
  our_result = s21_sscanf("42.45", "%Lf", &Lf2);
  ck_assert_msg((int)Lf1 == (int)Lf2, "not equal");
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_u_1) {
  int orig_result = 0, our_result = 0;
  unsigned u1 = 0, u2 = 0;

  orig_result = sscanf("", "%u", &u1);
  our_result = s21_sscanf("", "%u", &u2);
  ck_assert_int_eq(u1, u2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("481516", "%u", &u1);
  our_result = s21_sscanf("481516", "%u", &u2);
  ck_assert_int_eq(u1, u2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("-48", "%u", &u1);
  our_result = s21_sscanf("-48", "%u", &u2);
  ck_assert_int_eq(u1, u2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_h_1) {
  int orig_result = 0, our_result = 0;
  unsigned short hu1 = 0, hu2 = 0;

  orig_result = sscanf("1", "%hu", &hu1);
  our_result = s21_sscanf("1", "%hu", &hu2);
  ck_assert_int_eq(hu1, hu2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("48151623", "%hu", &hu1);
  our_result = s21_sscanf("48151623", "%hu", &hu2);
  ck_assert_int_eq(hu1, hu2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("-5", "%hu", &hu1);
  our_result = s21_sscanf("-5", "%hu", &hu2);
  ck_assert_int_eq(hu1, hu2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_s_1) {
  int orig_result = 0, our_result = 0;
  char suite[100], s2[100];

  orig_result = sscanf("Hello world", "%*s%s", suite);
  our_result = s21_sscanf("Hello world", "%*s%s", s2);
  ck_assert_str_eq(suite, s2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("", "%s", suite);
  our_result = s21_sscanf("", "%s", s2);
  ck_assert_str_eq(suite, s2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("Hello", "%18s", suite);
  our_result = s21_sscanf("Hello", "%18s", s2);
  ck_assert_str_eq(suite, s2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_f_1) {
  int orig_result = 0, our_result = 0;
  float f_1 = 0, f2 = 0;

  orig_result = sscanf("42e-3", "%f", &f_1);
  our_result = s21_sscanf("42e-3", "%f", &f2);
  ck_assert_int_eq(f_1, f2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("-42.21", "%f", &f_1);
  our_result = s21_sscanf("-42.21", "%f", &f2);
  ck_assert_int_eq(f_1, f2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("21.42", "%5f", &f_1);
  our_result = s21_sscanf("21.42", "%5f", &f2);
  ck_assert_int_eq(f_1, f2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_o_1) {
  int orig_result = 0, our_result = 0;
  unsigned int o1 = 0, o2 = 0;

  orig_result = sscanf("177751", "%o", &o1);
  our_result = s21_sscanf("177751", "%o", &o2);
  ck_assert_int_eq(o1, o2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("123456789", "%5o", &o1);
  our_result = s21_sscanf("123456789", "%5o", &o2);
  ck_assert_int_eq(o1, o2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("-1", "%o", &o1);
  our_result = s21_sscanf("-1", "%o", &o2);
  ck_assert_int_eq(o1, o2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_x_1) {
  int orig_result = 0, our_result = 0;
  unsigned x1 = 0, x2 = 0;

  orig_result = sscanf("0X481516", "%x", &x1);
  our_result = s21_sscanf("0X481516", "%x", &x2);
  ck_assert_uint_eq(x1, x2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("0x481516", "%2x", &x1);
  our_result = s21_sscanf("0x481516", "%2x", &x2);
  ck_assert_uint_eq(x1, x2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("fffff", "%2x", &x1);
  our_result = s21_sscanf("fffff", "%2x", &x2);
  ck_assert_uint_eq(x1, x2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("-42", "%x", &x1);
  our_result = s21_sscanf("-42", "%x", &x2);
  ck_assert_uint_eq(x1, x2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_i_1) {
  int orig_result = 0, our_result = 0;
  int i1 = 0, i2 = 0;

  orig_result = sscanf("9878645", "%i", &i1);
  our_result = s21_sscanf("9878645", "%i", &i2);
  ck_assert_int_eq(i1, i2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("0x42AB", "%i", &i1);
  our_result = s21_sscanf("0x42AB", "%i", &i2);
  ck_assert_int_eq(i1, i2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("-481516", "%i", &i1);
  our_result = s21_sscanf("-481516", "%i", &i2);
  ck_assert_int_eq(i1, i2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_proc_1) {
  int orig_result = 0, our_result = 0;
  char proc1[100] = {0}, proc2[100] = {0};

  orig_result = sscanf("48151642342", "%s%%", proc1);
  our_result = s21_sscanf("48151642342", "%s%%", proc2);
  ck_assert_str_eq(proc1, proc2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_p_1) {
  int orig_result = 0, our_result = 0;
  void *p1 = 0, *p2 = 0;

  orig_result = sscanf("0x42", "%8p", &p1);
  our_result = s21_sscanf("0x42", "%8p", &p2);
  ck_assert_ptr_eq(p1, p2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("0X4245", "%p", &p1);
  our_result = s21_sscanf("0X4245", "%p", &p2);
  ck_assert_ptr_eq(p1, p2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("0", "%p", &p1);
  our_result = s21_sscanf("0", "%p", &p2);
  ck_assert_ptr_eq(p1, p2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_n_1) {
  int n_1 = 0, n_2 = 0, d_1 = 0, d_2 = 0, orig_result = 0, our_result = 0;

  orig_result = sscanf("42.456", "%n", &n_1);
  our_result = s21_sscanf("42.456", "%n", &n_2);
  ck_assert_int_eq(n_1, n_2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("42456", "%d%n", &d_1, &n_1);
  our_result = s21_sscanf("42456", "%d%n", &d_2, &n_2);
  ck_assert_int_eq(n_1, n_2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_d_1) {
  int orig_result = 0, our_result = 0, d1 = 0, d2 = 0;

  orig_result = sscanf("42", "%d", &d1);
  our_result = s21_sscanf("42", "%d", &d2);
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("-42 l", "%10d", &d1);
  our_result = s21_sscanf("-42 l", "%10d", &d2);
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_all) {
  int d1 = 0, d2 = 0, i1 = 0, i2 = 0, orig_result = 0, our_result = 0;
  float e1 = 0, e2 = 0, E1 = 0, E2 = 0;
  char suite[100], s2[100], c1, c2;

  orig_result = sscanf("42", "%d", &d1);
  our_result = s21_sscanf("42", "%d", &d2);
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("42 amazing", "%d%9s", &d1, suite);
  our_result = s21_sscanf("42 amazing", "%d%9s", &d2, s2);
  ck_assert_int_eq(d1, d2);
  ck_assert_str_eq(suite, s2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result =
      sscanf("4 42 amazing q 12e21", "%i%d%9s%c%e", &i1, &d1, suite, &c1, &e1);
  our_result =
      s21_sscanf("4 42 amazing q 12e21", "%i%d%9s%c%e", &i2, &d2, s2, &c2, &e2);
  ck_assert_int_eq(i1, i2);
  ck_assert_int_eq(d1, d2);
  ck_assert_str_eq(suite, s2);
  ck_assert_int_eq(c1, c2);
  ck_assert_msg(e1 == e2, "floats not equals");
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf("4 42 amazing q 12 43e21", "%i%d%9s%c%e%E", &i1, &d1,
                       suite, &c1, &e1, &E1);
  our_result = s21_sscanf("4 42 amazing q 12 43e21", "%i%d%9s%c%e%E", &i2, &d2,
                          s2, &c2, &e2, &E2);
  ck_assert_int_eq(i1, i2);
  ck_assert_int_eq(d1, d2);
  ck_assert_str_eq(suite, s2);
  ck_assert_int_eq(c1, c2);
  ck_assert_msg(e1 == e2, "floats not equals");
  ck_assert_msg(E1 == E2, "floats not equals");
  ck_assert_int_eq(orig_result, our_result);
}

START_TEST(sscanf_test_c_1) {
  char c1, c2;
  int orig_result, our_result;
  char *storig_result = " abc ";

  orig_result = sscanf(storig_result, "%*c%*c%c", &c1);
  our_result = s21_sscanf(storig_result, "%*c%*c%c", &c2);
  ck_assert_int_eq(orig_result, our_result);
  ck_assert_int_eq(c1, c2);

  char fstr[] = "%c %c %c %c";
  char str[] = " c    a  d b";
  int a1 = 0, a2 = 5, b1 = 0, b2 = 5, z1 = 0, z2 = 5, d1 = 0, d2 = 5;

  int res1 = s21_sscanf(str, fstr, &a1, &b1, &z1, &d1);

  int res2 = sscanf(str, fstr, &a2, &b2, &z2, &d2);

  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
  ck_assert_int_eq(z1, z2);
  ck_assert_int_eq(d1, d2);
}
END_TEST

START_TEST(sscanf_test_u_2) {
  unsigned u1 = 1, u2 = 2, orig_result, our_result;
  char *str4 = " 0    3513187186";
  char *wrong_str4 = "  School21";

  orig_result = sscanf(str4, "%*u%u", &u1);
  our_result = s21_sscanf(str4, "%*u%u", &u2);
  ck_assert_uint_eq(u1, u2);
  ck_assert_int_eq(orig_result, our_result);

  orig_result = sscanf(wrong_str4, "%u", &u1);
  our_result = s21_sscanf(wrong_str4, "%u", &u2);
  ck_assert_uint_eq(u1, u2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_double) {
  int orig_result, our_result;
  double fl1 = 1, fl2 = 2;
  double dbl1 = 1, dbl2 = 2;
  char *str5 = "  -876.214235  2145.93560721567  324.21427 ";

  orig_result = sscanf(str5, "%lf%lf%*f", &fl1, &dbl1);
  our_result = s21_sscanf(str5, "%lf%lf%*f", &fl2, &dbl2);
  ck_assert_msg(dbl1 != dbl2, "Float test 1 fail");
  ck_assert_msg(fl1 != fl2, "Float test 2 fail");
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

START_TEST(sscanf_test_EeGg) {
  int orig_result, our_result;
  double g1, g2;
  char *wrong_str6 = "  School21+02";

  orig_result = sscanf(wrong_str6, "%lg", &g1);
  our_result = s21_sscanf(wrong_str6, "%lg", &g2);
  ck_assert_int_eq(g1, g2);
  ck_assert_int_eq(orig_result, our_result);
}
END_TEST

Suite *s21_sscanf_test(void) {
  Suite *s;
  TCase *tc_s21_sscanf;
  s = suite_create("\033[34m \033[46m SSCANF TESTS \033[0m");

  tc_s21_sscanf = tcase_create("sscanf_tc");

  tcase_add_test(tc_s21_sscanf, sscanf_test_e_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_l_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_L_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_u_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_h_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_s_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_f_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_o_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_x_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_i_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_proc_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_p_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_n_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_d_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_all);
  tcase_add_test(tc_s21_sscanf, sscanf_test_c_1);
  tcase_add_test(tc_s21_sscanf, sscanf_test_u_2);
  tcase_add_test(tc_s21_sscanf, sscanf_test_double);
  tcase_add_test(tc_s21_sscanf, sscanf_test_EeGg);
  suite_add_tcase(s, tc_s21_sscanf);
  return s;
}
