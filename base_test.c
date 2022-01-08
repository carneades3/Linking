
#include "print.h"
#include "utility.h"
#include "base_test.h"

#include <math.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_BASE_SO     "libbase_c.so"
#endif

static Interface_expected interface_expected = { {"pv_number", 0}, {"pv_char", '0'} };

static Abstract_expected abstract_expected = { { {"pv_number", 0}, {"pv_char", '0'} },
                {"x", 0}, {"y", 0}, {"virt_area", 0}, {"number", 0} };

static Base_expected base_expected = { { { {"pv_number", 0}, {"pv_char", '0'} },
                {"x", 0}, {"y", 0}, {"virt_area", 0}, {"number", 0} } };

static Interface_real interface_real =  { 0, '0'};

static Abstract_real abstract_real = { { 0, '0'}, 0, 0, 0, 0 };

static Base_real base_real = { { { 0, '0'}, 0, 0, 0, 0 } };

static Base_functions functions;

static Base_t * base = NULL;

static void assert_interface(int n, char c) {  
   Interface_t * inter = (Interface_t *) base;
   set_interface_real(&interface_real,
            functions.abstract.interface.pv_number(inter), functions.abstract.interface.pv_char(inter));
   set_interface_expected(&interface_expected, n, c);
   interface_print_and_assert(&interface_expected, &interface_real);
}

static void assert_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   Abstract_t * abs = (Abstract_t *) base;
   set_abstract_real(&abstract_real,
      functions.abstract.interface.pv_number(abs), functions.abstract.interface.pv_char(abs),
      functions.abstract.X(abs), functions.abstract.pv_Y(abs), functions.abstract.virt_area(abs),
      functions.abstract.Abstract_number(abs));
   set_abstract_expected(&abstract_expected, pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(&abstract_expected, &abstract_real);
}

static void assert_base(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   set_base_real(&base_real,
      functions.abstract.interface.pv_number(base), functions.abstract.interface.pv_char(base),
      functions.abstract.X(base), functions.abstract.pv_Y(base), functions.abstract.virt_area(base),
      functions.Base_number(base));
   set_base_expected(&base_expected, pv_n, pv_c, x, pv_y, area, n);
   base_print_and_assert(&base_expected, &base_real);
}

static void load_base_functions(void) {
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
   functions.handle = get_handle(LIB_BASE_SO, RTLD_LAZY);
   functions.init = get_symbol(functions.handle, "Base_init");
   functions.Base_destroy = get_symbol(functions.handle, "Base_destroy");
   functions.abstract.interface.Interface_destroy = get_symbol(functions.handle, "Interface_destroy");
   
   functions.abstract.interface.pv_number = get_symbol(functions.handle, "pv_number");
   functions.abstract.interface.pv_char = get_symbol(functions.handle, "pv_char");
   functions.abstract.X = get_symbol(functions.handle, "X");
   functions.abstract.virt_set_X = get_symbol(functions.handle, "virt_set_X");
   functions.abstract.pv_Y = get_symbol(functions.handle, "pv_Y");
   functions.virt_set_Y = get_symbol(functions.handle, "virt_set_Y");
   functions.abstract.virt_area = get_symbol(functions.handle, "virt_area");
   
   functions.abstract.Abstract_number = get_symbol(functions.handle, "Abstract_number");
   functions.Base_number = get_symbol(functions.handle, "Base_number");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   functions.handle = NULL;
   functions.init = Base_init;
   functions.Base_destroy = Base_destroy;
   functions.abstract.interface.Interface_destroy = Interface_destroy;
   
   functions.abstract.interface.pv_number = pv_number;
   functions.abstract.interface.pv_char = pv_char;
   functions.abstract.X = X;
   functions.abstract.virt_set_X = virt_set_X;
   functions.abstract.pv_Y = pv_Y;
   functions.virt_set_Y = virt_set_Y;
   functions.abstract.virt_area = virt_area;
   
   functions.abstract.Abstract_number = Abstract_number;
   functions.Base_number = Base_number;
#endif
}

#ifdef MANUAL_DLL_LOAD
static Result_codes test_base_linking(void) {
   Result_codes result = functions.init(&base, 4.0, 8.0);
   if (result == OK) {
      assert_interface(BASE, BASE_CHAR);
      assert_abstract(BASE, BASE_CHAR, 4.0, 8.0, 4.0 * 8.0, ABSTRACT);
      assert_base(BASE, BASE_CHAR, 4.0, 8.0, 4.0 * 8.0, BASE);
      
      result = getset_double(functions.abstract.virt_set_X, base, 7.5, functions.abstract.X, "Base x");
      if (OK == result)
         result = incorrect_set_double(functions.virt_set_Y, base, -9.8, functions.abstract.pv_Y, "Base y");
      if (OK == result)
         result = getset_double(functions.virt_set_Y, base, 105.8, functions.abstract.pv_Y, "Base y"); 
      if (OK == result) {
         print_and_assert(functions.abstract.virt_area(base), 7.5 * 105.8, "Base virt_area", "%f", "s f");
         result = close_handle(&(functions.handle));
      }
   }
   functions.Base_destroy(&base);/*
   functions.abstract.interface.Interface_destroy(&base);*/
   if (result != OK)
      close_handle(&(functions.handle));
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   assert_many(base == NULL, "assert failed: ", "s p", "base pointer == ", base);
   return result;
}
#else
static Result_codes test_base_linking(void) {
   Result_codes result = functions.init(&base, -6.0, 0.0);
   if (result == OK) {
      assert_interface(BASE, BASE_CHAR);
      assert_abstract(BASE, BASE_CHAR, -6.0, 0.0, -6.0 * 0.0, ABSTRACT);
      assert_base(BASE, BASE_CHAR, -6.0, 0.0, -6.0 * 0.0, BASE);

      result = getset_double(functions.abstract.virt_set_X, base, 7.5, functions.abstract.X, "Base x");
      if (OK == result)
         result = incorrect_set_double(functions.virt_set_Y, base, INFINITY, functions.abstract.pv_Y, "Base y");
      if (OK == result)
         result = getset_double(functions.virt_set_Y, base, 105.8, functions.abstract.pv_Y, "Base y"); 
      if (OK == result) {
         print_and_assert(functions.abstract.virt_area(base), 7.5 * 105.8, "Base virt_area", "%f", "s f");
      }
   }
   functions.Base_destroy(&base);/*
   functions.abstract.interface.Interface_destroy(&base);*/
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   assert_many(base == NULL, "assert failed: ", "s p", "base pointer == ", base);
   return result;
}
#endif

Result_codes test_base(void) {
   FUNCTION_INFO(__FUNCTION__);
   load_base_functions(); 
   Result_codes result = test_base_linking();
   return result;
}
