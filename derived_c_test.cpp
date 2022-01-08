#include "derived_c_test.hpp"
#include "print.hpp"
#include "utility.hpp"

#include <functional>

using std::function;
using std::numeric_limits;

#include "derived.h"

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_DERIVED_SO     "libderived_c.so"
#endif

namespace tests {
   
Derived_t * Derived_C_test::derived = nullptr;
Derived_functions Derived_C_test::functions;
   
void Derived_C_test::load_derived_functions() {
#ifdef MANUAL_DLL_LOAD
   cerr << "\nMANUAL DLL LOAD\n";
   functions.base.handle = get_handle(LIB_DERIVED_SO, RTLD_LAZY);
   functions.init = reinterpret_cast<Result_codes (*)(Derived_t ** const, const double, const double, const double)>
                           (get_symbol(functions.base.handle, "Derived_init"));
   functions.Derived_destroy = reinterpret_cast<void (*)(Derived_t ** const)>
                           (get_symbol(functions.base.handle, "Derived_destroy"));
   functions.base.abstract.interface.Interface_destroy = reinterpret_cast<void (*)(Interface_t ** const)>
                           (get_symbol(functions.base.handle, "Interface_destroy"));
   functions.base.abstract.interface.pv_number = reinterpret_cast<int (*)(const Interface_t * const)>
                           (get_symbol(functions.base.handle, "pv_number"));
   functions.base.abstract.interface.pv_char = reinterpret_cast<char (*)(const Interface_t * const)>
                           (get_symbol(functions.base.handle, "pv_char"));
   functions.base.abstract.X = reinterpret_cast<double (*)(const Abstract_t * const)>
                           (get_symbol(functions.base.handle, "X"));
   functions.base.abstract.virt_set_X = reinterpret_cast<Result_codes (*)(Abstract_t * const, const double)>
                           (get_symbol(functions.base.handle, "virt_set_X"));
   functions.base.abstract.pv_Y = reinterpret_cast<double (*)(const Abstract_t * const)>
                           (get_symbol(functions.base.handle, "pv_Y"));
   functions.base.virt_set_Y = reinterpret_cast<Result_codes (*)(Base_t * const, const double)>
                           (get_symbol(functions.base.handle, "virt_set_Y"));
   functions.Z               = reinterpret_cast<double (*)(const Derived_t * const)>
                           (get_symbol(functions.base.handle, "Z"));
   functions.virt_set_Z      = reinterpret_cast<Result_codes (*)(Derived_t * const, const double)>
                           (get_symbol(functions.base.handle, "virt_set_Z"));
   functions.base.abstract.virt_area = reinterpret_cast<double (*)(const Abstract_t * const)>
                           (get_symbol(functions.base.handle, "virt_area"));
   
   functions.base.abstract.Abstract_number = reinterpret_cast<int (*)(const Abstract_t * const)>
                           (get_symbol(functions.base.handle, "Abstract_number"));
   functions.base.Base_number = reinterpret_cast<int (*)(const Base_t * const)>
                           (get_symbol(functions.base.handle, "Base_number"));
   functions.Derived_number = reinterpret_cast<int (*)(const Derived_t * const)>
                           (get_symbol(functions.base.handle, "Derived_number"));
#else
   cerr << "\nAUTOMATIC DLL LOAD\n";
   functions.base.handle = nullptr;
   functions.init = Derived_init;
   functions.Derived_destroy = Derived_destroy;
   functions.base.abstract.interface.Interface_destroy = Interface_destroy;
   
   functions.base.abstract.interface.pv_number = pv_number;
   functions.base.abstract.interface.pv_char = pv_char;
   functions.base.abstract.X = X;
   functions.base.abstract.virt_set_X = virt_set_X;
   functions.base.abstract.pv_Y = pv_Y;
   functions.base.virt_set_Y = virt_set_Y;
   functions.Z = Z;
   functions.virt_set_Z = virt_set_Z;
   functions.base.abstract.virt_area = virt_area;
   
   functions.base.abstract.Abstract_number = Abstract_number;
   functions.base.Base_number = Base_number;
   functions.Derived_number = Derived_number;
#endif
}

Interface_expected Derived_C_test::interface_expected;
Abstract_expected Derived_C_test::abstract_expected;
Base_expected Derived_C_test::base_expected;
Derived_expected Derived_C_test::derived_expected;

Interface_real Derived_C_test::interface_real;
Abstract_real Derived_C_test::abstract_real;
Base_real Derived_C_test::base_real;
Derived_real Derived_C_test::derived_real;

void Derived_C_test::test_interface(int n, char c) {
   Interface_t * inter = reinterpret_cast<Interface_t * > (derived);
   interface_real.set(functions.base.abstract.interface.pv_number(inter), functions.base.abstract.interface.pv_char(inter));

   interface_expected.set(n, c);
   interface_print_and_assert(interface_expected, interface_real);
}

void Derived_C_test::test_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   Abstract_t * abs = reinterpret_cast<Abstract_t * > (derived);
   abstract_real.set(functions.base.abstract.interface.pv_number (reinterpret_cast<Interface_t * > ((abs))), 
                     functions.base.abstract.interface.pv_char (reinterpret_cast<Interface_t * > ((abs))),
      functions.base.abstract.X(abs), functions.base.abstract.pv_Y(abs), functions.base.abstract.virt_area(abs),
      functions.base.abstract.Abstract_number(abs));
   
