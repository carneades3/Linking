#ifndef PAIR_H 
#define PAIR_H 

typedef struct {
   char * string;
   double number;
} pair_double_str;

typedef struct {
   char * string;
   int number;
} pair_int_str;

typedef struct {
   char * string;
   char ch;
} pair_char_str;

#endif
