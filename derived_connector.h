#ifndef DERIVED_CONNECTOR_H
#define DERIVED_CONNECTOR_H

#include "abstract_connector.h"

#ifdef __cplusplus

#include "derived.hpp"

   using Hierarchy::Derived;

   class Derived_connector : public Abstract_connector<Derived> {
   public:
      Result_codes virt_set_Z(const double);
      Result_codes Z(double * const);
   };
#endif

#ifdef __cplusplus
extern "C" {
#endif

Result_codes derived_cpp_pv_number(int * const);
Result_codes derived_cpp_pv_char( char * const);
Result_codes derived_cpp_X(double * const );
Result_codes derived_cpp_virt_set_X(const double);
Result_codes derived_cpp_pv_Y(double * const );
Result_codes derived_cpp_virt_set_Y(const double);
Result_codes derived_cpp_Z(double * const );
Result_codes derived_cpp_virt_set_Z(const double);
Result_codes derived_cpp_virt_area(double * const );
Result_codes derived_cpp_number(int * const );

Result_codes derived_cpp_init(const double, const double, const double);
Result_codes derived_cpp_destroy(void);
   
   
#ifdef  __cplusplus
}
#endif

extern const Static_number_char DERIVED_STATICS;

#endif
