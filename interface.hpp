#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <iostream>
#include "print.hpp"

using std::cerr;

namespace Hierarchy {

class Interface {
public:
   static const int NB;
   static const string class_name;
   
   virtual int pv_number() const = 0;
   virtual char pv_char() const = 0;
   virtual void pv_print_number() const = 0;
   virtual void pv_print_char() const = 0;
   virtual ~Interface() { 
      cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, Interface::NB) << '\n';
   }
   
   Interface(Interface &&) noexcept {
      cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, "move constructor", Interface::NB) << '\n';
   }
   Interface& operator=(Interface &&) noexcept {
      cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, "move operator=", Interface::NB) << '\n';
      return *this;
   }
   
protected:
   virtual bool pv_valid(const double) const = 0;
   Interface() { 
      cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, "Interface() constructor", Interface::NB) << '\n';
   }
   Interface(Interface const &) { 
      cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, "copy constructor", Interface::NB) << '\n';
   }
   Interface& operator=(Interface const &) { 
      cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, "copy operator=", Interface::NB) << '\n';
      return *this;
   }
   
};

}


#endif
