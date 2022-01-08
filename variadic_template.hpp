#ifndef VARIADIC_TEMPLATE_HPP
#define VARIADIC_TEMPLATE_HPP

#include <iostream>
#include <string>
#include <limits>
#include <regex>
#include <cmath>

using std::string;
using std::regex;

template <typename T>
class Money {
static_assert((std::numeric_limits<T>::is_integer || std::is_floating_point<T>::value) && "Number required.");
   T amount {};
   
   static const regex & set_regex();
   void calculate(const string & dollars, const long double cents = 0);
   T calculate_dollars(const string & dollars) const;
public:
   Money(const string & dollars, const long double cents);   
   Money(const string & dollars); 
   
   ~Money(); 
   
   T get_amount() const;

   static Money create(const string & dollars, const long double cents);
   static Money create(const string & dollars);
   
   static const regex REGEX;
   static const string TYPE_NAME;
   
   static T round(const long double x) {
      return static_cast<T>(floor(x + 0.5));
   }
};

#include "variadic_template.cpp"

#endif
