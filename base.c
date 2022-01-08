#include "interface.h"
#include "abstract.h"
#include "abstract_private.h"
#include "print.h"
#include "c_string.h"
#include "allocate.h"
#include "utility.h"
#include "base_private.h"
#include "base.h"

const char * const base_class_name = "Base";
const int BASE = 1;
const char BASE_CHAR = 'B';

int Base_number(const Base_t * const ptr) { 
   LOG("%s %s \n", base_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(ptr, "ptr is null");
   
   return BASE; 
}

int pv_number_B(const Base_t * const object) {
   LOG("%s %s \n", base_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "base is null");
   
   return BASE; 
}

char pv_char_B(const Base_t * const object) {
   LOG("%s %s \n", base_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "base is null");
   
   return BASE_CHAR; 
}

Result_codes Base_check(const Base_t * const object, const double n, const char * const function) {
   LOG("%s %s \n", base_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "base is null");
   REQUIRE_NON_NULL(function, "function is null");
   
   Result_codes result = Abstract_validate(n, function);
   if (OK == result && ! Base_pv_valid_B(object, n)) {
      LOG(" %s: argument of number: %f is not valid (n < 0)\n", function, n);
      result = INVALID_ARG;
   }
   return result;
}

bool_t Base_pv_valid_B(const Base_t * const object, const double n) { 
   LOG("%s %s \n", base_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "base is null");
   
   return (n >= 0); 
}

void Base_destroy_B(Base_t ** const object) {
   LOG("%s %s \n", base_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "base is null");
   REQUIRE_NON_NULL(*object, "*base is null");
   
   free((*object)->y);
   Abstract_destroy_A((Abstract_t **) object);
   /*free(*object);*/
   *object = NULL; /* to avoid double free or corruption (fasttop)  */
}

double pv_Y_B(const Base_t * const object) {
   LOG("%s %s \n", base_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "base is null");
   REQUIRE_NON_NULL(object->y, "pointer to y is null");
   
   return *(object->y);
}

Result_codes virt_set_Y_B(Base_t * const object, const double number) {
   LOG("%s %s \n", base_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "base is null");
   REQUIRE_NON_NULL(object->y, "pointer to y is null");
   
   Result_codes result = Base_check(object, number, __FUNCTION__);
   if (OK == result)
      *(object->y) = number;
   return result;
}

double virt_area_B(const Base_t * const object) {
   LOG("%s %s \n", base_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "base is null");
   
   return X((Abstract_t *) object) * *(object->y);
}

Result_codes Base_init(Base_t ** const object, const double x, const double y) {
   Result_codes result = Abstract_init((Abstract_t **)object, x);
   if (OK != result)
      return result;
   
   LOG("%s %s \n", base_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "base is null");
   REQUIRE_NON_NULL(*object, "*base is null - after call of Abstract_t struct");
   
   REALLOCATE_STRING(*object, sizeof (Base_t));
   if (! *object)
      return BAD_ALLOC;
   
   (*object)->abstract.pv_Y_f  = pv_Y_B;
   (*object)->virt_set_Y_f = virt_set_Y_B;
   /* (*object)->abstract.X_f ; */
   (*object)->abstract.virt_area_f = virt_area_B;
   (*object)->abstract.interface.pv_number_f  = pv_number_B;
   (*object)->abstract.interface.pv_char_f    = pv_char_B; 
   (*object)->abstract.interface.pv_valid_f    = &Base_pv_valid_B; 
   (*object)->abstract.interface.interface_destroy_f = Base_destroy_B;
   (*object)->abstract.abstract_destroy_f  = Base_destroy_B;
   (*object)->base_destroy_f  = Base_destroy_B;
   
   result = Base_check((const Base_t * const)(*object), y, __FUNCTION__);
   if (OK == result) {
      (*object)->y = (double *) copy_bytes(&y, sizeof(double));
      if (NULL == (*object)->y)
         result = BAD_ALLOC;
   }
   
   if (OK != result)
      Abstract_destroy_A(object);
   
   return result;
}

Result_codes virt_set_Y(Base_t * const object, const double n) {
   LOG("%s %s \n", base_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "base is null");
   
   return object->virt_set_Y_f(object, n);
}

void Base_destroy(Base_t ** const object) {
   LOG("%s %s \n", base_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "base is null");
   REQUIRE_NON_NULL(*object, "*base is null");
   
   return (*object)->base_destroy_f(object);
}
