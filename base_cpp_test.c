
#include "print.h"
#include "base_cpp_test.h"
#include "base_connector.h"

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_BASE_CONNECTOR_SO     "libbase_cpp_connector.so"
#endif

static Base_connector functions;

static void load_base_connector(void) {
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
   functions.abstract.handle = get_handle(LIB_BASE_CONNECTOR_SO, RTLD_LAZY);
   functions.init = get_symbol(functions.abstract.handle, "base_cpp_init");
   functions.abstract.destroy = get_symbol(functions.abstract.handle, "base_cpp_destroy");
   functions.abstract.interface.pv_number = get_symbol(functions.abstract.handle, "base_cpp_pv_number");
   functions.abstract.interface.pv_char = get_symbol(functions.abstract.handle, "base_cpp_pv_char");
   functions.abstract.X = get_symbol(functions.abstract.handle, "base_cpp_X");
   functions.abstract.virt_set_X = get_symbol(functions.abstract.handle, "base_cpp_virt_set_X");
   functions.abstract.pv_Y = get_symbol(functions.abstract.handle, "base_cpp_pv_Y");
   functions.abstract.virt_set_Y = get_symbol(functions.abstract.handle, "base_cpp_virt_set_Y");
   functions.abstract.virt_area = get_symbol(functions.abstract.handle, "base_cpp_virt_area");
   functions.abstract.number = get_symbol(functions.abstract.handle, "base_cpp_number");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   functions.abstract.handle = NULL;
   functions.init = base_cpp_init;
   functions.abstract.destroy = base_cpp_destroy;
   functions.abstract.interface.pv_number = base_cpp_pv_number;
   functions.abstract.interface.pv_char = base_cpp_pv_char;
   functions.abstract.X = base_cpp_X;
   functions.abstract.virt_set_X = base_cpp_virt_set_X;
   functions.abstract.pv_Y = base_cpp_pv_Y;
   functions.abstract.virt_set_Y = base_cpp_virt_set_Y;
   functions.abstract.virt_area = base_cpp_virt_area;
   functions.abstract.number = base_cpp_number;
#endif
}

static Result_codes run_base_connector(void) { 
   Result_codes result = functions.init(5, 8); 
   if (OK == result)
      result = check_double(functions.abstract.X, "Base X", 5);
   if (OK == result)
      result = check_double(functions.abstract.pv_Y, "Base pv_Y", 8);
   if (OK == result)
      result = check_double(functions.abstract.virt_area, "Base virt_area", 5 * 8);
   if (OK == result)
      result = check_int(functions.abstract.interface.pv_number, "Base pv_number", BASE_STATICS.number);
   if (OK == result)
      result = check_int(functions.abstract.number, "Base number", BASE_STATICS.number);
   if (OK == result)
      result = check_char(functions.abstract.interface.pv_char, "Base pv_char", BASE_STATICS.ch);
   if (OK == result)
      result = incorrect_set_coordination(functions.abstract.virt_set_Y, -5.8, 
                                   functions.abstract.pv_Y, "Base pv_Y");
   if (OK == result)
      result = getset_coordination(functions.abstract.virt_set_X, 7,
                                functions.abstract.X, "Base X");
   if (OK == result)
      result = getset_coordination(functions.abstract.virt_set_Y, 99,
                                functions.abstract.pv_Y, "Base pv_Y"); 
   if (OK == result)
      result = check_double(functions.abstract.virt_area, "Base virt_area", 7 * 99);
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

Result_codes test_base_cpp(void) {
   FUNCTION_INFO(__FUNCTION__);
   load_base_connector(); 
   Result_codes result = run_base_connector();
   return result;
}
