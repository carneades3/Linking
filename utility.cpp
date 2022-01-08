#include "utility.hpp"

Result_codes get_error_code(exception * e) {
   if (dynamic_cast<bad_alloc*> (e) != nullptr)
      return BAD_ALLOC;
   if (dynamic_cast<invalid_argument*> (e) != nullptr)
      return INVALID_ARG;
   if (dynamic_cast<bad_cast*> (e) != nullptr)
      return BAD_CAST;
   if (dynamic_cast<regex_error*> (e) != nullptr)
      return REGEX_ERROR;
   if (dynamic_cast<out_of_range*> (e) != nullptr)
      return OUT_OF_RANGE_ERROR;
   if (dynamic_cast<std::runtime_error*> (e) != nullptr)
      return RUNTIME_ERROR;
   if (dynamic_cast<exception*> (e) != nullptr)
      return STD_ERROR;
   return UNRECOGNIZED_ERROR;
}

void print_address(const map<string, unsigned long long> & addresses) {
   const size_t SIZE = addresses.size();
   if (SIZE > 0) {
      cerr << " Addresses of objects printed in " << SIZE << " rows below:\n";
      for (const pair<string, unsigned long long> &record : addresses) {
         cerr << "{" << record.first << " = " << record.second << "}" << '\n';
      }
      cerr << "End of addresses printing.\n";
   }
}
