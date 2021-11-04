#ifndef CONNECTOR_H 
#define CONNECTOR_H 

#ifdef __cplusplus
 #define EXTERNC extern "C"
 #else
 #define EXTERNC
 #endif

typedef void* Money_int;

EXTERNC Money_int Money_int__init_1(const char * dollars);
EXTERNC Money_int Money_int__create_1(const char * dollars);
EXTERNC Money_int Money_int__init_2(const char * dollars, const long double cents);
EXTERNC Money_int Money_int__create_2(const char * dollars, const long double cents);

EXTERNC void demo_init(const char * name);
EXTERNC void demo_set_name(const char * name);
EXTERNC const char * demo_get_name();
EXTERNC void demo_destroy();

#undef EXTERNC

#endif
