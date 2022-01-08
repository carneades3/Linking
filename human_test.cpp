#include "human_test.hpp"
#include "print.hpp"
#include "utility.hpp"

#include "human_functions.h"
#include "human.h"

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
#endif

//#define LIB_HUMAN_SO     "libhuman.so"

namespace tests {

static void load_human_functions(Human_functions & functions) {
#ifdef MANUAL_DLL_LOAD
   cerr << "\nMANUAL DLL LOAD\n";
   functions.handle   = get_handle("libhuman.so", RTLD_LAZY);
   functions.init     = reinterpret_cast<Result_codes (*)(Human_t **, const char * const)> (get_symbol(functions.handle, "Human_init"));
   functions.set_name = reinterpret_cast<Result_codes (*)(Human_t * const, const char * const)> (get_symbol(functions.handle, "Human_set"));
   functions.get_name = reinterpret_cast<Result_codes (*)(const Human_t * const, char ** const)> (get_symbol(functions.handle, "Human_get_name"));
   functions.destroy  = reinterpret_cast<void (*)(Human_t ** const)> (get_symbol(functions.handle, "Human_destroy"));
#else
   cerr << "\nAUTOMATIC DLL LOAD\n";
   functions.handle = nullptr;
   functions.init = reinterpret_cast<Result_codes (*)(Human_t **, const char * const)> (Human_init);
   functions.set_name = reinterpret_cast<Result_codes (*)(Human_t * const, const char * const)> (Human_set);
   functions.get_name = reinterpret_cast<Result_codes (*)(const Human_t * const, char ** const)> (Human_get_name);
   functions.destroy = reinterpret_cast<void (*)(Human_t ** const)> (Human_destroy);
#endif
}

Result_codes test_human() {
   Human_functions functions;
   load_human_functions(functions);
   Human_t * human = nullptr;
   Human_t ** const human_2ptr = &human;
   Result_codes result = functions.init(human_2ptr, "Baruch Spinoza"); 
   if (OK == result) {
      char * name = nullptr;
      result = functions.get_name(human, &name);
      if (OK == result) {
         cerr << TIE( "C++", unmove(__cplusplus), __func__, " human name = ", name) << '\n';
         assert_many(string(name) == "Baruch Spinoza", "name == ", name);
         free(name);
         result = functions.set_name(human, "Newton");
         if (OK == result) {
            name = nullptr;
            result = functions.get_name(human, &name);
            if (OK == result) {
               cerr << TIE( "C++", unmove(__cplusplus), __func__, " human name = ", name) << '\n';
               assert_many(string(name) == "Newton", "name == ", name);
               free(name);
               name = nullptr;
#ifdef MANUAL_DLL_LOAD
               functions.destroy(human_2ptr);
               assert_many(human == nullptr, "human == ", human);
               assert_many(*human_2ptr == nullptr, "*human_2ptr == ", human);
               result = static_cast<Result_codes> (close_handle(&(functions.handle)));
               assert_many(result == OK, "result == ", result);
               return result;
#endif
            }
         }
      }
      if (name)
         free(name);
   }
#ifdef MANUAL_DLL_LOAD
   close_handle(&(functions.handle));
#endif
   if (human)
      functions.destroy(human_2ptr);
   assert_many(human == nullptr, "human == ", human);
   assert_many(*human_2ptr == nullptr, "*human_2ptr == ", human);
   assert_many(result == OK, "result == ", result);
   return result;
}
   
}
