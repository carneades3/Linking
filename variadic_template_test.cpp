#include "variadic_template_test.hpp"
#include "utility.hpp"
//#include "variadic_template.hpp"
#include <functional>

using std::bind;
using std::cref;

namespace tests {
   
using namespace std::placeholders;
   
Result_codes test_money() {
   Result_codes result = incorrect_call(Template_Constructor<int, Money>(), "-20i", -80.0L);
   if (OK == result)
      result = incorrect_call(Template_Constructor<short, Money>(), "200000", -80.0L);
   if (OK == result)
      result = call_catch_exception(Template_Constructor<int, Money>(), "-20", -80.0L);
   if (OK == result)
      call_catch_exception(Template_Constructor<long long, Money>(), "78");
   if (OK == result) {
      call_catch_exception(Money_Creation<unsigned long long>(), "2", 5);/*
      auto set = bind((Money<unsigned long long>(*)(const string &, const long double)) &Money<unsigned long long>::create,
                      cref(unmove(string("2"))), _1); 
      call_catch_exception(set, string("2"), 5);*/
   }
   if (OK == result) {/*
      auto set = bind((Money<long double>(*)(const string &)) &Money<long double>::create,
                      cref("7"), _1);  */
      call_catch_exception(Money_Creation<long double>(), "7");/*
      call_catch_exception(set, "7");*/
   }
   return result;
}

}
