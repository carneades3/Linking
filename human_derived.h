#ifndef HUMAN_DERIVED_H
#define HUMAN_DERIVED_H

struct Human_derived_t;

typedef struct Human_derived_t Human_derived_t;

#ifdef __cplusplus
extern "C" {
#endif
   
/* Human_derived_t* Human_derived_malloc(void); */

int Human_derived_init(Human_derived_t ** const, const char * const, const unsigned int);

void Human_derived_destroy(Human_derived_t ** const);

int Human_derived_set_age(Human_derived_t * const, const unsigned int);

int Human_derived_get_age(const Human_derived_t * const, unsigned int * const);

#ifdef  __cplusplus
}
#endif

typedef int bool_t;

#endif
