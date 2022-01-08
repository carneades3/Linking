#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include <type_traits>

#include "connector.h"

#ifdef __cplusplus
#include "variadic_template.hpp"
#include "demo.hpp"
#include "utility.hpp"
#include "null.hpp"

using demo::Demo;
using std::cerr;

extern template class Money<short>;
extern template class Money<unsigned short>;
extern template class Money<int>;
extern template class Money<unsigned int>;
extern template class Money<long>;
extern template class Money<unsigned long>;
extern template class Money<long long>;
extern template class Money<unsigned long long>;
extern template class Money<float>;
extern template class Money<double>;
extern template class Money<long double>;

template <class T>
inline Result_codes validate_pointers(Money_type * money_ptr, const char * dollars, const T * type_ptr, const char * function_name) {
   if (nullptr == function_name) {
      cerr << "Error function name is nullptr\n";
      return INVALID_ARG;
   }
   if (OK != check_pointer_1_1(money_ptr, function_name, " Error money pointer"))
      return INVALID_ARG;
   if (OK != check_pointer(dollars, function_name, " Error dollars"))
      return INVALID_ARG;
   if (OK != check_pointer(type_ptr, function_name, string(" Error pointer ") + typeid(T).name()))
      return INVALID_ARG;
   return OK;
}

//template <class Type, class Function, typename... Args, std::enable_if_t<std::is_function<Function>::value, bool> = true> 
template <class Type, typename Function, typename... Args>
Result_codes call_function(Money_type * money_ptr, Type * const type_ptr, Function && func, Args &&... args) {
   if (OK != check_pointer_1_1(money_ptr, __func__, " Error money pointer"))
      return INVALID_ARG;
   if (OK != check_pointer(type_ptr, __func__, string(" Error pointer ") + typeid(Type).name()))
      return INVALID_ARG;
   try {
      Money<Type> money = func(std::forward<Args>(args)...);
      *money_ptr = reinterpret_cast<Money<Type> *>(&money);
      *type_ptr = money.get_amount();
   } catch (const invalid_argument& const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      invalid_argument &e = const_cast<invalid_argument &>(const_e);
      return get_error_code(reinterpret_cast<invalid_argument *>(&e));
   } catch (const out_of_range& const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      out_of_range &e = const_cast<out_of_range &>(const_e);
      return get_error_code(reinterpret_cast<out_of_range *>(&e));
   }
   return OK;
} 

template <class Type, class ... Args>
//template <class Type>
Result_codes call(Money_type * money_ptr, Type * const type_ptr, const Money_functions function, Args &&... args) {
   if (OK != check_pointer_1_1(money_ptr, __func__, " Error money pointer"))
      return INVALID_ARG;
   if (OK != check_pointer(type_ptr, __func__, string(" Error pointer ") + typeid(Type).name()))
      return INVALID_ARG;

   Result_codes result = UNRECOGNIZED_ERROR;
   switch(function) {
      case INIT_1: 
         result = call_function<Type>(money_ptr, type_ptr, Template_Constructor<Type, Money>(), std::forward<Args>(args)...);
         break;
      case CREATE_1: 
         result = call_function<Type>(money_ptr, type_ptr, Money_Creation<Type>(), std::forward<Args>(args)...);
         break;
      case INIT_2: 
         result = call_function<Type>(money_ptr, type_ptr, Template_Constructor<Type, Money>(), std::forward<Args>(args)...);
         break;
      case CREATE_2: 
         result = call_function<Type>(money_ptr, type_ptr, Money_Creation<Type>(), std::forward<Args>(args)...);
         break;
      default:
         result = INVALID_ARG;
         cerr << "Unrecognized function type = " << function << '\n';
   }
   return result;
} 

template <typename... Args>
Result_codes call_1(Money_type * money_ptr, const Number type, union Number_pointer_union * const n_union,
                  const Money_functions function, Args &&... args) {
   if (OK != check_pointer_1_1(money_ptr, __func__, " Error money pointer"))
      return INVALID_ARG;
   if (OK != check_pointer(n_union, __func__, " Error union pointer "))
      return INVALID_ARG;
   
   Result_codes result = UNRECOGNIZED_ERROR;
   switch(type) {
      case SHORT: 
         result = call<short, Args...>(money_ptr,  &(n_union->s), function, std::forward<Args>(args)...);
         break;
      case U_SHORT: 
         result = call<unsigned short>(money_ptr, &(n_union->us), function, std::forward<Args>(args)...);
         break;
      case INT: 
         result = call<int>(money_ptr, &(n_union->i), function, std::forward<Args>(args)...);
         break;
      case U_INT: 
         result = call<unsigned int>(money_ptr, &(n_union->ui), function, std::forward<Args>(args)...);
         break;
      case LONG: 
         result = call<long>(money_ptr, &(n_union->l), function, std::forward<Args>(args)...);
         break;
      case U_LONG: 
         result = call<unsigned long>(money_ptr, &(n_union->ul), function, std::forward<Args>(args)...);
         break;
      case LONG_LONG: 
         result = call<long long>(money_ptr, &(n_union->ll), function, std::forward<Args>(args)...);
         break;
      case U_LONG_LONG: 
         result = call<unsigned long long>(money_ptr, &(n_union->ull), function, std::forward<Args>(args)...);
         break;
      case FLOAT: 
         result = call<float>(money_ptr, &(n_union->f), function, std::forward<Args>(args)...);
         break;
      case DOUBLE: 
         result = call<double>(money_ptr, &(n_union->d), function, std::forward<Args>(args)...);
         break;
      case LONG_DOUBLE: 
         result = call<long double>(money_ptr, &(n_union->ld), function, std::forward<Args>(args)...);
         break;
      default:
         result = INVALID_ARG;
         cerr << "Unrecognized type = " << type << '\n';
   }
   return result;
} 

