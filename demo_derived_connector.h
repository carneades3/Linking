#ifndef DEMO_DERIVED_CONNECTOR_H 
#define DEMO_DERIVED_CONNECTOR_H 

#include "result_codes.h"

#ifdef __cplusplus
#include "demo_derived.hpp"
   class Demo_derived_connector {
   private:
      static demo::Demo_derived * instance;
   public:
      static Result_codes init(const char * const name, const unsigned int age);
      static Result_codes set_name(const char * name);
      static Result_codes get_name(char ** const name);
      static Result_codes set_age(const unsigned int age);
      static Result_codes get_age(unsigned int * const age);
      static Result_codes destruct();
   };
#endif

#ifdef __cplusplus
extern "C" {
#endif
   
Result_codes demo_derived_init(const char * const name, const unsigned int age);
Result_codes demo_derived_set_name(const char * name);
Result_codes demo_derived_get_name(char ** const name);
Result_codes demo_derived_set_age(const unsigned int age);
Result_codes demo_derived_get_age(unsigned int * const age);
Result_codes demo_derived_destroy(void);
   
   
#ifdef  __cplusplus
}
#endif

#endif
