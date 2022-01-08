#include "demo_functions.h"
#include "print.h"
#include "utility.h"
#include "connector.h"

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
#endif

const int demo_functions_null = 3;

static void load_demo(Demo_functions * demo_functions) {
   if (! demo_functions) {
      FUNCTION_INFO(__FUNCTION__);
      LOG("%s\n", "demo_functions is NULL ");
      longjmp(JMP_BUF, demo_functions_null);
   }
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
   demo_functions->handle = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   demo_functions->init = get_symbol(demo_functions->handle, "demo_init");
   demo_functions->set_name = get_symbol(demo_functions->handle, "demo_set_name");
   demo_functions->get_name = get_symbol(demo_functions->handle, "demo_get_name");
   demo_functions->destroy = get_symbol(demo_functions->handle, "demo_destroy");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   demo_functions->handle = NULL;
   demo_functions->init = demo_init;
   demo_functions->set_name = demo_set_name;
   demo_functions->get_name = demo_get_name;
   demo_functions->destroy = demo_destroy;
#endif
}

static Result_codes run_demo(const Demo_functions * const demo_functions) { 
   if (! demo_functions) {
      LOG_EXIT(__FUNCTION__, "demo_functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
   Result_codes result = demo_functions->init("Nicolaus Copernicus"); 
   if (OK == result) {
      char * name = NULL;
      result = demo_functions->get_name(&name);
      if (OK == result) {
         LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
         assert_many(strcmp(name, "Nicolaus Copernicus") == 0, "assert failed: ", "s s", "name == ", name);
         result = demo_functions->set_name("Aristotle");
         if (OK == result) {
            name = NULL;
            result = demo_functions->get_name(&name);
            if (OK == result) {
               LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
               assert_many(strcmp(name, "Aristotle") == 0, "assert failed: ", "s s", "name == ", name);
               result = demo_functions->destroy();
#ifdef MANUAL_DLL_LOAD
               if (OK == result) {
                  result = close_handle(&(demo_functions->handle));
                  assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
                  return result;
               }
#endif
            }
         }
      }
   }
#ifdef MANUAL_DLL_LOAD
   close_handle(&(demo_functions->handle));
#endif
   demo_functions->destroy();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

Result_codes test_demo(void) {
   Demo_functions functions;
   load_demo(&functions); 
   Result_codes result = run_demo(&functions);
   return result;
}

Result_codes test_demo_linking(void) {
   volatile int jmp_value = 0;
   const int jmp_value_2 = 5;
   if ((jmp_value = setjmp(JMP_BUF)) != 0) {
      FUNCTION_INFO(__FUNCTION__);
      LOG("\nAfter calling test_print_many longjmp set value to %d\n", jmp_value);
      assert_many(jmp_value_2 == jmp_value, "assert failed: ", "s d", "jmp_value should be ", jmp_value_2);
      if ((jmp_value = setjmp(JMP_BUF)) != 0) {
         FUNCTION_INFO(__FUNCTION__);
         LOG("\nlongjmp set value to %d\n ", jmp_value);
         assert_many(demo_functions_null == jmp_value, "assert failed: ", "s d", "jmp_value should be ", demo_functions_null);
      }
      else 
         load_demo(NULL);

      return test_demo(); 
   }
   else 
      longjump_test(JMP_BUF, jmp_value_2);
}
