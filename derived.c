#include "interface.h"
#include "base.h"
#include "base_private.h"
#include "print.h"
#include "c_string.h"
#include "allocate.h"
#include "utility.h"
#include "derived_private.h"
#include "derived.h"

const char * const derived_class_name = "Derived";
const int DERIVED = 2;
const char DERIVED_CHAR = 'D';

int pv_number_D(const Derived_t * const object) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");
   
   return DERIVED; 
}

char pv_char_D(const Derived_t * const object) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");
   
   return DERIVED_CHAR; 
}

int Derived_number(const Derived_t * const ptr) { 
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(ptr, "ptr is null");
   
   return DERIVED; 
}

Result_codes Derived_check(const Derived_t * const object, const double n, const char * const function) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");
   REQUIRE_NON_NULL(function, "function is null");
   
   Result_codes result = Derived_validate(n, function);
   if (OK == result && ! pv_valid(object, n)) {
      LOG(" %s: argument of number: %f is not valid (n > 100)\n", function, n);
      result = INVALID_ARG;
   }
   return result;
}

Result_codes Derived_validate(const double number, const char * const function) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(function, "function is null");
   
   if (0 == number) {
      LOG(" %s: argument of number: %f = 0 is not valid \n", function, number);
      return INVALID_ARG;
   }
   return OK;
}

Result_codes virt_set_Z_D(Derived_t * const object, const double number) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");
   REQUIRE_NON_NULL(object->z, "pointer to z is null");
   
   Result_codes result = Derived_check(object, number, __FUNCTION__);
   if (OK == result)
      *(object->z) = number;
   return result;
}

bool_t Derived_pv_valid_D(const Derived_t * const object, const double n) { 
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");
   
   return (n <= 100); 
}

void Derived_destroy_D(Derived_t ** const object) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");
   REQUIRE_NON_NULL(*object, "*derived is null");
   
   free((*object)->z);
   Base_destroy_B((Base_t **) object);
   /*free(*object);*/
   *object = NULL; /* to avoid double free or corruption (fasttop)  */
}

double Z_D(const Derived_t * const object) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");
   REQUIRE_NON_NULL(object->z, "pointer to z is null");
   
   return *(object->z);
}

Result_codes virt_set_Y_D(Derived_t * const object, const double number) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");/*
   REQUIRE_NON_NULL(object->base.y, "pointer to y is null");*/
   
   Result_codes result = Derived_check(object, number, __FUNCTION__);
   if (OK == result)
      result = virt_set_Y_B((Base_t *)object, number);
   return result;
}

Result_codes virt_set_X_D(Derived_t * const object, const double number) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");/*
   REQUIRE_NON_NULL(object->base.abstract.x, "pointer to x is null");*/
   
   Result_codes result = Derived_check(object, number, __FUNCTION__);
   if (OK == result)
      result = virt_set_X_A((Abstract_t *)object, number);
   return result;
}

double virt_area_D(const Derived_t * const object) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");
   
   return virt_area_B((Base_t *) object) * *(object->z);
}

Result_codes Derived_init(Derived_t ** const object, const double x, const double y, double z) {
   Result_codes result = Base_init((Base_t **)object, x, y);
   if (OK != result)
      return result;
   
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");
   REQUIRE_NON_NULL(*object, "*derived is null - after call of Base_t struct");
   
   REALLOCATE_STRING(*object, sizeof (Derived_t));
   if (! *object)
      return BAD_ALLOC;
   
   (*object)->Z_f = Z_D;
   (*object)->virt_set_Z_f = virt_set_Z_D;
   /*(*object)->base.abstract.pv_Y_f ;*/
   (*object)->base.virt_set_Y_f = virt_set_Y_D;
   /* (*object)->base.abstract.X_f */
   (*object)->base.abstract.virt_area_f = virt_area_D;
   (*object)->base.abstract.interface.pv_number_f  = pv_number_D;
   (*object)->base.abstract.interface.pv_char_f    = pv_char_D; 
   (*object)->base.abstract.interface.pv_valid_f    = &Derived_pv_valid_D; 
   
   (*object)->base.abstract.interface.interface_destroy_f = Derived_destroy_D;
   (*object)->base.abstract.abstract_destroy_f  = Derived_destroy_D;
   (*object)->base.base_destroy_f  = Derived_destroy_D;
   (*object)->derived_destroy_f  = Derived_destroy_D;
   
   result = Base_check((const Base_t * const)(*object), z, __FUNCTION__);
   if (OK == result)
      result = Derived_check((const Derived_t * const)(*object), z, __FUNCTION__);
   if (OK == result) {
      (*object)->z = (double *) copy_bytes(&z, sizeof(double));
      if (NULL == (*object)->z)
         result = BAD_ALLOC;
   }
   
   if (OK != result)
      Base_destroy_B(object);
   
   return result;
}

double Z(const Derived_t * const object) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");
   
   return object->Z_f(object);
}

Result_codes virt_set_Z(Derived_t * const object, const double n) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");
   
   return object->virt_set_Z_f(object, n);
}

void Derived_destroy(Derived_t ** const object) {
   LOG("%s %s \n", derived_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "derived is null");
   REQUIRE_NON_NULL(*object, "*derived is null");
   
   return (*object)->derived_destroy_f(object);
}
