#include "connector.h"
#include "demo_derived_connector.h"
#include "demo_derived.hpp"
#include "utility.hpp"
#include "null.hpp"

using namespace demo;
using namespace std;

extern Result_codes demo_set_name(Demo * const object, const char * name);
extern Result_codes demo_get_name(const Demo * const object, char ** name);

//static Demo_derived * instance = nullptr;
Demo_derived * Demo_derived_connector::instance = nullptr;

Result_codes Demo_derived_connector::init(const char * const name, const unsigned int age) {
   if (OK != check_pointer(name, __func__, " Error name")) 
      return INVALID_ARG;
   Result_codes result = ::init<Demo_derived>(instance, Constructor<Demo_derived>(), name, age);
   return result;
}

Result_codes Demo_derived_connector::set_name(const char * name) {
   if (OK != check_pointer(name, __func__, " Error name")) 
      return INVALID_ARG;
   Result_codes result = demo_set_name(instance, name);
   return result;
}

Result_codes Demo_derived_connector::get_name(char ** const name) {
   if (OK != check_pointer_1_1(name, __func__, " Error name"))
      return INVALID_ARG;
   Result_codes result = demo_get_name(instance, name);
   return result;
}

Result_codes Demo_derived_connector::set_age(const unsigned int age) {
   if (instance == nullptr) {
      cerr  << __func__ << " Error instance = " << instance << '\n';
      return INVALID_ARG;
   }
   try {
      instance->set_age(age);
   } catch (exception & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return get_error_code(reinterpret_cast<exception *>(&e));
   } catch (...) {
      cerr  << __func__ << " Unrecognized exception was catched " << '\n';
      return UNRECOGNIZED_ERROR;
   }
   return OK;
}

Result_codes Demo_derived_connector::get_age(unsigned int * const age) {
   if (OK != check_pointer(age, __func__, " Error age"))
      return INVALID_ARG;
   if (instance == nullptr) {
      cerr  << __func__ << " Error instance = " << instance << '\n';
      return BAD_FUNTION_CALL;
   }
   try {
      *age = instance->get_age();
   } catch (exception & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return get_error_code(reinterpret_cast<exception *>(&e));
   } catch (...) {
      cerr  << __func__ << " Unrecognized exception was catched " << '\n';
      return UNRECOGNIZED_ERROR;
   }
   return OK;
}

Result_codes Demo_derived_connector::destruct() {
   Result_codes result = Destructor<Demo_derived>()(instance);
   return result;
}




Result_codes demo_derived_init(const char * const name, const unsigned int age) {
   Result_codes result = Demo_derived_connector::init(name, age);
   return result;
}

Result_codes demo_derived_set_name(const char * name) {
   Result_codes result = Demo_derived_connector::set_name(name);
   return result;
}

Result_codes demo_derived_get_name(char ** name) {
   Result_codes result = Demo_derived_connector::get_name(name);
   return result;
}

Result_codes demo_derived_set_age(const unsigned int age) {
   Result_codes result = Demo_derived_connector::set_age(age);
   return result;
}

Result_codes demo_derived_get_age(unsigned int * const age) {
   Result_codes result = Demo_derived_connector::get_age(age);
   return result;
}

Result_codes demo_derived_destroy(void) {
   Result_codes result = Demo_derived_connector::destruct();
   return result;
}
