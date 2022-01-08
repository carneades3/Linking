#ifndef DEMO_HPP
#define DEMO_HPP

#include <string>
#include <regex>

using std::string;
using std::regex;

namespace demo {

class Demo {
   string name;
   
   static const regex & set_regex();
public:
   Demo(const string & name);
   string get_name() const;
   char * get_name_cstring() const;
   void set_name(const string & name);
   ~Demo();
      
   static const regex NAME_REGEX;
};

}

#endif
