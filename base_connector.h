#ifndef BASE_CONNECTOR_H
#define BASE_CONNECTOR_H

#include "abstract_connector.h"

#ifdef __cplusplus

#include "base.hpp"

   using Hierarchy::Base;

   class Base_connector : public Abstract_connector<Base> {
   public:
      //Result_codes init(const double, const double);
   };
#endif

#ifdef __cplusplus
extern "C" {
#endif

Result_codes base_cpp_pv_number(int * const);
Result_codes base_cpp_pv_char( char * const);
Result_codes base_cpp_X(double * const );
Result_codes base_cpp_virt_set_X(const double);
Result_codes base_cpp_pv_Y(double * const );
Result_codes base_cpp_virt_set_Y(const double);
Result_codes base_cpp_virt_area(double * const );
Result_codes base_cpp_number(int * const );

Result_codes base_cpp_init(const double, const double);
Result_codes base_cpp_destroy(void);
   
#ifdef  __cplusplus
}
#endif

extern const Static_number_char BASE_STATICS;

#endif
