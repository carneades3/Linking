#ifndef CONNECTOR_H 
#define CONNECTOR_H 

#include "result_codes.h"

typedef enum {
   INIT_1                 = -101,
   CREATE_1               = -102,
   INIT_2                 = -103,
   CREATE_2               = -104
} Money_functions;

typedef enum {
   SHORT       = 49,
   U_SHORT     = 50,
   INT         = 51,
   U_INT       = 52,
   LONG        = 53,
   U_LONG      = 54,
   LONG_LONG   = 55,
   U_LONG_LONG = 56,
   FLOAT       = 57,
   DOUBLE      = 58,
   LONG_DOUBLE = 59
} Number;

union Number_pointer_union {
   short                s;
   unsigned short       us;
   int                  i;
   unsigned int         ui;
   long                 l;
   unsigned long        ul;
   long long            ll;
   unsigned long long   ull;
   float                f;
   double               d;
   long double          ld;
};

#ifdef __cplusplus
 #define EXTERNC extern "C"
 #else
 #define EXTERNC
 #endif
 
EXTERNC void set_handler(void (*pfunc)(void));

typedef void* Money_type ;

EXTERNC Result_codes Money_type__function(Money_type * money_ptr, const Money_functions function, const Number type, 
                                  union Number_pointer_union * const n_union, const char * dollars, ... );

EXTERNC Result_codes Money_type__init_1(Money_type * money_ptr, const char * dollars);
EXTERNC Result_codes Money_type__create_1(Money_type * money_ptr, const char * dollars);
EXTERNC Result_codes Money_type__init_2(Money_type * money_ptr, const char * dollars, const long double cents);
EXTERNC Result_codes Money_type__create_2(Money_type * money_ptr, const char * dollars, const long double cents);

EXTERNC Result_codes demo_init(const char * name);
EXTERNC Result_codes demo_set_name(const char * name);
EXTERNC Result_codes demo_get_name(char ** name);
EXTERNC Result_codes demo_destroy(void);

#undef EXTERNC

#ifdef MANUAL_DLL_LOAD
   #define LIB_CONNECTOR_SO "libconnector.so"
#endif

#endif
