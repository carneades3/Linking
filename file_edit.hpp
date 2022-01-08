#ifndef FILE_EDIT_HPP
#define FILE_EDIT_HPP

#include <sstream>
#include <string>

using std::string;
using std::stringstream;

bool exist_file(const char *file_name);

class File_edit {
private:
   string filename;
#ifdef MANUAL_DLL_LOAD
   static const size_t manual_dll_size;

   void delete_manual_dll_load();
#else
   void insert_manual_dll_load();
   
   static const string cppflags;
   static const string cflags; 
   static const size_t cppflags_size;
   static const size_t cflags_size; 
#endif
public:
   static const string comment;
   static const string manual_dll; 
   
   File_edit(const string & name);
   stringstream get_content_file();
   void set_filename(const string & name);
   string get_filename() const { return filename; }
   
   void edit_makefile() {
#ifdef MANUAL_DLL_LOAD
      delete_manual_dll_load();
#else
      insert_manual_dll_load();
#endif
   }
   
};


#endif
