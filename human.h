#ifndef HUMAN_H
#define HUMAN_H

#include <regex.h>

struct Human_t;

typedef struct Human_t Human_t;
/*
extern regex_t * NAME_REGEX;*/

#ifdef __cplusplus
extern "C" {
#endif
   
Human_t* Human_malloc(void);

int Human_init(Human_t ** const, const char * const);

void Human_destroy(Human_t ** const);

int Human_set(Human_t * const, const char * const);

int Human_get_name(const Human_t * const, char ** const);

void destroy_regex(void);

#ifdef  __cplusplus
}
#endif

typedef int bool_t;

#endif
