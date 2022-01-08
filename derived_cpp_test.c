
#include "print.h"
#include "derived_cpp_test.h"
#include "derived_connector.h"

#include <math.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_DERIVED_CONNECTOR_SO     "libderived_cpp_connector.so"
#endif

static Derived_connector functions;

static void load_derived_connector(void) {
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
   functions.abstract.handle = get_handle(LIB_DERIVED_CONNECTOR_SO, RTLD_LAZY);
   functions.init = get_symbol(functions.abstract.handle, "derived_cpp_init");
   functions.abstract.destroy = get_symbol(functions.abstract.handle, "derived_cpp_destroy");
   functions.abstract.interface.pv_number = get_symbol(functions.abstract.handle, "derived_cpp_pv_number");
   functions.abstract.interface.pv_char = get_symbol(functions.abstract.handle, "derived_cpp_pv_char");
   functions.abstract.X = get_symbol(functions.abstract.handle, "derived_cpp_X");
   functions.abstract.virt_set_X = get_symbol(functions.abstract.handle, "derived_cpp_virt_set_X");
   functions.abstract.pv_Y = get_symbol(functions.abstract.handle, "derived_cpp_pv_Y");
   functions.abstract.virt_set_Y = get_symbol(functions.abstract.handle, "derived_cpp_virt_set_Y");
   functions.abstract.virt_area = get_symbol(functions.abstract.handle, "derived_cpp_virt_area");
   functions.abstract.number = get_symbol(functions.abstract.handle, "derived_cpp_number");
   functions.Z = get_symbol(functions.abstract.handle, "derived_cpp_Z");
   functions.virt_set_Z = get_symbol(functions.abstract.handle, "derived_cpp_virt_set_Z");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   functions.abstract.handle = NULL;
   functions.init = derived_cpp_init;
   functions.abstract.destroy = derived_cpp_destroy;
   functions.abstract.interface.pv_number = derived_cpp_pv_number;
   functions.abstract.interface.pv_char = derived_cpp_pv_char;
   functions.abstract.X = derived_cpp_X;
   functions.abstract.virt_set_X = derived_cpp_virt_set_X;
   functions.abstract.pv_Y = derived_cpp_pv_Y;
   functions.abstract.virt_set_Y = derived_cpp_virt_set_Y;
   functions.abstract.virt_area = derived_cpp_virt_area;
   functions.abstract.number = derived_cpp_number;
   functions.Z = derived_cpp_Z;
   functions.virt_set_Z = derived_cpp_virt_set_Z;
#endif
}

static Result_codes incorrect_init(const double x, const double y, const double z) { 
   Result_codes result = functions.init(x, y, z); 
   assert_many(result == INVALID_ARG, "assert failed: ", "s d", "result == ", result);
   functions.abstract.destroy();
   if (result == INVALID_ARG)
      return OK;
   else
      return BAD_FUNTION_CALL;
}

static Result_codes test_constructor(void) { 
   Result_codes result = incorrect_init(4, -3, 7); 
   if (OK == result)
      result = incorrect_init(4, 55, 107); 
   if (OK == result)
      result = incorrect_init(4, 55, 0);
   if (OK == result)
      result = incorrect_init(INFINITY, 101, 7);
   if (OK == result)
      result = functions.init(4, 101, 7);
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   if (OK == result)
      result = check_double(functions.abstract.X, "Derived X", 4);
   if (OK == result)
      result = check_double(functions.abstract.pv_Y, "Derived pv_Y", 101);
   if (OK == result)
      result = check_double(functions.Z, "Derived Z", 7);
   if (OK == result)
      result = check_double(functions.abstract.virt_area, "Derived virt_area", 4 * 101 * 7);
   if (OK == result)
      result = check_int(functions.abstract.interface.pv_number, "Derived pv_number", DERIVED_STATICS.number);
   if (OK == result)
      result = check_int(functions.abstract.number, "Derived number", DERIVED_STATICS.number);
   if (OK == result)
      result = check_char(functions.abstract.interface.pv_char, "Derived pv_char", DERIVED_STATICS.ch);
   return result;
}

static Result_codes run_derived_connector(void) { 
   Result_codes result = test_constructor(); 
   if (OK == result)
      result = getset_coordination(functions.abstract.virt_set_X, 7.5, functions.abstract.X, "Derived X");
   if (OK == result)
      result = incorrect_set_coordination(functions.abstract.virt_set_X, INFINITY, functions.abstract.X, "Derived X");
   if (OK == result)
      result = check_double(functions.abstract.X, "Derived X", 7.5);
   
   if (OK == result)
      result = getset_coordination(functions.abstract.virt_set_Y, 0.8, functions.abstract.pv_Y, "Derived pv_Y"); 
   if (OK == result)
      result = incorrect_set_coordination(functions.abstract.virt_set_Y, -105.8, functions.abstract.pv_Y, "Derived pv_Y");
   if (OK == result)
      result = check_double(functions.abstract.pv_Y, "Derived pv_Y", 0.8);
   
   if (OK == result)
      result = getset_coordination(functions.virt_set_Z, -66.8, functions.Z, "Derived Z"); 
   if (OK == result)
      result = incorrect_set_coordination(functions.virt_set_Z, 0.0, functions.Z, "Derived Z");
   if (OK == result)
      result = incorrect_set_coordination(functions.virt_set_Z, 190.0, functions.Z, "Derived Z");
   if (OK == result)
      result = check_double(functions.Z, "Derived Z", -66.8);
   if (OK == result)
      result = check_double(functions.abstract.virt_area, "Derived virt_area", 7.5 * 0.8 * -66.8);
#ifdef MANUAL_DLL_LOAD
   if (OK == result) {
      functions.abstract.destroy();
      result = close_handle(&(functions.abstract.handle));
      assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
      return result;
   }
   close_handle(&(functions.abstract.handle));
#endif
   functions.abstract.destroy();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

Result_codes test_derived_cpp(void) {
   FUNCTION_INFO(__FUNCTION__);
   load_derived_connector();
   Result_codes result = run_derived_connector();
   return result;
}
