#include "regular_expr.h"
#include "print.h"
#include "result_codes.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>

Result_codes print_regerror (int errcode, size_t length, const regex_t * compiled) {
   if (! compiled) {
      LOG("%s: regex %p  is null", __FUNCTION__, compiled);
      return INVALID_ARG;
   } 
  char* buffer = calloc(length, sizeof(char));
  regerror (errcode, compiled, buffer, length);
  LOG("Regex match failed: %s\n", buffer);
  free(buffer);
  return OK;
}

void compile_regex(regex_t * regex, const char * pattern, const int cflags) {
   if (! pattern || ! regex) {
      LOG_EXIT(__FUNCTION__, "regex or regex's pattern is null\n", EXIT_FAILURE);
   }
   int result = regcomp(regex, pattern, cflags);
   if (0 != result) {
      size_t length = regerror (result, regex, NULL, 0);
      print_regerror (result, length, regex); 
      LOG_EXIT(__FUNCTION__, "Could not compile regex\n", EXIT_FAILURE);
   }
}

Result_codes match_regex(const regex_t * regex, const char * const str) {
   if (! str || ! regex) {
      LOG("%s  str: %p or regex %p  is null", __FUNCTION__, str, regex);
      return INVALID_ARG;
   } 
   int result = regexec (regex, str, 0, NULL, 0);
   switch (result) {
      case 0:
         return OK;
      case REG_NOMATCH:
         LOG (" argument of str: '%s' is not matches by regex\n", str);
         return INVALID_ARG;
      default:  {
         size_t length = regerror (result, regex, NULL, 0);
         print_regerror (result, length, regex);       
         return INVALID_ARG;
      }
   }
}

Result_codes regex_matches(const char *str, const char *pattern, const int cflags) {
   if (! pattern || ! str) {
      LOG("%s  pattern: %p or str %p  is null", __FUNCTION__, pattern, str);
      return INVALID_ARG;
   } 
   regex_t re;
   compile_regex(&re, pattern, cflags);
   Result_codes result = match_regex(&re, str);
   regfree(&re);
   return result;
}
