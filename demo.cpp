#include "demo.hpp"
#include "print.hpp"
#include "utility.hpp"
#include "null.hpp"

using std::cerr;

namespace demo {
   
const regex Demo::NAME_REGEX       = Demo::set_regex() ;
/*
static inline void validate(const char * name, const string & function, const string & message) {
   if (OK != check_pointer(name, function, message)) 
      throw std::invalid_argument(function + " argument of name: '" + name + "' is nullptr");
   if (! regex_match(name, Demo::NAME_REGEX)) 
      throw std::invalid_argument(function + " argument of name: '" + name + "' is not matches by regex");
}
*/
static inline void validate(const string & name, const string & function) {
   if (! regex_match(name, Demo::NAME_REGEX)) 
      throw std::invalid_argument(function + " argument of name: '" + name + "' is not matches by regex");
}

Demo::Demo(const string & name) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, name) << '\n';
   validate(name, __func__);
   this->name = name;
}

void Demo::set_name(const string & name) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, name) << '\n';
   validate(name, __func__);
   this->name = name;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->name) << '\n';
}

string Demo::get_name() const {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__) << '\n';
   return name;
   //char * result = const_cast<char *>(name.c_str());
   //return result;
}

char * Demo::get_name_cstring() const {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__) << '\n';
   char * result = const_cast<char *>(name.c_str());
   return result;
}

Demo::~Demo() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->name) << '\n';
}

const regex & Demo::set_regex() {
   try {
      static regex NAME_REGEX       = regex { R"(^[[:upper:]][[:lower:]]*( [[:upper:]][[:lower:]]*)?$)", std::regex::extended } ;
      return NAME_REGEX;
   } catch (const std::regex_error & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      throw e;
   }
}

}

//

/*  only to use in C++ code in manual dll (shared object) loading  */
extern "C" demo::Demo * demo_create(const string & name) {
   //if (name == nullptr) 
   //   throw std::invalid_argument(string(__func__) + " argument of name is nullptr");
   using namespace demo;
   try {
      Demo * result = new Demo(name);
      return result;
   }
   catch (const std::invalid_argument & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return nullptr;
   }
}

/*  only to use in C++ code in manual dll (shared object) loading  */
extern "C" void demo_destroy(demo::Demo * & pointer) {
   if (pointer == nullptr) 
      throw std::invalid_argument(string(__func__) + " argument of demo is nullptr");
   
   delete pointer;
   pointer = nullptr;
}
