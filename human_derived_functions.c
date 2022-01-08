#include "human_derived_functions.h"
#include "print.h"
#include "utility.h"
#include "human.h"
#include "human_derived.h"

#include <string.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_HUMAN_DERIVED_SO     "libhumanderived.so"
#endif

static void load_human(Human_derived_functions * functions) {
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "human functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
   functions->handle = get_handle(LIB_HUMAN_DERIVED_SO, RTLD_LAZY);
   functions->init = get_symbol(functions->handle, "Human_derived_init");
   functions->set_age = get_symbol(functions->handle, "Human_derived_set_age");
   functions->get_age = get_symbol(functions->handle, "Human_derived_get_age");
   functions->destroy = get_symbol(functions->handle, "Human_derived_destroy");
   functions->set_name = get_symbol(functions->handle, "Human_set");
   functions->get_name = get_symbol(functions->handle, "Human_get_name");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   functions->handle = NULL;
   functions->init = Human_derived_init;
   functions->set_age = Human_derived_set_age;
   functions->get_age = Human_derived_get_age;
   functions->destroy = Human_derived_destroy;
   functions->set_name = Human_set;
   functions->get_name = Human_get_name;
#endif
}

static Result_codes check_name(const Human_derived_functions * const functions, const Human_derived_t * const human, const char * const expected_name) {
   REQUIRE_NON_NULL(functions, "functions derived is null");
   REQUIRE_NON_NULL(human, "human derived is null");
   REQUIRE_NON_NULL(expected_name, "expected_name is null");
   char * name = NULL;
   Result_codes result = functions->get_name((Human_t *)human, &name);
   if (OK == result) {
      LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
      assert_many(strcmp(name, expected_name) == 0, "assert failed: ", "s s", "name == ", name);
   }
   free(name);
   return result;
}

static Result_codes check_age(const Human_derived_functions * const functions, const Human_derived_t * const human, const unsigned int expected) {
   REQUIRE_NON_NULL(functions, "functions derived is null");
   REQUIRE_NON_NULL(human, "human derived is null");
   unsigned int age;
   Result_codes result = functions->get_age(human, &age);
   if (OK == result) {
      LOG("%s: %s human age = %u", LANGUAGE, __FUNCTION__, age);
      assert_many(age == expected, "assert failed: ", "s u", " age == ", age);
   }
   return result;
}

static Result_codes incorrect_human_init(const Human_derived_functions * const functions, 
                                         const char * const name, const unsigned int age) {
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "human functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
   Human_derived_t * human = NULL;
   Result_codes result = functions->init(&human, name, age); 
   assert_many(human == NULL, "assert failed: ", "s p", "pointer to human == ", human);
   assert_many(result == INVALID_ARG, "assert failed: ", "s d", "result == ", result);
   if (INVALID_ARG == result)
      return OK; 

   LOG("%s: %s incorrect call of human init result = %d", LANGUAGE, __FUNCTION__, result);
   return RUNTIME_ERROR;
}

static Result_codes run_human(const Human_derived_functions * const functions) { 
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "human functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
   Human_derived_t * human = NULL;
   Result_codes result = functions->init(&human, "Claudius Ptolemaeus", 9); 
   if (OK == result)
      result = check_name(functions, human, "Claudius Ptolemaeus"); 
   if (OK == result)
      result = check_age(functions, human, 9); 
   if (OK == result)
      result = functions->set_name((Human_t *)human, "Plato");
   if (OK == result)
      result = check_name(functions, human, "Plato"); 
   if (OK == result)
      result = functions->set_age(human, 84);
   if (OK == result)
      result = check_age(functions, human, 84); 
#ifdef MANUAL_DLL_LOAD
   if (OK == result) {
      functions->destroy(&human);
      assert_many(human == NULL, "assert failed: ", "s p", "pointer to human == ", human);
      result = close_handle(&(functions->handle));
      assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
      return result;
   }
   close_handle(&(functions->handle));
#endif
   if (human)
      functions->destroy(&human);
   assert_many(human == NULL, "assert failed: ", "s p", "pointer to human == ", human);
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

Result_codes test_human_derived(void) {
   FUNCTION_INFO(__FUNCTION__);
   Human_derived_functions functions;
   load_human(&functions); 
   Result_codes result = incorrect_human_init(&functions, "Plato", 150);
   if (OK == result)
      result = incorrect_human_init(&functions, "1R", 8);
   if (OK == result)
      result = run_human(&functions);
   return result;
}