   abstract_expected.set(pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(abstract_expected, abstract_real);
}

void Derived_C_test::test_base(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   Base_t * base = reinterpret_cast<Base_t * > (derived);
   base_real.set(functions.base.abstract.interface.pv_number (reinterpret_cast<Interface_t * > (base)),
                 functions.base.abstract.interface.pv_char(reinterpret_cast<Interface_t * > (base)),
      functions.base.abstract.X(reinterpret_cast<Abstract_t * > (base)), 
      functions.base.abstract.pv_Y(reinterpret_cast<Abstract_t * > (base)), 
      functions.base.abstract.virt_area(reinterpret_cast<Abstract_t * > (base)), 
      functions.base.Base_number(base) );

   base_expected.set(pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(base_expected, base_real);
}

void Derived_C_test::test_derived(int pv_n, char pv_c, double x, double pv_y, double z, double area, int n) {
   derived_real.set(functions.base.abstract.interface.pv_number (reinterpret_cast<Interface_t * > (derived)),
                  functions.base.abstract.interface.pv_char(reinterpret_cast<Interface_t * > (derived)),
      functions.base.abstract.X(reinterpret_cast<Abstract_t * > (derived)), 
      functions.base.abstract.pv_Y(reinterpret_cast<Abstract_t * > (derived)), 
      functions.base.abstract.virt_area(reinterpret_cast<Abstract_t * > (derived)), 
      functions.Derived_number(derived),
      functions.Z(derived));

   derived_expected.set(pv_n, pv_c, x, pv_y, area, n, z);
   derived_print_and_assert(derived_expected, derived_real);
}

void Derived_C_test::print_assert() {
   test_interface(DERIVED, DERIVED_CHAR);
   test_abstract(DERIVED, DERIVED_CHAR, 4.0, 101.0, 4.0 * 101.0 * 7.0, ABSTRACT);
   test_base(DERIVED, DERIVED_CHAR, 4.0, 101.0, 4.0 * 101.0 * 7.0, BASE);
   test_derived(DERIVED, DERIVED_CHAR, 4.0, 101.0, 7.0, 4.0 * 101.0 * 7.0, DERIVED);
}

const double INF = numeric_limits<double>::infinity();

Result_codes Derived_C_test::main_test_derived_linking() {
   Result_codes result = bind_execute_function_assert <Derived_t *, Abstract_t *, Abstract_t *>
         (derived, functions.base.abstract.X, 7.5, "x", __func__, functions.base.abstract.virt_set_X, 7.5);
   if (OK == result)
      result = incorrect_call<Derived_t *, Abstract_t *, Abstract_t *>
               (derived, functions.base.abstract.X, INF, "x", __func__, functions.base.abstract.virt_set_X,
                INF);
   if (OK == result) {
      print_and_assert(functions.base.abstract.X(reinterpret_cast<Abstract_t * > (derived)), 7.5, "x");
      result = bind_execute_function_assert<Derived_t *, Base_t *, Abstract_t *>
                        (derived, functions.base.abstract.pv_Y, 0.8, "y", __func__, functions.base.virt_set_Y, 0.8);
   }
   if (OK == result)
      result = incorrect_call<Derived_t *, Base_t *, Abstract_t *>
         (derived, functions.base.abstract.pv_Y, -105.8, "y", __func__, functions.base.virt_set_Y, -105.8); 
   if (OK == result) {
      print_and_assert(functions.base.abstract.pv_Y(reinterpret_cast<Abstract_t * > (derived)), 0.8, "y");
      result = bind_execute_function_assert<Derived_t *, Derived_t *, Derived_t *>
               (derived, functions.Z, -66.8, "z", __func__, functions.virt_set_Z, -66.8);
   }
   if (OK == result)
      result = incorrect_call<Derived_t *, Derived_t *, Derived_t *>
         (derived, functions.Z, -0.0, "z", __func__, functions.virt_set_Z, -0.0); 
   if (OK == result)
      result = incorrect_call<Derived_t *, Derived_t *, Derived_t *>
         (derived, functions.Z, 190.0, "z", __func__, functions.virt_set_Z, 190.0);
   if (OK == result) {
      print_and_assert(functions.Z(derived), -66.8, "z");
      print_and_assert(functions.base.abstract.virt_area(reinterpret_cast<Abstract_t * > (derived)), 7.5 * 0.8 * -66.8, "virt_area");
   }
   assert_many(result == OK, "result == ", result);
   return result;
}

Result_codes Derived_C_test::test_constructor() {
   Result_codes result = incorrect_construct(4, -3, 7); 
   if (result == OK)
      result = incorrect_construct(4, 55, 107);
   if (result == OK)
      result = incorrect_construct(4, 55, 0);
   if (result == OK)
      result = incorrect_construct(INF, 101, 7);
   if (result == OK)
      result = functions.init(&derived, 4, 101, 7);
   assert_many(derived != nullptr, "derived pointer == ", derived);
   assert_many(result == OK, "result == ", result);
   return result;
}

Result_codes Derived_C_test::test_derived_linking() {
   load_derived_functions();
   Result_codes result = test_constructor();
   if (result == OK) {
      print_assert();
      result = main_test_derived_linking();
   }
   functions.Derived_destroy(&derived);
#ifdef MANUAL_DLL_LOAD
   if (OK == result)
      result = static_cast<Result_codes> (close_handle(&(functions.base.handle)));
   else
      close_handle(&(functions.base.handle));
#endif
   assert_many(derived == nullptr, "derived pointer == ", derived);
   assert_many(result == OK, "result == ", result);
   return result; 
}

Result_codes test_derived_c() {
   function<Result_codes()> test = bind(&Derived_C_test::test_derived_linking);
   Result_codes result = call_catch_exception(test);
   assert_many(result == OK, "result == ", result);
   return result; 
}

}
