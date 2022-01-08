#include "utility.hpp"
#include "print.hpp"
#include "variadic_template_test.hpp"
#include "demo_test.hpp"
#include "demo_derived_test.hpp"
#include "human_test.hpp"
#include "human_derived_test.hpp"
#include "human.h"
#include "base_test.hpp"
#include "base_c_test.hpp"
#include "derived_test.hpp"
#include "derived_c_test.hpp"

#include <functional>

using namespace tests;

//using namespace std::placeholders;
using std::function;
using std::bind;
using std::initializer_list;

void set_handler(void (*func)(void)) { 
   std::set_terminate(func); 
}

void handle_terminate(void) { 
   cerr << TIE( "C++", unmove(__cplusplus), __func__, " handler called - must've been some exception ?!\n");
}

static void at_exit () {
   destroy_regex();
}

template <typename Function, typename... Args>  
static inline  Result_codes call_function(Function && func, Args&&... args) {
   Result_codes result = func(forward<Args>(args)...);
   assert_many(result == OK, "result == ", result);
   return result;
}

static Result_codes main_tests() {
   Result_codes result = UNRECOGNIZED_ERROR;
   initializer_list < function <Result_codes()> > vec = { bind(tests::test_demo), bind(tests::test_demo_derived), 
                     bind(tests::test_human), bind(tests::test_human_derived)
                     , bind(tests::test_money)
                     , bind(tests::test_base), bind(tests::test_derived)
                     , bind(tests::test_base_c), bind(tests::test_derived_c)
      
   };
   for (auto fun : vec) {
      result = call_function(fun);
      if (OK != result)
         break;
   }
   return result;   
}

static Result_codes main_linking_tests() {
   atexit (at_exit);
   Result_codes result = main_tests();
   return result;
}

int main(const int, const char * argv[]) {
   try {
      cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';
      size_t pos = string(argv[0]).rfind('/');
      const string program_name = (pos != string::npos && pos + 1 < string(argv[0]).size()) ? string(argv[0]).substr(pos + 1) : argv[0];
      cerr << "\n  Program name = " << program_name << '\n';
      set_handler(handle_terminate);
      Result_codes result = main_linking_tests();
      cerr << "\n  Program name = " << program_name;
      cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';
      cerr << " Final result = " << result << '\n';
      assert_many(result == OK, "result == ", result);
      return result;
   } 
   catch (const std::invalid_argument & e) {
     cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   } catch (const std::out_of_range& e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   }
   catch (const std::bad_alloc & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   }
   catch (const std::exception & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   }
   catch (...) {
      cerr << "Unrecognized Exception: " << '\n';
   }
   return EXIT_FAILURE;
}
