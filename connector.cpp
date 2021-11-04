#include <cstdlib>

#include "variadic_template.hpp"
#include "demo.hpp"
#include "connector.h"

#ifdef __cplusplus
extern "C" {
#endif

Money_int Money_int__init_1(const char * dollars) {
   Money<int> money = Money<int>(dollars);
   void* result = reinterpret_cast<void*>(& money);
   return result;
}

Money_int Money_int__create_1(const char * dollars) {
   Money<int> money =  Money<int>::create(dollars);
   void* result = reinterpret_cast<void*>(& money);
   return result;
}

Money_int Money_int__init_2(const char * dollars, const long double cents) {
   Money<int> money =  Money<int>(dollars, cents);
   void* result = reinterpret_cast<void*>(& money);
   return result;
}

Money_int Money_int__create_2(const char * dollars, const long double cents) {
   Money<int> money =  Money<int>::create(dollars, cents);
   void* result = reinterpret_cast<void*>(& money);
   return result;
}

static Demo * demo_instance = NULL;

void demo_init(const char * name) {
   if (demo_instance == NULL)
      demo_instance = new Demo(name);
}

void demo_set_name(const char * name) {
   if (demo_instance == NULL)
      exit(EXIT_FAILURE);
   demo_instance->set_name(name);
}

const char * demo_get_name() {
   if (demo_instance == NULL)
      exit(EXIT_FAILURE);
   return demo_instance->get_name();
}

void demo_destroy() {
   if (demo_instance)
      delete demo_instance;
}

#ifdef __cplusplus
}
#endif
