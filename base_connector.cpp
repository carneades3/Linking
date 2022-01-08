#include "base_connector.h"
#include "utility.hpp"
#include "null.hpp"

using namespace Hierarchy;
using namespace std;

template<>
Base * Base_connector::Abstract_connector<Base>::instance = nullptr;

template<>
const int Base_connector::Abstract_connector<Base>::NUMBER = Base::BASE;

template<>
const char Base_connector::Abstract_connector<Base>::CHAR = Base::BASE_CHAR;

const Static_number_char BASE_STATICS = { .number = Base_connector::Abstract_connector<Base>::NUMBER,
                                          .ch = Base_connector::Abstract_connector<Base>::CHAR
};

static Base_connector connector;

Result_codes base_cpp_init(const double x, const double y) {
   Result_codes result = connector.init(x, y);
   return result;
}

Result_codes base_cpp_pv_number(int * const n) {
   Result_codes result = (&connector)->pv_number(n);
   return result;
}

Result_codes base_cpp_pv_char(char * const c) {
   Result_codes result = connector.pv_char(c);
   return result;
}

Result_codes base_cpp_X(double  * const  x) {
   Result_codes result = connector.X(x);
   return result;
}

Result_codes base_cpp_virt_set_X(const double x) {
   Result_codes result = connector.virt_set_X(x);
   return result;
}

Result_codes base_cpp_pv_Y(double * const y) {
   Result_codes result = connector.pv_Y(y);
   return result;
}


Result_codes base_cpp_virt_set_Y(const double y) {
   Result_codes result = connector.virt_set_Y(y);
   return result;
}

Result_codes base_cpp_virt_area(double * const area) {
   Result_codes result = connector.virt_area(area);
   return result;
}

Result_codes base_cpp_number(int * const number) {
   Result_codes result = connector.number(number);
   return result;
}

Result_codes base_cpp_destroy(void) {
   Result_codes result = connector.destruct();
   return result;
}
