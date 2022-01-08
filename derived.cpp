#include "print.hpp"
#include "derived.hpp"

using std::cerr;
using std::to_string;
using std::exchange;
using std::move;

namespace Hierarchy {
   
const string Derived::class_name = typeid(Derived).name();
const int Derived::DERIVED = 2;
const char Derived::DERIVED_CHAR = 'D';

void Derived::check(const double n, const string & func) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n';
   validate(n, func);
   if (! pv_valid(n))
      throw std::invalid_argument(func + " argument of number: '" + to_string(n) + "' is not valid (n > 100)");
}

void Derived::validate(const double number, const string & function) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n';
   if (0 == number) 
      throw std::invalid_argument(function + " argument of number: '" + to_string(number) + "' is 0");
}

void Derived::virt_set_Z(const double n) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n';
   //Base::check(n, __func__);
   check(n, __func__);
   this->z = n;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, this->z) << '\n';
}

void Derived::virt_print_Z() const {
   cerr << TIE( "C++", unmove(__cplusplus), class_name, __func__, " = ", z, unmove('\n'));
}

void Derived::virt_set_Y(const double n) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, unmove(pv_Y())) << '\n';
   check(n, __func__);
   Base::virt_set_Y(n);
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, unmove(pv_Y())) << '\n';
}

void Derived::virt_set_X(const double n) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n';
   check(n, __func__);
   Base::virt_set_X(n);
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, unmove(X())) << '\n';
}

void Derived::pv_print_Y() const {
   double Y = pv_Y();
   cerr << TIE( "C++", unmove(__cplusplus), class_name, __func__, " = ", Y, unmove('\n'));
}

double Derived::virt_area() const {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n';
   return Base::virt_area() * z;
}

void Derived::virt_print_number() const { 
   double n = number();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, class_name, n) << '\n'; 
};

void Derived::print_number() const { 
   double n = number();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, class_name, n) << '\n'; 
};

void Derived::pv_print_char() const { 
   char c = pv_char();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, class_name, c) << '\n'; 
};

Derived::Derived(const double n1, const double n2, const double n3) : Base(n1, n2) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n';
   Base::check(n3, __func__); 
   check(n3, __func__);
   this->z = n3;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, unmove(X()), unmove(pv_Y()), this->z) << '\n';
}

Derived::Derived(const Derived & object) : Base(object) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, "copy constructor") << '\n';
   this->z = object.z;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, unmove(X()), unmove(pv_Y()), this->z) << '\n';
}

Derived& Derived::operator=(const Derived & object) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, "copy operator= ") << '\n';
   if (this == &object)
      return *this;
   static_cast<Base &>(*this) = object;
   //this->Base::operator=(object);
   this->z = object.z;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, unmove(X()), unmove(pv_Y()), this->z) << '\n';
   return *this;
}

Derived::Derived(Derived && object) noexcept : Base(move(object)) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, "move constructor", unmove(X()), unmove(pv_Y()), this->z) << '\n';
   this->z = exchange(object.z, 0);  // explicit move of a member of non-class type 
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, "move constructor", unmove(X()), unmove(pv_Y()), this->z) << '\n';
}

Derived& Derived::operator=(Derived && object) noexcept {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, "move operator= ", unmove(X()), unmove(pv_Y()), this->z) << '\n';
   if(this != &object) {
      static_cast<Base &>(*this) = move(object);
      this->z = exchange(object.z, 0);
   }
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, "move operator= ", unmove(X()), unmove(pv_Y()), this->z) << '\n';
   return *this;
}

Derived::~Derived() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, unmove(X()), unmove(pv_Y()), this->z) << '\n';
}

}

extern "C" Hierarchy::Interface * derived_create(const double n1, const double n2, const double n3) {
   using namespace Hierarchy;
   try {
      Interface * result = new Derived(n1, n2, n3);
      return result;
   }
   catch (const std::invalid_argument & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return nullptr;
   }
}

extern "C" void derived_destroy(Hierarchy::Interface * & pointer) {
   if (pointer == nullptr) 
      throw std::invalid_argument(string(__func__) + " argument of derived is nullptr");
   
   delete pointer;
   pointer = nullptr;
}
