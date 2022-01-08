#include "interface_private.h"
#include "interface.h"

#include "print.h"
#include "result_codes.h"
#include "c_string.h"
#include "utility.h"


#define MUST_NULL(pointer, pointer_name) \
if (NULL != pointer) { \
   LOG("%s %s \n", interface_class_name, __FUNCTION__); \
   LOG("%s must be null\n", pointer_name); \
   return INVALID_ARG; \
}

void Interface_destroy_I(Interface_t ** const object) {
   LOG("%s %s \n", interface_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "interface is null");
   REQUIRE_NON_NULL(*object, "*interface is null");
   
   free(*object);
   *object = NULL; /* to avoid double free or corruption (fasttop)  */
}

const int NB = -8;
const char * const interface_class_name = "Interface";

Interface_t * Interface_malloc(void) {
   LOG("%s %s \n", interface_class_name, __FUNCTION__);
   
   Interface_t * new = NULL;
   ALLOCATE_STRING(new, sizeof (Interface_t));
   return new;
}

Result_codes Interface_init(Interface_t ** const object) {
   LOG("%s %s \n", interface_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "interface is null");
   MUST_NULL(*object, "*interface");
   
   *object = Interface_malloc();
   if (! *object)
      return BAD_ALLOC;
   
   (*object)->pv_valid_f   = NULL;
   (*object)->pv_number_f  = NULL;
   (*object)->pv_char_f    = NULL;
   (*object)->interface_destroy_f = Interface_destroy_I;
   
   return OK;
}

bool_t pv_valid(const Interface_t * const object, const double n) {
   LOG("%s %s \n", interface_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "interface is null");
   
   return object->pv_valid_f(object, n);
}

int pv_number(const Interface_t * const object) {
   LOG("%s %s \n", interface_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "interface is null");
   
   return object->pv_number_f(object);
}

char pv_char(const Interface_t * const object) {
   LOG("%s %s \n", interface_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "interface is null");
   
   return object->pv_char_f(object);
}

void Interface_destroy(Interface_t ** const object) {
   LOG("%s %s \n", interface_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "interface is null");
   REQUIRE_NON_NULL(*object, "*interface is null");
   
   return (*object)->interface_destroy_f(object);
}
