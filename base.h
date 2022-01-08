#ifndef BASE_H
#define BASE_H

#include "abstract.h"

struct Base_t;

typedef struct Base_t Base_t;

#ifdef __cplusplus
extern "C" {
#endif
   
Result_codes Base_init(Base_t ** const, const double, const double);   

void Base_destroy(Base_t ** const);

Result_codes virt_set_Y(Base_t * const, const double);

int Base_number(const Base_t * const);

#ifdef  __cplusplus
}
#endif

extern const int BASE;
extern const char BASE_CHAR;
extern const char * const base_class_name;


#endif
