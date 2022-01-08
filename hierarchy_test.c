
#include "print.h"
#include "utility.h"
#include "hierarchy_test.h"

Result_codes check_double(Result_codes (*get)(double * const), const char * message,
                                       const double expected) {
   REQUIRE_NON_NULL(get, "get is null");
   REQUIRE_NON_NULL(message, "message is null");
   double n;
   Result_codes result = get(&n);
   if (OK == result) {
      LOG("%s: %s %s = %f\n", LANGUAGE, __FUNCTION__, message, n);
      assert_many(n == expected, "assert failed: ", "s f", " n == ", n);
   }
   return result;
}

Result_codes getset_coordination(Result_codes (*set)(const double), const double set_value,
                                Result_codes (*get)(double * const), const char * message) {
   REQUIRE_NON_NULL(set, "set is null");
   Result_codes result = set(set_value);
   if (OK == result)
      result = check_double(get, message, set_value);
   return result;
}

Result_codes incorrect_set_coordination(Result_codes (*set)(const double), const double set_value,
                                Result_codes (*get)(double * const), const char * message) {
   REQUIRE_NON_NULL(set, "set is null");
   Result_codes result = set(set_value);
   if (INVALID_ARG == result)
      result = OK;
   else {
      if (OK == result)
         check_double(get, message, set_value);
      assert_many(result != INVALID_ARG, "assert failed: ", "s d", "result == ", result);
   }
   return result;
}

Result_codes check_int(Result_codes (*get)(int * const), const char * message, const int expected) {
   REQUIRE_NON_NULL(get, "get is null");
   REQUIRE_NON_NULL(message, "message is null");
   int n;
   Result_codes result = get(&n);
   if (OK == result) {
      LOG("%s: %s %s = %d\n", LANGUAGE, __FUNCTION__, message, n);
      assert_many(n == expected, "assert failed: ", "s d", " n == ", n);
   }
   return result;
}

Result_codes check_char(Result_codes (*get)(char * const), const char * message, const char expected) {
   REQUIRE_NON_NULL(get, "get is null");
   REQUIRE_NON_NULL(message, "message is null");
   char n;
   Result_codes result = get(&n);
   if (OK == result) {
      LOG("%s: %s %s = %c\n", LANGUAGE, __FUNCTION__, message, n);
      assert_many(n == expected, "assert failed: ", "s c", " n == ", n);
   }
   return result;
}

void assert_double(double (*get)(const void * const), const void * const object, const char * message,
                                       const double expected) {
   REQUIRE_NON_NULL(object, "object is null");
   REQUIRE_NON_NULL(get, "get is null");
   REQUIRE_NON_NULL(message, "message is null");
   double n = get(object);
   LOG("%s: %s %s = %f\n", LANGUAGE, __FUNCTION__, message, n);
   assert_many(n == expected, "assert failed: ", "s f", " n == ", n);
}

Result_codes getset_double(Result_codes (*set)(void * const object, const double), void * const object, 
                           const double set_value, double (*get)(const void * const), const char * message) {
   REQUIRE_NON_NULL(set, "set is null");
   Result_codes result = set(object, set_value);
   if (OK == result)
      assert_double(get, object, message, set_value);
   return result;
}

Result_codes incorrect_set_double(Result_codes (*set)(void * const object, const double), void * const object, 
                                  const double set_value,
                                Result_codes (*get)(double * const), const char * message) {
   REQUIRE_NON_NULL(set, "set is null");
   Result_codes result = set(object, set_value);
   if (INVALID_ARG == result)
      result = OK;
   else {
      if (OK == result)
         assert_double(get, object, message, set_value);
      assert_many(result != INVALID_ARG, "assert failed: ", "s d", "result == ", result);
   }
   return result;
}
/*
Result_codes check_int(Result_codes (*get)(int * const), const char * message, const int expected) {
   REQUIRE_NON_NULL(get, "get is null");
   REQUIRE_NON_NULL(message, "message is null");
   int n;
   Result_codes result = get(&n);
   if (OK == result) {
      LOG("%s: %s %s = %d\n", LANGUAGE, __FUNCTION__, message, n);
      assert_many(n == expected, "assert failed: ", "s d", " n == ", n);
   }
   return result;
}

Result_codes check_char(Result_codes (*get)(char * const), const char * message, const char expected) {
   REQUIRE_NON_NULL(get, "get is null");
   REQUIRE_NON_NULL(message, "message is null");
   char n;
   Result_codes result = get(&n);
   if (OK == result) {
      LOG("%s: %s %s = %c\n", LANGUAGE, __FUNCTION__, message, n);
      assert_many(n == expected, "assert failed: ", "s c", " n == ", n);
   }
   return result;
}
*/
// void test_base(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
// #ifdef MANUAL_DLL_LOAD
//    base_real.set(base->pv_number(), base->pv_char(), base->X(), base->pv_Y(), base->virt_area(), base->number());
// #else
//    base_real.set(base.pv_number(), base.pv_char(), base.X(), base.pv_Y(), base.virt_area(), base.number());
// #endif
//    base_expected.set(pv_n, pv_c, x, pv_y, area, n);
//    abstract_print_and_assert(base_expected, base_real);
// }

