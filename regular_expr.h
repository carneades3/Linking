#ifndef REGULAR_EXPR_H
#define REGULAR_EXPR_H

#include <regex.h>

int print_regerror (int errcode, size_t length, const regex_t * compiled);
int regex_matches(const char *str, const char *pattern, const int cflags);/*
regex_t compile_regex(const char * pattern, const int cflags);*/
void compile_regex(regex_t * regex, const char * pattern, const int cflags);
int match_regex(const regex_t * regex, const char * const str);

#endif
