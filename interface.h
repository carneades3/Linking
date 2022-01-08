#ifndef INTERFACE_H
#define INTERFACE_H

#include "result_codes.h"

typedef int bool_t;

struct Interface_t;

typedef struct Interface_t Interface_t;

#ifdef __cplusplus
extern "C" {
#endif

void Interface_destroy(Interface_t ** const);

int pv_number(const Interface_t * const);

char pv_char(const Interface_t * const);

#ifdef  __cplusplus
}
#endif

extern const int NB;
extern const char * const interface_class_name;

#endif
