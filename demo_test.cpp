#include "demo_test.hpp"
#include "print.hpp"
#include "utility.hpp"
#include "demo.hpp"

#include "connector.h"

#include <functional>

using namespace std::placeholders;
using std::function;

using std::bind;

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   //#include "demo_functions.h"
   #include "shared_lib_open.h"
#endif
   
namespace tests {

using namespace demo;

#ifdef MANUAL_DLL_LOAD
typedef struct Manual_DLL_interface {
   Demo * (*create)(const string &);
   void (*destroy)(Demo * & );
   void * handle;
} Manual_DLL_interface;

static void load_demo(Manual_DLL_interface & demo_functions) {
   demo_functions.handle   = get_handle("libdemo.so", RTLD_LAZY);
   demo_functions.create   = reinterpret_cast<Demo * (*)(const string & )> (get_symbol(demo_functions.handle, "demo_create"));
   demo_functions.destroy  = reinterpret_cast<void (*)(Demo * & )> (get_symbol(demo_functions.handle, "demo_destroy"));
}

static Result_codes test_demo_linking() {
   cerr << "\nMANUAL DLL LOAD\n";
   Manual_DLL_interface demo_functions;
   load_demo(demo_functions);
   Demo * human = demo_functions.create("Marcus Aurelius"); 
   if (nullptr == human) {
      close_handle(&(demo_functions.handle));
      return INVALID_ARG;
   }
   string name = human->get_name();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, " human name = ", name) << '\n';
   assert_many(name == "Marcus Aurelius", "name == ", name);
   //Result_codes result = bind_execute_member_function(human, &Demo::set_name, "Socrates");
   function<void(const string&)> set = bind(&Demo::set_name, human, _1);
   Result_codes result = call_catch_exception(set, "Socrates");
   if (OK != result)
      close_handle(&(demo_functions.handle));
   else {
      name = human->get_name();
      cerr << TIE( "C++", unmove(__cplusplus), __func__, " human name = ", name) << '\n';
      assert_many(name == "Socrates", "name == ", name);
      result = static_cast<Result_codes> (close_handle(&(demo_functions.handle)));
   }
   demo_functions.destroy(human);
   assert_many(result == OK, "result == ", result);
   assert_many(human == nullptr, "human pointer == ", human);
   return result;
}
#else
static Result_codes test_demo_linking() {
   cerr << "\nAUTOMATIC DLL LOAD\n";
   Demo human = {"Leibniz"};
   string name = human.get_name();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, name) << '\n';
   assert_many(name == "Leibniz", "name == ", name);
   function<void(const string&)> set = bind(&Demo::set_name, std::ref(human), _1);
   Result_codes result = call_catch_exception(set, "Descartes");
   name = human.get_name();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, name) << '\n';
   assert_many(name == "Descartes", "name == ", name);
   return result;
}
#endif

Result_codes test_demo() {
   function<Result_codes()> test = bind(&test_demo_linking);
   Result_codes result = call_catch_exception(test);
   return result; 
}

}
