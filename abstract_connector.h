#ifndef ABSTRACT_CONNECTOR_H
#define ABSTRACT_CONNECTOR_H

#include "result_codes.h"

typedef struct Static_number_char {
   const int number;
   const char ch;
} Static_number_char;

#ifdef __cplusplus

   #include "base.hpp"
   #include "null.hpp"
   #include "utility.hpp"
   
   using Hierarchy::Base;
   
   using std::is_base_of;

   //template <class T, enable_if_t<is_base_of<Base, T>::value, bool> = true>
   template <class T>
   class Abstract_connector {
      static_assert(is_base_of<Base, T>::value);
   public:
      static const int  NUMBER;
      static const char CHAR; 
      
      Result_codes pv_number(int * const);
      Result_codes pv_char(char * const);
      Result_codes X(double * const);
      Result_codes virt_set_X(const double);
      Result_codes pv_Y(double * const);
      Result_codes virt_set_Y(const double);
      Result_codes virt_area(double * const);
      Result_codes number(int * const);
      
      Result_codes destruct();
      
      template <typename... Args>
      Result_codes init(Args ...args);
      
      virtual ~Abstract_connector() { };
   protected:
      static T * instance;
//       Abstract_connector();
   };
   
   template <class T>
   Result_codes Abstract_connector<T>::pv_number(int * const n) {
      if (OK != check_pointer(n, __func__, " Error number pointer")) 
         return INVALID_ARG;
      *n = instance->pv_number();
      return OK;
   }
   
   template <class T>
   Result_codes Abstract_connector<T>::pv_char(char * const c) {
      if (OK != check_pointer(c, __func__, " Error char pointer")) 
         return INVALID_ARG;
      *c = instance->pv_char();
      return OK;
   }
   
   template <class T>
   Result_codes Abstract_connector<T>::X(double * const x) {
      if (OK != check_pointer(x, __func__, " Error number pointer")) 
         return INVALID_ARG;
      *x = instance->X();
      return OK;
   }
   
   template <class T>
   Result_codes Abstract_connector<T>::virt_set_X(const double x) {
      Result_codes result = bind_execute_member_function(instance, &T::virt_set_X, x);
      return result;
   }
   
   template <class T>
   Result_codes Abstract_connector<T>::pv_Y(double * const y) {
      if (OK != check_pointer(y, __func__, " Error number pointer")) 
         return INVALID_ARG;
      *y = instance->pv_Y();
      return OK;
   }
   
   template <class T>
   Result_codes Abstract_connector<T>::virt_set_Y(const double y) {
      Result_codes result = bind_execute_member_function(instance, &T::virt_set_Y, y);
      return result;
   }
   
   template <class T>
   Result_codes Abstract_connector<T>::virt_area(double * const area) {
      if (OK != check_pointer(area, __func__, " Error double pointer")) 
         return INVALID_ARG;
      *area = instance->virt_area();
      return OK;
   }
   
   template <class T>
   Result_codes Abstract_connector<T>::number(int * const number) {
      if (OK != check_pointer(number, __func__, " Error int pointer")) 
         return INVALID_ARG;
      *number = instance->number();
      return OK;
   }
   
   template <class T> 
   template <typename... Args>
   Result_codes Abstract_connector<T>::init(Args ...args) {
      Result_codes result = ::init<T>(instance, Constructor<T>(), std::forward<Args>(args)...);
      return result;
   }
   
   template <class T>
   Result_codes Abstract_connector<T>::destruct() {
      Result_codes result = Destructor<T>()(instance);
      return result;
   }

#endif

#ifdef __cplusplus
extern "C" {
#endif
   
#ifdef  __cplusplus
}
#endif

#endif