/*   
   EXPECTED PART BELOW
*/

void set_interface_expected(Interface_expected * interface_expected, const int n, const char c) {
   REQUIRE_NON_NULL(interface_expected, "interface_expected is null");
   interface_expected->pv_number.number = n;
   interface_expected->pv_char.ch = c;
}
/*   */
void set_abstract_expected(Abstract_expected * abstract_expected, int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   REQUIRE_NON_NULL(abstract_expected, "abstract_expected is null");
   set_interface_expected(&(abstract_expected->interface), pv_n, pv_c);
   abstract_expected->X.number = x;
   abstract_expected->pv_Y.number = pv_y;
   abstract_expected->virt_area.number = area;
   abstract_expected->number.number = n;
}
/*   */
void set_base_expected(Base_expected * base_expected, int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   REQUIRE_NON_NULL(base_expected, "base_expected is null");
   set_abstract_expected(&(base_expected->abstract), pv_n, pv_c, x, pv_y, area, n);
}
/*   */

void set_derived_expected(Derived_expected * derived_expected, int pv_n, char pv_c, double x, double pv_y, double area, int n, double z) {
   REQUIRE_NON_NULL(derived_expected, "derived_expected is null");
   set_base_expected((Base_expected *)derived_expected, pv_n, pv_c, x, pv_y, area, n);
   derived_expected->Z.number = z;
}

/*   
   REAL PART BELOW
*/

void set_interface_real(Interface_real * interface_real, const int n, const char c) {
   REQUIRE_NON_NULL(interface_real, "interface_real is null");
   interface_real->pv_number = n;
   interface_real->pv_char = c;
}
/*   */

void set_abstract_real(Abstract_real * abstract_real, int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   REQUIRE_NON_NULL(abstract_real, "abstract_real is null");
   set_interface_real(&(abstract_real->interface), pv_n, pv_c);
   abstract_real->X = x;
   abstract_real->pv_Y = pv_y;
   abstract_real->virt_area = area;
   abstract_real->number = n;
}
/*   */

void set_base_real(Base_real * base_real, int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   REQUIRE_NON_NULL(base_real, "base_real is null");
   set_abstract_real(&(base_real->abstract), pv_n, pv_c, x, pv_y, area, n);
}
/*   */
void set_derived_real(Derived_real * derived_real, int pv_n, char pv_c, double x, double pv_y, double area, int n, double z) {
   REQUIRE_NON_NULL(derived_real, "derived_real is null");
   set_base_real((Base_real *)derived_real, pv_n, pv_c, x, pv_y, area, n);
   derived_real->Z = z;
}




void interface_print_and_assert(const Interface_expected * const expected, const Interface_real * const real) {
   REQUIRE_NON_NULL(expected, "expected is null");
   REQUIRE_NON_NULL(real, "real is null");
   print_and_assert(real->pv_number, expected->pv_number.number, expected->pv_number.string, "%d", "s d");
   print_and_assert(real->pv_char, expected->pv_char.ch, expected->pv_char.string, "%c", "s c");
}

void abstract_print_and_assert(const Abstract_expected * const expected, const Abstract_real * const real) {
   interface_print_and_assert(expected, real);
   print_and_assert(real->X, expected->X.number, expected->X.string, "%f", "s f");
   print_and_assert(real->pv_Y, expected->pv_Y.number, expected->pv_Y.string, "%f", "s f");
   print_and_assert(real->virt_area, expected->virt_area.number, expected->virt_area.string, "%f", "s f");
   print_and_assert(real->number, expected->number.number, expected->number.string, "%d", "s d");
}

void base_print_and_assert(const Base_expected * const expected, const Base_real * const real) {
   abstract_print_and_assert((Abstract_expected *)expected, (Abstract_real *)real);
}

void derived_print_and_assert(const Derived_expected * const expected, const Derived_real * const real) {
   base_print_and_assert((Base_expected *)expected, (Base_real *)real);
   print_and_assert(real->Z, expected->Z.number, expected->Z.string, "%f", "s f");
}
