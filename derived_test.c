
#include "print.h"
#include "utility.h"
#include "derived_test.h"

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_DERIVED_SO     "libderived_c.so"
#endif

#include <math.h>

static Interface_expected interface_expected = { {"pv_number", 0}, {"pv_char", '0'} };

static Abstract_expected abstract_expected = { { {"pv_number", 0}, {"pv_char", '0'} },
                {"x", 0}, {"y", 0}, {"virt_area", 0}, {"number", 0} };

static Base_expected base_expected = { { { {"pv_number", 0}, {"pv_char", '0'} },
                {"x", 0}, {"y", 0}, {"virt_area", 0}, {"number", 0} } };

static Derived_expected derived_expected = { { { { {"pv_number", 0}, {"pv_char", '0'} },
                {"x", 0}, {"y", 0}, {"virt_area", 0}, {"number", 0} } }, 
   {"z", 0}
};

static Interface_real interface_real =  { 0, '0'};

static Abstract_real abstract_real = { { 0, '0'}, 0, 0, 0, 0 };

static Base_real base_real = { { { 0, '0'}, 0, 0, 0, 0 } };

static Derived_real derived_real = { { { { 0, '0'}, 0, 0, 0, 0 } }, 0 }; 

static Derived_functions functions;

static Derived_t * derived = NULL;

static Result_codes incorrect_init(const double x, const double y, const double z) { 
   Result_codes result = functions.init(&derived, x, y, z); 
   assert_many(result == INVALID_ARG, "assert failed: ", "s d", "result == ", result);
   assert_many(derived == NULL, "assert failed: ", "s p", "derived pointer == ", derived);
   return (result == INVALID_ARG) ? OK : BAD_FUNTION_CALL;
}

