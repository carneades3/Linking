#ifndef PRINT_HPP
#define PRINT_HPP

#include <iostream>
#include <tuple>

using std::enable_if;
using std::ostream;
using std::get;
using std::tuple;
using std::cerr;

#include "utility.hpp"

#define TIE(...) (std::tie(__VA_ARGS__))

#define assert_many(EX,...) \
  ((EX) || (assert_tuples (std::tie(__VA_ARGS__), #EX, __FILE__, __LINE__, __DATE__, __TIME__)))
  
#define print_and_assert(value, expected_value, value_string) \
   cerr << TIE( "C++", unmove(__cplusplus), __func__, unmove(string(value_string) + " =="), unmove(value)) << '\n'; \
   assert_many((value) == (expected_value), unmove(string(value_string) + " == "), unmove(value))

template<const unsigned int N>
struct print_tuple {
   template<typename... T>
   static typename enable_if<(N < sizeof...(T))>::type
   print(ostream& os, const tuple<T...>& t) {
      os << " " << get<N>(t);
      print_tuple<N + 1>::print(os, t);
   }
   
   template<typename... T>
   static typename enable_if<(N >= sizeof...(T))>::type
   print(ostream& , const tuple<T...>&) { }    // for empty tuple
};

inline ostream& operator<< (ostream & os, const tuple<> &) {
   return os << "";
}

template<typename S, typename... T>
inline ostream& operator<< (ostream & os, const tuple<S, T...> & t) {
   os << get<0>(t);
   print_tuple<1>::print(os, t);
   return os;
}

template <typename ...Args>
bool assert_tuples(tuple<Args...> info, const char *msg, const char *file, int line,
                   const char * date, const char * time) {
   cerr << "\nAssertion failed:\n"
            << "Expression:\t" << msg << "\n"
            << "Source file:\t\t" << file << ", line " << line << "\n"
            << "Date:\t\t"   << date  << "\n"
            << "Time:\t\t"   << time  << "\n"
            << "  Info: " << info << '\n' ;
   abort();
   //assert(false);
   //return false;
}
/*
template<typename T>
inline void (const T& value, const T& expected_value, const string& value_string, const string& function) {
   cerr << TIE( "C++", unmove(__cplusplus), function, value) << '\n';
   assert_many(value == expected_value, unmove(value_string + " == "), value);
}
*/
template <typename Object, typename Value, typename Func_1, typename Func_2, typename... Args>  
Result_codes bind_execute_member_function_assert(Object & object, Func_1 && m_funct, 
                                                        const Value & expected_value, const string& value_string, const string& function,
                                                        Func_2 && m_funct_args, Args&&... args ) {
   Result_codes result = bind_execute_member_function(object, m_funct_args, args ...);
   if (OK != result)
      return result;
   const auto bind_function = std::mem_fn(m_funct);
   const Value value = bind_function(object);
   cerr << " Function: " << function << '\n';
   print_and_assert(value, expected_value, value_string);
   return result;
}

template <typename Object, typename Cast_1, typename Cast_2, typename Value, typename Func_1, typename Func_2, typename... Args>  
Result_codes bind_execute_function_assert(Object & object, Func_1 && get, const Value & expected_value, 
                                          const string& value_string, const string& function,
                                                        Func_2 && set, Args&&... args) {
   if (nullptr == object || nullptr == get || nullptr == set) {
      map<string, unsigned long long> m { {"object", address(object)}, {"get", address(get)}, {"set", address(set)}, };
      cerr << __func__ << " nullptr detected:\n";
      print_address(m);
      return INVALID_ARG;
   }
   Result_codes result = set(reinterpret_cast<Cast_1> (object), args ...);
   if (OK != result)
      return result;
   const Value value = get(reinterpret_cast<Cast_2> (object));
   cerr << " Function: " << function << '\n';
   print_and_assert(value, expected_value, value_string);
   return result;
}

#endif
