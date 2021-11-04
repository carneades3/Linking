#ifndef VARIADIC_TEMPLATE_H
#define VARIADIC_TEMPLATE_H

#include <iostream>
#include <string>
#include <limits>

using std::string;

template <typename T>
class Money {
static_assert((std::numeric_limits<T>::is_integer || std::is_floating_point<T>::value) && "Number required.");
public:
   Money(const string & dollars, const long double cents);   
   Money(const string & dollars); 
   
   ~Money(); 

   static Money create(const string & dollars, const long double cents);
   static Money create(const string & dollars);
};

template <typename T>
struct Creation {
	template<typename... Args>
	Money<T> operator()(Args...args) const {
		return Money<T>::create(std::forward<Args>(args)...);
	}
};

template <class Type, template<typename> class Template>
struct Constructor {
	template<typename... Args>
	Template<Type> operator()(Args...args) const {
		return Template<Type>(std::forward<Args>(args)...);
	}
};

#include "variadic_template.cpp"

#endif
