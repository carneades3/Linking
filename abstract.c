#include "interface.h"
#include "abstract.h"
#include "abstract_private.h"
#include "print.h"
#include "c_string.h"
#include "allocate.h"
#include "utility.h"

#include <math.h>

const char * const abstract_class_name = "Abstract";
const int ABSTRACT = 0;

int Abstract_number(const Abstract_t * const ptr) { 
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(ptr, "ptr is null");
   
   return ABSTRACT; 
}

int pv_number_A(const Abstract_t * const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   
   return ABSTRACT; 
}

void Abstract_destroy_A(Abstract_t ** const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(*object, "*abstract is null");
   
   free((*object)->x);
   Interface_destroy_I((Interface_t **) object);
   /*free(*object);*/
   *object = NULL; /* to avoid double free or corruption (fasttop)  */
}

double X_A(const Abstract_t * const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(object->x, "pointer to x is null");
   
   return *(object->x);
}

Result_codes virt_set_X_A(Abstract_t * const object, const double number) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(object->x, "pointer to x is null");
   
   Result_codes result = Abstract_validate(number, __FUNCTION__);
   if (OK == result)
      *(object->x) = number;
   return result;
}

double virt_area_A(const Abstract_t * const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(object->x, "pointer to x is null");
   
   return *(object->x);
}

Result_codes Abstract_init(Abstract_t ** const object, const double x) {
   Result_codes result = Interface_init((Interface_t **)object);
   if (OK != result)
      return result;
   
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(*object, "*abstract is null - after call of Interface_t struct");
   
   REALLOCATE_STRING(*object, sizeof (Abstract_t));
   if (! *object)
      return BAD_ALLOC;
   
   (*object)->pv_Y_f  = NULL;
   (*object)->X_f  = X_A;
   (*object)->virt_set_X_f = virt_set_X_A;
   (*object)->virt_area_f = virt_area_A;
   (*object)->interface.pv_number_f  = pv_number_A;
   /* (*object)->interface.pv_char_f    = NULL; */
   /* (*object)->interface.pv_valid_f    = NULL; */
   (*object)->interface.interface_destroy_f = Abstract_destroy_A;
   (*object)->abstract_destroy_f  = Abstract_destroy_A;
   
   result = Abstract_validate(x, __FUNCTION__);
   if (OK == result) {
      (*object)->x = (double *) copy_bytes(&x, sizeof(double));
      if (NULL == (*object)->x)
         result = BAD_ALLOC;
   }
   
   if (OK != result)
      Interface_destroy_I(object);

   return result;
}

double pv_Y(const Abstract_t * const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   
   return object->pv_Y_f(object);
}

double X(const Abstract_t * const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   
   return object->X_f(object);
}

double virt_area(const Abstract_t * const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   
   return object->virt_area_f(object);
}

Result_codes virt_set_X(Abstract_t * const object, const double n) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   
   return object->virt_set_X_f(object, n);
}

void Abstract_destroy(Abstract_t ** const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(*object, "*abstract is null");
   
   return (*object)->abstract_destroy_f(object);
}

Result_codes Abstract_validate(const double number, const char * const function) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(function, "function is null");
   
   if (! isfinite(number)) {
      LOG(" %s: argument of number: %f is not finite \n", function, number);
      return INVALID_ARG;
   }
   return OK;
}
