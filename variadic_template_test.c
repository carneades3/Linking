#include "variadic_template_test.h"
#include "c_string.h"
#include "print.h"

#include <limits.h>
#include <string.h>
#include <float.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
#endif 

char * get_format(const Number type) { 
   switch(type) {
      case SHORT: 
         return "hd";
      case U_SHORT: 
         return "hu";
      case INT: 
         return "d";
      case U_INT: 
         return "u";
      case LONG: 
         return "ld";
      case U_LONG: 
         return "lu";
      case LONG_LONG: 
         return "lld";
      case U_LONG_LONG: 
         return "llu";
      case FLOAT:
      case DOUBLE:
         return "G";
      case LONG_DOUBLE:
         return "Lg";
      default: {
         const char * type_string = to_string((int)type);
         const char * message = concatenate_many_free_args(" improper type = ", 0, type_string, 1, NULL);
         LOG_EXIT_FREE(__FUNCTION__, message, EXIT_FAILURE);
      }
   } 
}

int print_union_member(const Number type, union Number_pointer_union n_union, const char * format) {
   switch(type) {
      case SHORT: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.s);
      case U_SHORT: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.us);
      case INT: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.i);
      case U_INT: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.ui);
      case LONG: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.l);
      case U_LONG: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.ul);
      case LONG_LONG: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.ll);
      case U_LONG_LONG:  
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.ull);
      case FLOAT:
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.f);
      case DOUBLE: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.d);
      case LONG_DOUBLE:
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.ld);
      default: {
         LOG(" improper type = %d", type);
         return INVALID_ARG;
      }
   } 
}

#define LOGS_MONEY(money, n_union, format, type) \
LOG("\nAddress of money is: %p", &money); \
LOG("\nAddress pointed by money is: %p", money); \
LOG("\nAddress of n_union is: %p\n", &n_union); \
print_union_member(type, n_union, format); \
LOG("%c", '\n')

typedef Result_codes (*p_func_many)(Money_type * money_ptr, const Money_functions function, const Number type, 
                                  union Number_pointer_union * n_union, const char * ,... );

static Result_codes run_money(const Number type, const char * const dollars, const long double cents) {
   if (type < SHORT || type > LONG_DOUBLE || ! dollars) {
      LOG(" type = %d \t dollars = %p\n", type, dollars);
      return INVALID_ARG;
   }
#ifdef MANUAL_DLL_LOAD
   void* handle = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   p_func_many p_function = get_symbol(handle, "Money_type__function");
#else
   p_func_many p_function = Money_type__function;
#endif
   Money_type money = NULL;
   union Number_pointer_union n_union;
   const char * format = get_format(type);
   Result_codes result = p_function(&money, INIT_1, type, &n_union, dollars);
   if (result == OK) {
      LOGS_MONEY(money, n_union, format, type);
      money = NULL;
      result = p_function(&money, CREATE_1, type, &n_union, dollars);
      if (result == OK) {
         LOGS_MONEY(money, n_union, format, type);
         money = NULL;
         result = p_function(&money, INIT_2, type, &n_union, dollars, cents);  
         if (result == OK) {
            LOGS_MONEY(money, n_union, format, type);
            money = NULL;
            result = p_function(&money, CREATE_2, type, &n_union, dollars, cents); 
            if (result == OK) {
               LOGS_MONEY(money, n_union, format, type);
#ifdef MANUAL_DLL_LOAD
               if (OK == result) {
                  result = close_handle(&handle); 
                  assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
                  return result;
               }
#endif
            }
         }
      }
   }
#ifdef MANUAL_DLL_LOAD
   close_handle(&handle);
#endif 
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

static Result_codes incorrect_money_init(const p_func_many p_function, const Number type, const char * const dollars, const long double cents) {
   if (type < SHORT || type > LONG_DOUBLE || ! dollars) {
      LOG(" type = %d \t dollars = %p\n", type, dollars);
      return INVALID_ARG;
   }

   Money_type money = NULL;
   union Number_pointer_union n_union;
   const char * format = get_format(type);
   Result_codes result = p_function(&money, INIT_2, type, &n_union, dollars, cents);
   assert_many(money == NULL, "assert failed: ", "s p", "pointer to money == ", money);
   if (INVALID_ARG == result || OUT_OF_RANGE_ERROR == result)
      return OK; 
   
   LOG("%s: %s incorrect call of money init result = %d", LANGUAGE, __FUNCTION__, result);
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return RUNTIME_ERROR;
}

static Result_codes incorrect_money(void) {
#ifdef MANUAL_DLL_LOAD
   void* handle = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   p_func_many p_function = get_symbol(handle, "Money_type__function");
#else
   p_func_many p_function = Money_type__function;
#endif
   
   Result_codes result = incorrect_money_init(p_function, INT, "20i", 80.0L);
   if (OK == result)
      result = incorrect_money_init(p_function, SHORT, "200000", 80.0L);
   
#ifdef MANUAL_DLL_LOAD
   if (OK == result)
      result = close_handle(&handle);
   else
      close_handle(&handle);
#endif
   return result;
}

#undef LOGS_MONEY

#define TEST_ALLOC(TYPE, dollars, number, cents) \
if (OK == result) { \
   dollars = to_string((number)); \
   result = run_money(TYPE, dollars, cents); \
   free(dollars); \
}

#define LOG_MAX(TYPE, number, format) \
format = get_format(TYPE); \
LOG(#TYPE " = " #number " = %s", ""); \
print_many("", format, number); \
LOG(" %c ", '\n')

Result_codes test_money(void) {
   Result_codes result = OK;
   char * max_dollars;
   char * format;
   LOG_MAX(SHORT, SHRT_MIN, format);
   TEST_ALLOC(SHORT, max_dollars, SHRT_MIN / 110, 0);
   LOG_MAX(SHORT, SHRT_MIN / 110, format);
   
   TEST_ALLOC(U_SHORT, max_dollars, USHRT_MAX / 110, 0);
   LOG_MAX(U_SHORT, USHRT_MAX / 110, format);
   
   TEST_ALLOC(INT, max_dollars, INT_MIN / 110, 0);
   TEST_ALLOC(U_INT, max_dollars, UINT_MAX / 110, 0);
   TEST_ALLOC(LONG, max_dollars, LONG_MIN / 110, 0);
   TEST_ALLOC(U_LONG, max_dollars, ULONG_MAX / 110, 0);
   TEST_ALLOC(LONG_LONG, max_dollars, LLONG_MIN / 110, 0);
   TEST_ALLOC(U_LONG_LONG, max_dollars, ULLONG_MAX / 110, 0);
   TEST_ALLOC(FLOAT, max_dollars, FLT_MAX  / 110, 0);
   TEST_ALLOC(DOUBLE, max_dollars, DBL_MIN  * 100, 0);
   TEST_ALLOC(LONG_DOUBLE, max_dollars, LDBL_MIN * 100, 0);
   
   LOG_MAX(FLOAT, FLT_MAX  / 110, format);
   LOG_MAX(SHORT, SHRT_MIN, format);
   LOG_MAX(LONG_LONG, LLONG_MAX, format);
   LOG_MAX(LONG_LONG, LLONG_MIN, format);
   char * number_string = to_string(LLONG_MIN);
   LOG("%s\n", number_string);
   free(number_string);
   
   if (OK == result)
      result = incorrect_money();
   return result;
}
