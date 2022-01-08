#include "base_test.hpp"
#include "print.hpp"
#include "utility.hpp"
#include "base.hpp"

#include <functional>

using std::function;
using std::numeric_limits;

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
#endif
   
namespace tests {

using namespace Hierarchy;

#ifdef MANUAL_DLL_LOAD
   Manual_DLL_interface::Manual_DLL_interface(const char * shared_library, const char * create, const char * destroy) {
      handle   = get_handle(const_cast<char *> (shared_library), RTLD_LAZY);
      this->create   = reinterpret_cast<Base * (*)(const double, const double)> (get_symbol(handle, const_cast<char *> (create)));
      this->destroy  = reinterpret_cast<void (*)(Base * & )> (get_symbol(handle, const_cast<char *> (destroy)));
   }
   const Manual_DLL_interface Base_test::manual_interface("libbase_cpp.so", "base_create", "base_destroy");
   Base * Base_test::base;
#else
   Base Base_test::base {-6, 0};
#endif
   
Interface_expected Base_test::interface_expected;
Abstract_expected Base_test::abstract_expected;
Base_expected Base_test::base_expected;
Interface_real Base_test::interface_real;
Abstract_real Base_test::abstract_real;
Base_real Base_test::base_real;

#ifdef MANUAL_DLL_LOAD
Result_codes Base_test::load_base() {
   base = manual_interface.create(4, 8);
   return (nullptr == base) ? INCORRECT_VALUE : OK;
}
#endif
 
void Base_test::test_interface(int n, char c) {
#ifdef MANUAL_DLL_LOAD
   Interface * inter = base;
   interface_real.set(inter->pv_number(), inter->pv_char());
#else
   Interface & inter = base;
   interface_real.set(inter.pv_number(), inter.pv_char());
#endif
   interface_expected.set(n, c);
   interface_print_and_assert(interface_expected, interface_real);
}

void Base_test::test_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
#ifdef MANUAL_DLL_LOAD
   Abstract * abs = base;
   abstract_real.set(abs->pv_number(), abs->pv_char(), abs->X(), abs->pv_Y(), abs->virt_area(), abs->number());
#else
   Abstract & abs = base;
   abstract_real.set(abs.pv_number(), abs.pv_char(), abs.X(), abs.pv_Y(), abs.virt_area(), abs.number());
#endif
   abstract_expected.set(pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(abstract_expected, abstract_real);
}

void Base_test::test_base(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
#ifdef MANUAL_DLL_LOAD
   base_real.set(base->pv_number(), base->pv_char(), base->X(), base->pv_Y(), base->virt_area(), base->number());
#else
   base_real.set(base.pv_number(), base.pv_char(), base.X(), base.pv_Y(), base.virt_area(), base.number());
#endif
   base_expected.set(pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(base_expected, base_real);
}

#ifdef MANUAL_DLL_LOAD
Result_codes Base_test::test_base_linking() {
   cerr << "\nMANUAL DLL LOAD\n";
   Result_codes result = load_base();
   if (result == OK) {
      test_interface(Base::BASE, Base::BASE_CHAR);
      test_abstract(Base::BASE, Base::BASE_CHAR, 4.0, 8.0, 4.0 * 8.0, Abstract::ABSTRACT);
      test_base(Base::BASE, Base::BASE_CHAR, 4.0, 8.0, 4.0 * 8.0, Base::BASE);
      
      result = bind_execute_member_function_assert(base, &Base::X, 7.5, "x", __func__, &Base::virt_set_X, 7.5);
      if (OK == result)
         result = incorrect_member_call(base, &Base::pv_Y, -9.8, "y", __func__, &Base::virt_set_Y, -9.8);
      if (OK == result)
         result = bind_execute_member_function_assert(base, &Base::pv_Y, 105.8, "y", __func__, &Base::virt_set_Y, 105.8); 
      if (OK == result) {
         print_and_assert(base->virt_area(), 7.5 * 105.8, "virt_area");
         result = static_cast<Result_codes> (close_handle(&(manual_interface.handle)));
      }
   }
   manual_interface.destroy(base);
   if (result != OK)
      close_handle(&(manual_interface.handle));
   assert_many(result == OK, "result == ", result);
   assert_many(base == nullptr, "base pointer == ", base);
   return result;
}
#else
Result_codes Base_test::test_base_linking() {
   cerr << "\nAUTOMATIC DLL LOAD\n";
   test_interface(Base::BASE, Base::BASE_CHAR);
   test_abstract(Base::BASE, Base::BASE_CHAR, -6.0, 0.0, -6.0 * 0.0, Abstract::ABSTRACT);
   test_base(Base::BASE, Base::BASE_CHAR, -6.0, 0.0, -6.0 * 0.0, Base::BASE);

   Result_codes result = bind_execute_member_function_assert(base, &Base::X, 7.5, "x", __func__, &Base::virt_set_X, 7.5);
   if (OK == result)
      result = incorrect_member_call(base, &Base::pv_Y, numeric_limits<double>::infinity(), "y", __func__, &Base::virt_set_Y,        numeric_limits<double>::infinity());
   if (OK == result)
      result = bind_execute_member_function_assert(base, &Base::pv_Y, 105.8, "y", __func__, &Base::virt_set_Y, 105.8);
   if (OK == result) {
      print_and_assert(base.virt_area(), 7.5 * 105.8, "virt_area"); // brackets - multiline macro
   }
   
   assert_many(result == OK, "result == ", result);
   return result;
}
#endif

Result_codes test_base() {
   function<Result_codes()> test = bind(&Base_test::test_base_linking);
   Result_codes result = call_catch_exception(test);
   return result; 
}

}