extern "C" {
#endif
void set_handler(void (*func)(void)) { 
   std::set_terminate(func); 
}

//#ifdef MANUAL_DLL_LOAD
Result_codes Money_type__function(Money_type * money_ptr, const Money_functions function, const Number type, 
union Number_pointer_union * const n_union, const char * dollars, ... ) {
   if (OK != check_pointer(dollars, __func__, " Error dollars"))
      return INVALID_ARG;
   if (OK != check_pointer_1_1(money_ptr, __func__, " Error money pointer"))
      return INVALID_ARG;
   if (OK != check_pointer(n_union, __func__, " Error n_union"))
      return INVALID_ARG;
   
   va_list arg_list;
   va_start( arg_list, dollars );
   Result_codes result = UNRECOGNIZED_ERROR;
   switch(function) {
      case INIT_1: 
         result = call_1(money_ptr, type, n_union, function, dollars);
         break;
      case CREATE_1: 
         result = call_1(money_ptr, type, n_union, function, dollars);
         break;
      case INIT_2: 
         result = call_1(money_ptr, type, n_union, function, dollars, va_arg(arg_list, long double));
         break;
      case CREATE_2:
         result = call_1(money_ptr, type, n_union, function, dollars, va_arg(arg_list, long double));
         break;
      default:
         result = INVALID_ARG;
         cerr << "Unrecognized function type = " << function << '\n';
   }
   va_end(arg_list);
   return result;
}

#ifdef __cplusplus
static Demo * demo_instance = nullptr;
}
#endif

Result_codes demo_set_name(Demo * const object, const char * name) {
   if (OK != check_pointer(object, __func__, " Error demo_instance")
      ||   OK != check_pointer(name, __func__, " Error name")) 
      return INVALID_ARG;
   try {
      object->set_name(name);
   } catch (exception & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return get_error_code(reinterpret_cast<exception *>(&e));
   } catch (...) {
      cerr  << __func__ << " Unrecognized exception was catched " << '\n';
      return UNRECOGNIZED_ERROR;
   }
   return OK;
}

Result_codes demo_get_name(const Demo * const object, char ** name) {
   if (OK != check_pointer_1_1(name, __func__, " Error name")
      || OK != check_pointer(object, __func__, " Error demo_instance"))
      return INVALID_ARG;
   try {
      *name = object->get_name_cstring();
      //string name_string = ::demo_instance->get_name();
      //*name = name_string.data();
      //*name = reinterpret_cast<char *>(name_string);
      //*name = const_cast<char *>(cstring);
      //char * result = const_cast<char *>(name.c_str());
   } catch (exception & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return get_error_code(reinterpret_cast<exception *>(&e));
   } catch (...) {
      cerr  << __func__ << " Unrecognized exception was catched " << '\n';
      return UNRECOGNIZED_ERROR;
   }
   return OK;
}

#ifdef __cplusplus
extern "C" {
#endif

Result_codes demo_init(const char * name) {
   if (OK != check_pointer(name, __func__, " Error name")) 
      return INVALID_ARG;
   //int * y = nullptr;
   //init<int>(y, Constructor<int>(), 8);
   //cerr  << __func__ << " *y = " << *y << '\n';
   //assert(*y == 8);
   //Result_codes result = init<int>(y, Constructor<int>(), 8);
   Result_codes result = init<Demo>(::demo_instance, Constructor<Demo>(), name);
   return result;
}

Result_codes demo_set_name(const char * name) {
   if (OK != check_pointer(name, __func__, " Error name")) 
      return INVALID_ARG;
   Result_codes result = demo_set_name(::demo_instance, name);
   return result;
}

Result_codes demo_get_name(char ** name) {
   if (OK != check_pointer_1_1(name, __func__, " Error name"))
      return INVALID_ARG;
   Result_codes result = demo_get_name(::demo_instance, name);
   return result;
}

Result_codes demo_destroy(void) {
   Result_codes result = Destructor<Demo>()(::demo_instance);
   //Demo* j = nullptr;
   //Result_codes result = Destructor<Demo>()(j);
   return result;
}

#ifdef __cplusplus
}
#endif
