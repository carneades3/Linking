#ifndef NULL_HPP
#define NULL_HPP

#include <string>
#include <iostream>
#include <type_traits>

#include "result_codes.h"

using std::string;
using std::cerr;

template <class Pointer, std::enable_if_t<std::is_pointer<Pointer>::value
                                          || std::is_array<Pointer>::value, bool> = true> 
inline Result_codes check_pointer(Pointer pointer, const string & function, const string & message) {
   if (nullptr == pointer) {
      cerr  << function << " : " << message <<  " = " << reinterpret_cast<unsigned long long>(pointer) << '\n'; 
      return INVALID_ARG;
   }
   return OK;
}

template <class T> 
inline Result_codes check_pointer_2(T **pointer, const string & function, const string & message) {
   if (OK == check_pointer(pointer, function, message))
      return check_pointer(*pointer, function, message + string(1, '*'));
   return INVALID_ARG;
}

template <class T> 
inline Result_codes check_pointer_1_1(T **pointer, const string & function, const string & message) {
   if (OK == check_pointer(pointer, function, message)) {
      if (*pointer == nullptr)
         return OK;
      else
         cerr  << function << " : " << message + string(1, '*') <<  " = " << *pointer << '\n';
   }
   return INVALID_ARG;
}

inline Result_codes check_pointer(std::nullptr_t) {
   cerr  << "type: nullptr_t - pointer is null" << '\n';  
   return INVALID_ARG;
}
/*
template <typename Function, typename... Args>  
void construct_incorrect(Function && f, Args&&... args ) { 
   try {
      f(std::forward<Args>(args)...);
      assert(0);
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}
*/
#endif
