#ifndef DEMO_HPP
#define DEMO_HPP

#include <string>

using std::string;

class Demo {
   string name;
   public:
      Demo(const char *name);
      const char * get_name() const;
      void set_name(const char * name);
      ~Demo();
};

#endif
