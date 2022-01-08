#include "human.h"
#include "human_private.h"
#include "print.h"
#include "result_codes.h"
#include "regular_expr.h"
#include "c_string.h"
#include "singleton.h"
#include "utility.h"

#include <string.h>

const char * const NAME_REGEX_STRING = "^[[:upper:]][[:lower:]]*( [[:upper:]][[:lower:]]*)?$";

SINGLETON_HEAP(regex_t, compile_regex, NAME_REGEX_STRING, REG_EXTENDED);

static regex_t * NAME_REGEX = NULL;

#define INIT_REGEX   \
if (NULL == NAME_REGEX)    \
      NAME_REGEX = Singleton_Heap_regex_t();

Human_t* Human_malloc(void) {
   LOG_FUNC(__FUNCTION__);
   Human_t * new = NULL;
   ALLOCATE_STRING(new, sizeof (Human_t));
   return new;
}

static Result_codes validate(const char * const name) {
   if (! NAME_REGEX)
      return INCORRECT_VALUE;
   Result_codes result = match_regex(NAME_REGEX, name);
   return result;
   /*
   regex_t ** NAME_REGEX = Singleton_Heap_regex_t();
   Result_codes result = match_regex(*NAME_REGEX, name);*/
}

Result_codes Human_init(Human_t ** const object, const char * const name) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "human is null");
   REQUIRE_NON_NULL(name, "name is null");
   if (*object) {
      LOG_FUNC(__FUNCTION__);
      LOG("%s\n", "*object must be null");
      return INVALID_ARG;
   }
   /*
   Result_codes result = regex_matches(name, NAME_REGEX_STRING, REG_EXTENDED);*/
   INIT_REGEX
   Result_codes result = validate(name);
   if (OK != result)
      return result;
   
   *object = Human_malloc();
   if (! *object)
      return BAD_ALLOC;
   
   (*object)->name = copy_string(name);
   if (NULL == (*object)->name)
      return BAD_ALLOC;
   return OK;
}

void destroy_regex(void) {
   /*regex_t * NAME_REGEX = Singleton_Heap_regex_t();*/
   if (NAME_REGEX) {
      regfree(NAME_REGEX);
      free(NAME_REGEX);
      NAME_REGEX = NULL;
   }
   else {
      LOG("\n%s: NAME_REGEX is %p\n", __FUNCTION__, NAME_REGEX);
   }
}

void Human_destroy(Human_t ** const object) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "human is null");
   REQUIRE_NON_NULL(*object, "*human is null");
   free((*object)->name);
   free(*object);
   *object = NULL; /* to avoid double free or corruption (fasttop)  */
   /*
   regex_t ** NAME_REGEX = Singleton_Heap_regex_t();
   regfree(*NAME_REGEX);*/
   /*destroy_regex();*/
}
/*
void Human_destroy_protected(Human_t * const object) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "human is null");
   free(object->name);
   destroy_regex();
}
*/
Result_codes Human_set(Human_t * const object, const char * const name) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "human is null");
   REQUIRE_NON_NULL(name, "name is null");
   /*
   Result_codes result = regex_matches(name, NAME_REGEX_STRING, REG_EXTENDED);*/
   Result_codes result = validate(name);
   if (OK != result)
      return result;
   
   free(object->name);
   object->name = copy_string(name);
   if (NULL == object->name)
      return BAD_ALLOC;
   return OK;
}

Result_codes Human_get_name(const Human_t * const object, char ** const name) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "human is null");
   REQUIRE_NON_NULL(name, "name is null");
   if (*name) {
      LOG("%s\n", "*name must be null");
      return INVALID_ARG;
   }
   *name = copy_string(object->name);
   if (NULL == *name)
      return BAD_ALLOC;
   return OK;
}

#undef INIT_REGEX
