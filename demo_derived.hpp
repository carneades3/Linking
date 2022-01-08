#ifndef DEMO_DERIVED_HPP
#define DEMO_DERIVED_HPP

#include <string>

#include "demo.hpp"

using std::string;

namespace demo {

class Demo_derived : public Demo {
   unsigned int age;

public:
   Demo_derived(const string & name, const unsigned int year);
   unsigned int  get_age() const;
   void set_age(const unsigned int year);
   ~Demo_derived();
};

}

#endif