static Result_codes test_constructor(void) {  
   Result_codes result = incorrect_init(4, -3, 7); 
   if (OK == result)
      result = incorrect_init(4, 55, 107); 
   if (OK == result)
      result = incorrect_init(4, 55, 0);
   if (OK == result)
      result = incorrect_init(INFINITY, 101, 7);
   if (result == OK) {
      result = functions.init(&derived, 4, 101, 7);
      assert_many(derived != NULL, "assert failed: ", "s p", "derived pointer == ", derived);
   }
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

static void assert_interface(int n, char c) {  
   Interface_t * inter = (Interface_t *) derived;
   set_interface_real(&interface_real,
            functions.base.abstract.interface.pv_number(inter), functions.base.abstract.interface.pv_char(inter));
   set_interface_expected(&interface_expected, n, c);
   interface_print_and_assert(&interface_expected, &interface_real);
}

static void assert_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   Abstract_t * abs = (Abstract_t *) derived;
   set_abstract_real(&abstract_real,
      functions.base.abstract.interface.pv_number(abs), functions.base.abstract.interface.pv_char(abs),
      functions.base.abstract.X(abs), functions.base.abstract.pv_Y(abs), functions.base.abstract.virt_area(abs),
      functions.base.abstract.Abstract_number(abs));
   set_abstract_expected(&abstract_expected, pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(&abstract_expected, &abstract_real);
}

static void assert_base(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   Base_t * base = (Base_t *) derived;
   set_base_real(&base_real,
      functions.base.abstract.interface.pv_number(base), functions.base.abstract.interface.pv_char(base),
      functions.base.abstract.X(base), functions.base.abstract.pv_Y(base), functions.base.abstract.virt_area(base),
      functions.base.Base_number(base));
   set_base_expected(&base_expected, pv_n, pv_c, x, pv_y, area, n);
   base_print_and_assert(&base_expected, &base_real);
}

static void assert_derived(int pv_n, char pv_c, double x, double pv_y, double z, double area, int n) {
   set_derived_real(&derived_real,
      functions.base.abstract.interface.pv_number(derived), functions.base.abstract.interface.pv_char(derived),
      functions.base.abstract.X(derived), functions.base.abstract.pv_Y(derived), functions.base.abstract.virt_area(derived),
      functions.Derived_number(derived), functions.Z(derived));
   set_derived_expected(&derived_expected, pv_n, pv_c, x, pv_y, area, n, z);
   derived_print_and_assert(&derived_expected, &derived_real);
}

static void load_derived_functions(void) {
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
   functions.base.handle = get_handle(LIB_DERIVED_SO, RTLD_LAZY);
   functions.init = get_symbol(functions.base.handle, "Derived_init");
   functions.Derived_destroy = get_symbol(functions.base.handle, "Derived_destroy");
   functions.base.abstract.interface.Interface_destroy = get_symbol(functions.base.handle, "Interface_destroy");
   
   functions.base.abstract.interface.pv_number = get_symbol(functions.base.handle, "pv_number");
   functions.base.abstract.interface.pv_char = get_symbol(functions.base.handle, "pv_char");
   functions.base.abstract.X = get_symbol(functions.base.handle, "X");
   functions.base.abstract.virt_set_X = get_symbol(functions.base.handle, "virt_set_X");
   functions.base.abstract.pv_Y = get_symbol(functions.base.handle, "pv_Y");
   functions.base.virt_set_Y = get_symbol(functions.base.handle, "virt_set_Y");
   functions.base.abstract.virt_area = get_symbol(functions.base.handle, "virt_area");
   functions.Z = get_symbol(functions.base.handle, "Z");
   functions.virt_set_Z = get_symbol(functions.base.handle, "virt_set_Z");
   
   functions.base.abstract.Abstract_number = get_symbol(functions.base.handle, "Abstract_number");
   functions.base.Base_number = get_symbol(functions.base.handle, "Base_number");
   functions.Derived_number = get_symbol(functions.base.handle, "Derived_number");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   functions.base.handle = NULL;
   functions.init = Derived_init;
   functions.Derived_destroy = Derived_destroy;
   functions.base.abstract.interface.Interface_destroy = Interface_destroy;
   
   functions.base.abstract.interface.pv_number = pv_number;
   functions.base.abstract.interface.pv_char = pv_char;
   functions.base.abstract.X = X;
   functions.base.abstract.virt_set_X = virt_set_X;
   functions.base.abstract.pv_Y = pv_Y;
   functions.base.virt_set_Y = virt_set_Y;
   functions.base.abstract.virt_area = virt_area;
   functions.Z = Z;
   functions.virt_set_Z = virt_set_Z;
   
   functions.base.abstract.Abstract_number = Abstract_number;
   functions.base.Base_number = Base_number;
   functions.Derived_number = Derived_number;
#endif
}

static void print_assert_of_derived() {
   assert_interface(DERIVED, DERIVED_CHAR);
   assert_abstract(DERIVED, DERIVED_CHAR, 4.0, 101.0, 4.0 * 101.0 * 7.0, ABSTRACT);
   assert_base(DERIVED, DERIVED_CHAR, 4.0, 101.0, 4.0 * 101.0 * 7.0, BASE);
   assert_derived(DERIVED, DERIVED_CHAR, 4.0, 101.0, 7.0, 4.0 * 101.0 * 7.0, DERIVED);
}

static Result_codes test_derived_linking(void) {
   Result_codes result = getset_double(functions.base.abstract.virt_set_X, derived, 7.5, functions.base.abstract.X, "Derived x");
   if (OK == result)
      result = incorrect_set_double(functions.base.virt_set_Y, derived, INFINITY, functions.base.abstract.pv_Y, "Derived x");
   if (OK == result) {
      print_and_assert(functions.base.abstract.X(derived), 7.5, "Derived x", "%f", "s f");
      result = getset_double(functions.base.virt_set_Y, derived, 0.8, functions.base.abstract.pv_Y, "Derived y"); 
   }
   if (OK == result)
      result = incorrect_set_double(functions.base.virt_set_Y, derived, -105.8, functions.base.abstract.pv_Y, "Derived y");
   if (OK == result) {
      print_and_assert(functions.base.abstract.pv_Y(derived), 0.8, "Derived y", "%f", "s f");
      result = getset_double(functions.virt_set_Z, derived, -66.8, functions.Z, "Derived z");
   }
   if (OK == result)
      result = incorrect_set_double(functions.virt_set_Z, derived, -0.0, functions.Z, "Derived z");
   if (OK == result)
      result = incorrect_set_double(functions.virt_set_Z, derived, 190.0, functions.Z, "Derived z");
   if (OK == result) {
      print_and_assert(functions.Z(derived), -66.8, "Derived virt_area", "%f", "s f");
      print_and_assert(functions.base.abstract.virt_area(derived), 7.5 * 0.8 * -66.8, "Derived virt_area", "%f", "s f");
   }
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   assert_many(derived != NULL, "assert failed: ", "s p", "derived pointer == ", derived);
   return result;
}

Result_codes test_derived(void) {
   FUNCTION_INFO(__FUNCTION__);
   load_derived_functions(); 
   Result_codes result = test_constructor();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   if (result == OK) {
      print_assert_of_derived();
      result = test_derived_linking(); 
   }
#ifdef MANUAL_DLL_LOAD
   if (result == OK)
      result = close_handle(&(functions.base.handle));
   else
      close_handle(&(functions.base.handle));
#endif
   functions.Derived_destroy(&derived);
   assert_many(derived == NULL, "assert failed: ", "s p", "derived pointer == ", derived);
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}
