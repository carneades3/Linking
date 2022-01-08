#include "demo_derived_functions.h"
#include "print.h"
#include "utility.h"
#include "connector.h"
#include "demo_derived_connector.h"

#include <string.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_DEMO_DERIVED_CONNECTOR_SO     "libdemoderivedconnector.so"
#endif

static void load_demo(Demo_derived_functions * functions) {
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
   functions->handle = get_handle(LIB_DEMO_DERIVED_CONNECTOR_SO, RTLD_LAZY);
   functions->init = get_symbol(functions->handle, "demo_derived_init");
   functions->set_age = get_symbol(functions->handle, "demo_derived_set_age");
   functions->get_age = get_symbol(functions->handle, "demo_derived_get_age");
   functions->destroy = get_symbol(functions->handle, "demo_derived_destroy");
   functions->set_name = get_symbol(functions->handle, "demo_derived_set_name");
   functions->get_name = get_symbol(functions->handle, "demo_derived_get_name");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   functions->handle = NULL;
   functions->init = demo_derived_init;
   functions->set_age = demo_derived_set_age;
   functions->get_age = demo_derived_get_age;
   functions->destroy = demo_derived_destroy;
   functions->set_name = demo_derived_set_name;
   functions->get_name = demo_derived_get_name;
#endif
}

static Result_codes check_name(Demo_derived_functions * functions, const char * const expected_name) {
   REQUIRE_NON_NULL(functions, "functions is null");
   REQUIRE_NON_NULL(expected_name, "expected_name is null");
   char * name = NULL;
   Result_codes result = functions->get_name(&name);
   if (OK == result) {
      LOG("%s: %s demo name = %s", LANGUAGE, __FUNCTION__, name);
      assert_many(strcmp(name, expected_name) == 0, "assert failed: ", "s s", "name == ", name);
   }
   return result;
}

static Result_codes check_age(Demo_derived_functions * functions, const unsigned int expected) {
   REQUIRE_NON_NULL(functions, "functions is null");
   unsigned int age;
   Result_codes result = functions->get_age(&age);
   if (OK == result) {
      LOG("%s: %s demo age = %u", LANGUAGE, __FUNCTION__, age);
      assert_many(age == expected, "assert failed: ", "s u", " age == ", age);
   }
   return result;
}

static Result_codes run_demo(const Demo_derived_functions * const functions) { 
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "demo functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
   Result_codes result = functions->init("Claudius Ptolemaeus", 9); 
   if (OK == result)
      result = check_name(functions, "Claudius Ptolemaeus"); 
   if (OK == result)
      result = check_age(functions, 9); 
   if (OK == result)
      result = functions->set_name("Plato");
   if (OK == result)
      result = check_name(functions, "Plato"); 
   if (OK == result)
      result = functions->set_age(84);
   if (OK == result)
      result = check_age(functions, 84); 
#ifdef MANUAL_DLL_LOAD
   if (OK == result) {
      functions->destroy();
      result = close_handle(&(functions->handle));
      assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
      return result;
   }
   close_handle(&(functions->handle));
#endif
   functions->destroy();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

Result_codes test_demo_derived(void) {
   FUNCTION_INFO(__FUNCTION__);
   Demo_derived_functions functions;
   load_demo(&functions); 
   Result_codes result = run_demo(&functions);
   return result;
}
