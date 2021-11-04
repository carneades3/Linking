#include "print.hpp"
#include "variadic_template.hpp"
#include "demo.hpp"

template <typename Function, typename... Args> 
inline void call(Function && f, Args &&... args ) { 
   f(std::forward<Args>(args)...);
}

static Demo human = {"Leibniz"};

int main() {
   call(Constructor<int, Money>(), "-20.8", 5.0L);
   call(Constructor<long long, Money>(), "78");
   call(Creation<unsigned long long>(), "2", 5);
   call(Creation<long double>(), "7");
   cerr << '\n';

   cerr << TIE( unmove(__cplusplus), __func__, unmove(human.get_name())) << '\n';
   human.set_name("Spinoza");
   cerr << TIE( unmove(__cplusplus), __func__, unmove(human.get_name())) << '\n';
}

// g++ -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 
