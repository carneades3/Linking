#include "derived_connector.h"
#include "utility.hpp"
#include "null.hpp"

using namespace Hierarchy;
using namespace std;

template<>
Derived * Derived_connector::Abstract_connector<Derived>::instance = nullptr;

template<>
const int Derived_connector::Abstract_connector<Derived>::NUMBER = Derived::DERIVED;

template<>
const char Derived_connector::Abstract_connector<Derived>::CHAR = Derived::DERIVED_CHAR;

const Static_number_char DERIVED_STATICS = { .number = Derived_connector::Abstract_connector<Derived>::NUMBER,
                                          .ch = Derived_connector::Abstract_connector<Derived>::CHAR
};

static Derived_connector connector;

Result_codes Derived_connector::Z(double * const z) {
   if (OK != check_pointer(z, __func__, " Error number pointer")) 
      return INVALID_ARG;
   *z = instance->Z();
   return OK;
}

Result_codes Derived_connector::virt_set_Z(const double z) {
   Result_codes result = bind_execute_member_function(instance, &Derived::virt_set_Z, z);
   return result;
}

Result_codes derived_cpp_init(const double x, const double y, const double z) {
   Result_codes result = connector.init(x, y, z);
   return result;
}

Result_codes derived_cpp_pv_number(int * const n) {
   Result_codes result = (&connector)->pv_number(n);
   return result;
}

Result_codes derived_cpp_pv_char(char * const c) {
   Result_codes result = connector.pv_char(c);
   return result;
}

Result_codes derived_cpp_X(double  * const  x) {
   Result_codes result = connector.X(x);
   return result;
}

Result_codes derived_cpp_virt_set_X(const double x) {
   Result_codes result = connector.virt_set_X(x);
   return result;
}

Result_codes derived_cpp_pv_Y(double * const y) {
   Result_codes result = connector.pv_Y(y);
   return result;
}


Result_codes derived_cpp_virt_set_Y(const double y) {
   Result_codes result = connector.virt_set_Y(y);
   return result;
}

Result_codes derived_cpp_Z(double  * const  z) {
   Result_codes result = connector.Z(z);
   return result;
}

Result_codes derived_cpp_virt_set_Z(const double z) {
   Result_codes result = connector.virt_set_Z(z);
   return result;
}

Result_codes derived_cpp_virt_area(double * const area) {
   Result_codes result = connector.virt_area(area);
   return result;
}

Result_codes derived_cpp_number(int * const number) {
   Result_codes result = connector.number(number);
   return result;
}

Result_codes derived_cpp_destroy(void) {
   Result_codes result = connector.destruct();
   return result;
}
