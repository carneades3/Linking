#include "print.hpp"
#include "file_edit.hpp"
#include "system.hpp"

#include "result_codes.h"

#include <string>
#include <cerrno>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <sys/wait.h>

#include <dirent.h>
#include <unistd.h>

using std::exception;
using std::cerr;
using std::string;
using std::system_error;
using std::invalid_argument;

void makefile() {
   system_utility::execute("make", "make", "clean");
   File_edit file_edit("Makefile");
   file_edit.edit_makefile();
   //system_utility::execute("make", "make");
   system_utility::call_system("make 2> compilation_output.txt");
   cerr << "Parent process: pid = " << getpid() << '\n';
}

int test_linking(const bool valgrind) {
   static const string ld_path = "LD_LIBRARY_PATH=.";
   static const string exec = "./linking_test_cpp";
   int result = OK;
   //int result = system_utility::call_system(ld_path + " " + exec);
   if (OK == result && valgrind) {
      static const string valgrind_str = "valgrind --leak-check=full --show-leak-kinds=all --exit-on-first-error=yes --error-exitcode=1";
      result = system_utility::call_system(ld_path + " " + valgrind_str + " " + exec);
   }
   return result;
}

int main(const int argc, const char * argv[]) {
   try {
      cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';
      size_t pos = string(argv[0]).rfind('/');
      const string program_name = (pos != string::npos && pos + 1 < string(argv[0]).size()) ? string(argv[0]).substr(pos + 1) : argv[0];
      cerr << "\n  Program name = " << program_name << '\n';
      const bool valgrind = (argc == 2 && string(argv[1]) == "valgrind") ? true : false;
      int result = test_linking (valgrind);
      assert_many(result == OK, "result == ", result);
      if (result == OK) {
         makefile();
         result = test_linking(valgrind);
      }
      cerr << "\n  Program name = " << program_name;
      cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';
      cerr << " Final result = " << result << '\n';
      assert_many(result == OK, "result == ", result);
      return result;
   } 
   catch (const system_error& e) {
      cerr  << __func__ << " " << typeid(e).name() << '\n';
      cerr << "Caught system_error with code " << e.code() << " meaning: " << e.what() << '\n';
      return e.code().value();
   }
   catch (const invalid_argument & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   }
   catch (const exception & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   }
   catch (...) {
      cerr << "Unrecognized Exception: " << '\n';
   }
   return EXIT_FAILURE;
}
