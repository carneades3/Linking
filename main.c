/*
#define MANUAL_DLL_LOAD
*/
#include "print.h"
#include "result_codes.h"
#include "file_modify.h"
#include "system.h"
#include "c_string.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

Result_codes change_makefile(void) {
   struct File_modify_t * modifier = NULL;
   Result_codes result = File_modify_init(&modifier, "Makefile");
   if (result == OK)
      result = File_modify_set(modifier, "Makefile");  /* only for test  */
   if (result == OK) {
      char * filename = NULL;
      result = File_modify_get_filename(modifier, &filename);
      LOG("File name = %s\n", filename);
      free(filename);
   }
   if (result == OK)
      result = edit_makefile(modifier);
   File_modify_destroy(&modifier);
   free(modifier);
   assert_many(modifier == NULL, "assert failed: ", "s p", "pointer to File_modify_t == ", modifier);
   return OK;
}

Result_codes make_clean_make(void) {
   char *exec_args[] = { "make", "clean", NULL, NULL};
   int result = execute(exec_args);
   if (result != SYSTEM_ERROR) 
      result = change_makefile();
   if (result == OK) { 
      exec_args[0] = "make 2> compilation_output.txt";
      result = call_system(exec_args[0]);
      /*
      exec_args[1] = NULL;
      if (execute(exec_args) != SYSTEM_ERROR)
         result = OK;*/
   }
   LOG("Parent process: pid = %d\nGoodbye!\n", getpid());
   return result;
}

int test_linking(const bool_t valgrind) {
   FUNCTION_INFO(__FUNCTION__);
   static const char * const ld_path = "LD_LIBRARY_PATH=.";
   static const char * const exec = "./c_linking_test";
   /*
   const char * command = concatenate_many(ld_path, " ", exec, NULL);
   int result = call_system(command);
   free(command);*/
   const char * command = NULL;
   int result = OK;
   if (OK == result && valgrind) {
      static const char * const valgrind_str = "valgrind --leak-check=full --show-leak-kinds=all --exit-on-first-error=yes --error-exitcode=1 --tool=memcheck --track-origins=yes";
      command = concatenate_many(ld_path, " ", valgrind_str, " ", exec, NULL);
      result = call_system(command);
      free(command);
   }
   return result;
}

int main(const int argc, const char * argv[]) {
   const char * program_name = strrchr(argv[0], '/');
   LOG(" Program name = %s\n", program_name ? ++program_name : argv[0]);
   FUNCTION_INFO(__FUNCTION__);
   
   const bool_t valgrind = (argc == 2 && strcmp(argv[1], "valgrind") == 0) ? 1 : 0;
   int result = test_linking (valgrind);
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   if (result == OK)
      result = make_clean_make();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   if (result == OK)
      result = test_linking(valgrind);
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   LOG(" Program name = %s", program_name);
   FUNCTION_INFO(__FUNCTION__);
   LOG(" Final result = %d\n", result);
   return result;
}
