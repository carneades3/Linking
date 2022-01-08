#include "variadic_template.hpp"

#include <limits>

using std::cerr;
using std::numeric_limits;
using std::is_floating_point;
using std::out_of_range;

#define LOG_FUNC(x) cerr << '\n' << #x " = " << x << '\t'
#define LOG_ARG(x) cerr << #x " = " << x << '\t'

template <typename T>
const string Money<T>::TYPE_NAME = typeid(T).name();

template <typename T>
const regex Money<T>::REGEX       = Money<T>::set_regex() ;

template<typename Smaller, typename Greater>
inline bool is_overflow(const Greater & x) {
   static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) && "Number required.");
   return x < numeric_limits<Smaller>::lowest() || x > numeric_limits<Smaller>::max();
}

template <typename Smaller, typename Greater, class Function, class... Args, std::enable_if_t<std::is_function<Function>::value, bool> = true> 
Smaller calculate_range(Function && func, Args &&... args) {
   Greater result = func(std::forward<Args>(args)...);
   if (is_overflow<Smaller, Greater>(result))
      throw out_of_range("amount = " + std::to_string(result) + " is overflow for type " + typeid(Smaller).name());
   return static_cast<Smaller>(result);
}

template <typename T>
T Money<T>::calculate_dollars(const string & dollars) const {
   if (is_floating_point<T>::value)
      return calculate_range<T, long double, long double(const string& , size_t*)>(std::stold, dollars, nullptr);
   else if (numeric_limits<T>::is_integer) {
      if (numeric_limits<T>::is_signed)
         return calculate_range<T, long long, long long(const string& , size_t*, int)>(std::stoll, dollars, nullptr, 10);
      else if (std::is_unsigned<T>::value)
         return calculate_range<T, unsigned long long, unsigned long long(const string& , size_t*, int)>(std::stoull, dollars, nullptr, 10);
   }
   throw std::invalid_argument(typeid(T).name() + string("is not supported"));
}

template <typename T>
void Money<T>::calculate(const string & dollars, const long double cents /* = 0 */ ) {
   if (! regex_match(dollars, REGEX)) 
      throw std::invalid_argument(" argument of dollars: '" + dollars + "' is not matches by regex");
   this->amount = 100 * calculate_dollars(dollars) + cents;
}

template <typename T>
Money<T>::Money(const string & dollars, const long double cents) {
   cerr << "\n C++ " << __cplusplus << " type = " << TYPE_NAME << '\n';
   LOG_FUNC(__func__);
   LOG_ARG(dollars);
   LOG_ARG(cents);
   calculate(dollars, cents);
   LOG_ARG(this->amount);
}

template <typename T>
Money<T>::Money(const string & dollars) {
   cerr << "\n C++ " << __cplusplus << " type = " << TYPE_NAME << '\n';
   LOG_FUNC(__func__);
   LOG_ARG(dollars);
   calculate(dollars);
   LOG_ARG(this->amount);
}

template <typename T>
Money<T> Money<T>::create(const string & dollars) {   
   cerr << "\n C++ " << __cplusplus << " type = " << TYPE_NAME << '\n';
   LOG_FUNC(__func__);
   LOG_ARG(dollars);
   return Money<T>(dollars);
}

template <typename T>
Money<T> Money<T>::create(const string & dollars, const long double cents) {
   cerr << "\n C++ " << __cplusplus << " type = " << TYPE_NAME << '\n';
   LOG_FUNC(__func__);
   LOG_ARG(dollars);
   LOG_ARG(cents);
   return Money<T>(dollars, cents);
}

template <typename T>
Money<T>::~Money() {
   cerr << "\n C++ " << __cplusplus << " type = " << TYPE_NAME << '\n';
   LOG_FUNC(__func__);
}

template <typename T>
T Money<T>::get_amount() const { 
   cerr << "\n C++ " << __cplusplus << " type = " << TYPE_NAME << '\n';
   LOG_FUNC(__func__);
   return amount;
}

union Regex_string_union {
   const char * float_regex_string  = R"(^[+-]?(\d+([.]\d*)?([eE][+-]?\d+)?|[.]\d+([eE][+-]?\d+)?)$)";
   const char * int_regex_string;
};

template <typename Number, std::enable_if_t<is_floating_point<Number>::value, bool> = true>
const char * get_regex_string(Number &&) {
   Regex_string_union u;
   //u.float_regex_string = R"(^[+-]?(\d+([.]\d*)?([eE][+-]?\d+)?|[.]\d+([eE][+-]?\d+)?)$)";
   return u.float_regex_string;
}

template <typename Number, std::enable_if_t<numeric_limits<Number>::is_integer, bool> = true>
const char * get_regex_string(Number &&) {
   Regex_string_union u;
   u.int_regex_string = R"(^[+-]?(\d+)$)";
   return u.int_regex_string;
}

template <typename T>
const regex & Money<T>::set_regex() {
   try {
      static regex REGEX       = regex { get_regex_string(static_cast<T>(0)), std::regex::ECMAScript } ;
      return REGEX;
   } catch (const std::regex_error & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      throw e;
   }
}
