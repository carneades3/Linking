#ifndef BASE_HPP
#define BASE_HPP

#include "abstract.hpp"

namespace Hierarchy {

class Base : public Abstract {
private:
   double y { static_cast<double>(BASE) };
public:
   static const int BASE;
   static const char BASE_CHAR;
   static const string class_name;
   
   virtual double pv_Y() const override final { return y; };
   virtual void virt_set_Y (const double );
   virtual void pv_print_Y() const override;
   
   virtual void virt_print_number() const;
   
   //virtual double virt_area() const;
   virtual double virt_area() const override;

   int number() const { return BASE; };
   void print_number() const;
   
   int pv_number() const override { return BASE; };
   
   virtual char pv_char() const { return BASE_CHAR; };
   virtual void pv_print_char() const;
   
   virtual ~Base();
   Base(const double, const double);
   Base() { cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n'; }
   
   Base(const Base & object);
   Base& operator=(const Base & object);
   Base(Base &&) noexcept;
   Base& operator=(Base &&) noexcept;
protected:
   void check(const double n, const string & func);
   inline virtual bool pv_valid(const double n) const override { 
      cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n';
      return (n < 0) ? false : true; 
   }
};

}


#endif
