#undef MANUAL_DLL_LOAD

#include "print.hpp"
#include "system.hpp"
#include "result_codes.h"

#include <string>

using std::string;

#define VALGRIND

int test(int tests, const string & command) {
   if (tests < 0 || tests > 99) 
      throw std::invalid_argument(__func__ + string("improper tests = ") + std::to_string(tests));
   int result = OK;
   for (int i = 0; i < tests && OK == result; i++)
      result = system_utility::call_system(command);
   return result;
}

int main(const int, const char ** argv) {
   const size_t pos = string(argv[0]).rfind('/');
   const string program_name = (pos != string::npos && pos + 1 < string(argv[0]).size()) ? string(argv[0]).substr(pos + 1) : argv[0];
   cerr << "\n  Program name = " << program_name << '\n';
   const char * command = "./main_cpp";
   int result = test(2, command);
#ifdef VALGRIND
   if (result == OK) {
      command = "LD_LIBRARY_PATH=. valgrind --leak-check=full --show-leak-kinds=all --exit-on-first-error=yes --error-exitcode=1 ./main_cpp valgrind";
      result = test(2, command);
   }
#endif
   cerr << "\n  Program name = " << program_name;
   cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';
   cerr << " Final result = " << result << '\n';
   assert_many(result == OK, "result == ", result);
   return result;
}
