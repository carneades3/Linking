#ifndef ABSTRACT_HPP
#define ABSTRACT_HPP

#include "interface.hpp"

#include <string>

using std::string;
using std::cerr;

namespace Hierarchy {

class Abstract : public Interface {
private:
   double x {ABSTRACT};
public:
   static const int ABSTRACT = 0;
   static const string class_name;
   //static string get_class_name() { return class_name; }
   
   virtual double pv_Y() const = 0;
   virtual void pv_print_Y() const = 0;
   
   static void validate(const double, const string &);
   
   virtual double X() const final { return x; };
   virtual void virt_set_X (const double x);
   virtual void virt_print_X() const final;
   
   virtual double virt_area() const;
   //virtual void print_virt_area() const ;
   virtual void print_virt_area() const final;

   int number() const { return ABSTRACT; };
   void print_number() const;
   /*
   int number() { return ABSTRACT; };
   void print_number();*/
   
   int pv_number() const override { return ABSTRACT; };
   void pv_print_number() const override final;
   
   virtual ~Abstract() ;
   Abstract(Abstract &&) noexcept;
   Abstract& operator=(Abstract &&) noexcept;
protected:
   Abstract(const double);
   Abstract() { cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n'; }
   Abstract(Abstract const &);
   Abstract& operator=(Abstract const &);
   
};

}


#endif
