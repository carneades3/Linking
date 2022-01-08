#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
#endif

#include "human_derived_test.hpp"
#include "print.hpp"
#include "utility.hpp"

#include "human_derived_functions.h"
#include "human.h"
#include "human_derived.h"

namespace tests {

static void load_human_functions(Human_derived_functions & functions) {
#ifdef MANUAL_DLL_LOAD
   cerr << "\nMANUAL DLL LOAD\n";
   functions.handle   = get_handle("libhumanderived.so", RTLD_LAZY);
   functions.init     = reinterpret_cast<Result_codes (*)(Human_derived_t ** const, const char * const, const unsigned int)> (get_symbol(functions.handle, "Human_derived_init"));
   functions.set_name = reinterpret_cast<Result_codes (*)(Human_t * const, const char * const)> (get_symbol(functions.handle, "Human_set"));
   functions.get_name = reinterpret_cast<Result_codes (*)(const Human_t * const, char ** const)> (get_symbol(functions.handle, "Human_get_name"));
   functions.set_age = reinterpret_cast<Result_codes (*)(Human_derived_t * const, const unsigned int)> (get_symbol(functions.handle, "Human_derived_set_age"));
   functions.get_age = reinterpret_cast<Result_codes (*)(const Human_derived_t * const, unsigned int * const)> (get_symbol(functions.handle, "Human_derived_get_age"));
   functions.destroy  = reinterpret_cast<void (*)(Human_derived_t ** const)> (get_symbol(functions.handle, "Human_derived_destroy"));
#else
   cerr << "\nAUTOMATIC DLL LOAD\n";
   functions.handle = nullptr;
   functions.init = reinterpret_cast<Result_codes (*)(Human_derived_t ** const, const char * const, const unsigned int)> (Human_derived_init);
   functions.set_name = reinterpret_cast<Result_codes (*)(Human_t * const, const char * const)> (Human_set);
   functions.get_name = reinterpret_cast<Result_codes (*)(const Human_t * const, char ** const)> (Human_get_name);
   functions.set_age = reinterpret_cast<Result_codes (*)(Human_derived_t * const, const unsigned int)> (Human_derived_set_age);
   functions.get_age = reinterpret_cast<Result_codes (*)(const Human_derived_t * const, unsigned int * const)> (Human_derived_get_age);
   functions.destroy = reinterpret_cast<void (*)(Human_derived_t ** const)> (Human_derived_destroy);
#endif
}

static Result_codes check_name(const Human_derived_functions & functions, const Human_derived_t & human, const string & expected_name) {
   char * name = NULL;
   Result_codes result = functions.get_name((Human_t *)&human, &name);
   if (OK == result) {
      cerr << TIE( "C++", unmove(__cplusplus), __func__, " human name = ", name) << '\n';
      assert_many(name == expected_name, "name == ", name);
   }
   free(name);
   return result;
}

static Result_codes check_age(const Human_derived_functions & functions, const Human_derived_t & human, const unsigned int expected) {
   unsigned int age;
   Result_codes result = functions.get_age(&human, &age);
   if (OK == result) {
      cerr << TIE( "C++", unmove(__cplusplus), __func__, " human age = ", age) << '\n';
      assert_many(age == expected, " age == ", age);
   }
   return result;
}

static Result_codes incorrect_human_init(const Human_derived_functions & functions, 
                                         const string & name, const unsigned int age) {
   Human_derived_t * human = nullptr;
   Result_codes result = functions.init(&human, name.c_str(), age); 
   assert_many(human == nullptr, "pointer to human == ", human);
   assert_many(result == INVALID_ARG, "result == ", result);
   if (INVALID_ARG == result)
      return OK; 
   
   cerr << TIE( "C++", unmove(__cplusplus), __func__, " result = ", result) << '\n';
   return RUNTIME_ERROR;
}

static Result_codes test_human_derived(Human_derived_functions & functions) {
   Human_derived_t * human = nullptr;
   Human_derived_t ** const human_2ptr = &human;
   Result_codes result = functions.init(human_2ptr, "Baruch Spinoza", 8); 
   if (OK == result)
      result = check_name(functions, *human, "Baruch Spinoza"); 
   if (OK == result)
      result = check_age(functions, *human, 8); 
   if (OK == result)
      result = functions.set_name((Human_t *)human, "Immanuel Kant");
   if (OK == result)
      result = check_name(functions, *human, "Immanuel Kant"); 
   if (OK == result)
      result = functions.set_age(human, 84);
   if (OK == result)
      result = check_age(functions, *human, 84);
#ifdef MANUAL_DLL_LOAD
   if (OK == result) {
      functions.destroy(human_2ptr);
      assert_many(human == nullptr, "human == ", human);
      assert_many(*human_2ptr == nullptr, "*human_2ptr == ", human);
      result = static_cast<Result_codes> (close_handle(&(functions.handle)));
      assert_many(result == OK, "result == ", result);
      return result;
   }
   close_handle(&(functions.handle));
#endif
   if (human)
      functions.destroy(human_2ptr);
   assert_many(human == nullptr, "human == ", human);
   assert_many(*human_2ptr == nullptr, "*human_2ptr == ", human);
   assert_many(result == OK, "result == ", result);
   return result;
}

Result_codes test_human_derived() {
   Human_derived_functions functions;
   load_human_functions(functions);
   Result_codes result = incorrect_human_init(functions, "Plato", 0);
   if (OK == result)
      result = incorrect_human_init(functions, "Carneades_", 8);
   if (OK == result)
      result = test_human_derived(functions);
   return result;
}
   
}
