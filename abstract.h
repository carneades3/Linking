#ifndef ABSTRACT_H
#define ABSTRACT_H

#include "result_codes.h"

struct Abstract_t;

typedef struct Abstract_t Abstract_t;

#ifdef __cplusplus
extern "C" {
#endif

void Abstract_destroy(Abstract_t ** const);

double X(const Abstract_t * const);

Result_codes virt_set_X(Abstract_t * const, const double);

double pv_Y(const Abstract_t * const);

Result_codes Abstract_validate(const double, const char * const);

double virt_area(const Abstract_t * const);

int Abstract_number(const Abstract_t * const);

#ifdef  __cplusplus
}
#endif

extern const int ABSTRACT;
extern const char * const abstract_class_name;


#endif
