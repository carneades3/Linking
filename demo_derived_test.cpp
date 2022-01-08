#include "demo_derived_test.hpp"
#include "print.hpp"
#include "utility.hpp"

#include "connector.h"

#include <functional>

using namespace std::placeholders;
using std::function;

using std::bind;

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   //#include "demo_functions.h"
   #include "shared_lib_open.h"
#else
   #include <functional>
   
   using namespace std::placeholders;
#endif
   
namespace tests {

using namespace demo;

#ifdef MANUAL_DLL_LOAD
typedef struct Manual_DLL_interface {
   Demo_derived * (*create)(const string & , const unsigned int);
   void (*destroy)(Demo_derived * & );
   void * handle;
} Manual_DLL_interface;

static void load_demo_derived(Manual_DLL_interface & demo_functions) {
   demo_functions.handle   = get_handle("libdemoderived.so", RTLD_LAZY);
   demo_functions.create   = reinterpret_cast<Demo_derived * (*)(const string &, const unsigned int )> (get_symbol(demo_functions.handle, "demo_derived_create"));
   demo_functions.destroy  = reinterpret_cast<void (*)(Demo_derived * & )> (get_symbol(demo_functions.handle, "demo_derived_destroy"));
}

static Result_codes test_demo_derived_linking() {
   cerr << "\nMANUAL DLL LOAD\n";
   Manual_DLL_interface demo_functions;
   load_demo_derived(demo_functions);
   Demo_derived * human = demo_functions.create("Marcus Aurelius", 77); 
   if (nullptr == human) {
      close_handle(&(demo_functions.handle));
      return INCORRECT_VALUE;
   }
   print_and_assert(human->get_name(), string("Marcus Aurelius"), "name");
   print_and_assert(human->get_age(), 77u, "age");

   Result_codes result = bind_execute_member_function(human, &Demo_derived::set_name, "Socrates");
   if (OK == result) {
      print_and_assert(human->get_name(), string("Socrates"), "name");
      result = bind_execute_member_function(human, &Demo_derived::set_age, 28);
      if (OK == result) {
         assert_many(result == OK, "result == ", result);
         print_and_assert(human->get_age(), 28u, "age");
         result = static_cast<Result_codes> (close_handle(&(demo_functions.handle)));
      }
   }
   demo_functions.destroy(human);
   if (OK != result)
      close_handle(&(demo_functions.handle));
   assert_many(result == OK, "result == ", result);
   assert_many(human == nullptr, "human pointer == ", human);
   return result;
}
#else
static Result_codes test_demo_derived_linking() {
   cerr << "\nAUTOMATIC DLL LOAD\n";
   Demo_derived human = {"Leibniz", 54};
   print_and_assert(human.get_name(), string("Leibniz"), "name");
   print_and_assert(human.get_age(), 54u, "age");
   
   function<void(const string&)> set_name = bind(&Demo_derived::set_name, std::ref(human), _1);
   Result_codes result = call_catch_exception(set_name, "Descartes");
   if (OK != result)
      return result;
   print_and_assert(human.get_name(), string("Descartes"), "name");
   
   function<void(const unsigned int )> set_age = bind(&Demo_derived::set_age, std::ref(human), _1);
   result = call_catch_exception(set_age, 23);
   print_and_assert(human.get_age(), 23u, "age");
   return result;
}
#endif

Result_codes test_demo_derived() {
   function<Result_codes()> test = bind(&test_demo_derived_linking);
   Result_codes result = call_catch_exception(test);
   return result; 
}

}
