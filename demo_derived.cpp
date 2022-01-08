#include "demo_derived.hpp"
#include "print.hpp"
#include "null.hpp"

using std::cerr;
using std::to_string;

namespace demo {

static inline void validate(const unsigned int age, const string & function) {
   if (age <= 0 || age >= 150) 
      throw std::invalid_argument(function + " argument of age: '" + to_string(age) + "' is out of bounds");
}

Demo_derived::Demo_derived(const string & name, const unsigned int age) : Demo(name) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, age) << '\n';
   validate(age, __func__);
   this->age = age;
}

void Demo_derived::set_age(const unsigned int age) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, age) << '\n';
   validate(age, __func__);
   this->age = age;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->age) << '\n';
}

unsigned int Demo_derived::get_age() const {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__) << '\n';
   return age;
}

Demo_derived::~Demo_derived() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->age) << '\n';
}

}

/*  only to use in C++ code in manual dll (shared object) loading  */
extern "C" demo::Demo_derived * demo_derived_create(const string & name, const unsigned int age) {
   using namespace demo;
   try {
      demo::Demo_derived * result = new demo::Demo_derived(name, age);
      return result;
   }
   catch (const std::invalid_argument & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return nullptr;
   }
}

/*  only to use in C++ code in manual dll (shared object) loading  */
extern "C" void demo_derived_destroy(demo::Demo_derived * & pointer) {
   if (pointer == nullptr) 
      throw std::invalid_argument(string(__func__) + " argument of demo is nullptr");
   
   delete pointer;
   pointer = nullptr;
}
