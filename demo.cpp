#include "demo.hpp"
#include "print.hpp"

using std::cerr;

Demo::Demo(const char * NAME) : name(NAME) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, name) << '\n';
}

void Demo::set_name(const char *name) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, name) << '\n';
   this->name = name;
}

const char * Demo::get_name() const {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__) << '\n';
   const char * result = name.data();
   return result;
}

Demo::~Demo() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->name) << '\n';
}
