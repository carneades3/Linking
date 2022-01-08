#include "demo_derived_functions.h"
#include "print.h"
#include "utility.h"
#include "connector.h"
#include "base_connector.h"

#include <string.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_BASE_CPP_SO     "libbase_cpp.so"
#endif

#include "abstract_connector.h"
#include "demo_derived_connector.h"
#include "demo_derived.hpp"
#include "utility.hpp"
#include "null.hpp"

using namespace Hierarchy;
using namespace std;


Result_codes demo_derived_init(const char * const name, const unsigned int age) {
   Result_codes result = Base_connector::pv_number(name, age);
   return result;
}

Result_codes demo_derived_set_name(const char * name) {
   Result_codes result = Base_connector::set_name(name);
   return result;
}

Result_codes demo_derived_get_name(char ** name) {
   Result_codes result = Base_connector::get_name(name);
   return result;
}

Result_codes demo_derived_set_age(const unsigned int age) {
   Result_codes result = Base_connector::set_age(age);
   return result;
}

Result_codes demo_derived_get_age(unsigned int * const age) {
   Result_codes result = Base_connector::get_age(age);
   return result;
}

Result_codes demo_derived_destroy(void) {
   Result_codes result = Base_connector::destruct();
   return result;
}
