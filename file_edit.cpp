#include <fstream>
#include <iostream>
#include <cerrno>
#include <cstring>
#include "file_edit.hpp"

#include <sys/stat.h>

using std::fstream;
using std::runtime_error;
using std::size_t;
using std::ios_base;
using std::to_string;
using std::stringstream;
using std::cerr;

bool exist_file(const char *name) {
   struct stat   buffer;
   int result = stat (name, &buffer);
   if (-1 == result) {
      cerr << "Stat(): file with name: '" << name << "'. Error: " << strerror(errno) << '\n';
   }
   else if (S_ISREG( buffer.st_mode ) == 0) {
      cerr << "Error: file with name: '" << name << "' is not regular file" << '\n';
      result = -2;
   }
   return result == 0;
}

struct Io_guard {
   fstream& stream;
   const ios_base::iostate old_e;
   Io_guard(fstream& s, ios_base::iostate e) :stream(s), old_e(stream.exceptions()) { 
      stream.exceptions(stream.exceptions() | e); 
   }
   ~Io_guard() { stream.exceptions(old_e); }
   
   void open_file(const string & filename, ios_base::openmode mode = ios_base::in | ios_base::out) {
      try {
         stream.open(filename, mode);
      }
      catch (const ios_base::failure & e) {
         cerr  << __func__ << " " << typeid(e).name() << '\n';
         throw ios_base::failure("Exception opening file. Caught an ios_base::failure.\n" + string(e.what()) +
            "\nError code: " + string(e.code().category().name()) + " : " + to_string((e.code().value())) + "\n");
      }
   }
};

File_edit::File_edit(const string & name) {
   if (! exist_file(name.c_str()))
      throw runtime_error("Init of File_edit failed. Error: " +  string(strerror(errno)));
   filename = name;
}

void File_edit::set_filename(const string & name) {
   if (! exist_file(name.c_str()))
      throw runtime_error("Init of File_edit failed. Error: " +  string(strerror(errno)));
   filename = name;
}

stringstream File_edit::get_content_file() {
   fstream in_file;
   Io_guard in_guard(in_file, ios_base::failbit | ios_base::badbit);
   in_guard.open_file(filename, ios_base::in);
   stringstream result;
   result << in_guard.stream.rdbuf();
   in_guard.stream.close();
   return result;
}

const string File_edit::comment = "#";
const string File_edit::manual_dll = "-DMANUAL_DLL_LOAD"; 

#ifdef MANUAL_DLL_LOAD
const size_t File_edit::manual_dll_size = manual_dll.size();

void File_edit::delete_manual_dll_load() {
   stringstream text = get_content_file();
   fstream out_file;
   Io_guard out_guard(out_file, ios_base::failbit | ios_base::badbit);
   out_guard.open_file(filename, ios_base::out); 
   //  1st while loop  
   for (string line ; getline(text, line); out_guard.stream << '\n') {
      size_t manual_dll_pos = line.find(manual_dll);
      if (manual_dll_pos != string::npos) {
         size_t comment_pos = line.find(comment);
         if (comment_pos == string::npos || (comment_pos != string::npos && comment_pos > manual_dll_pos)) {  // interested line (flags_line before comment)
            stringstream line_stream(line);
            bool not_comment = true;   // code, not comment
            for (string word; getline(line_stream, word, ' '); ) { //  2nd while loop  
               if (not_comment) { // modify word only in code, not in comments
                  comment_pos = word.find(comment);
                  if (comment_pos != string::npos)
                     not_comment = false;
                  manual_dll_pos = word.find(manual_dll);
                  if (manual_dll_pos != string::npos) {
                     if (comment_pos == string::npos || (comment_pos != string::npos && comment_pos > manual_dll_pos)) {
                        //cerr << " erasing word = " << word << '\n';
                        word.erase(manual_dll_pos, manual_dll_pos + manual_dll_size);  // erase manual_dll in word
                     }
                  }
               }
               out_guard.stream << word << ' '; // save to file modified or unmodified word  
            }
            continue;    //  after process on interested line (flags_line before comment) continuing to 1st while loop
         }
      }
      out_guard.stream << line;    //  insert not interested line without changes 
   }
   out_guard.stream.close();
}
#else
enum class Insert_flag { NOT_YET, FLAG, DONE };   /* FLAG = "CPPFLAGS" or "CFLAGS" to distinct with
                                                 "CPPFLAGS=" or "CFLAGS="   */
const string File_edit::cppflags = "CPPFLAGS";
const string File_edit::cflags = "CFLAGS";
const size_t File_edit::cflags_size = cflags.size();
const size_t File_edit::cppflags_size = cppflags.size();

void File_edit::insert_manual_dll_load() {
   stringstream text = get_content_file();
   fstream out_file;
   Io_guard out_guard(out_file, ios_base::failbit | ios_base::badbit);
   out_guard.open_file(filename, ios_base::out);
   Insert_flag inserting = Insert_flag::NOT_YET;       
   /* 1st for loop  */
   for (string line; getline(text, line); out_guard.stream << '\n') {
      size_t flags_pos = line.find(cppflags);
      if (flags_pos == string::npos)
         flags_pos = line.find(cflags);
      if (flags_pos != string::npos) {
         size_t comment_pos = line.find(comment);
         if (comment_pos == string::npos || (comment_pos != string::npos && comment_pos > flags_pos)) {   /* interested line (flags_line before comment)  */
            stringstream line_stream(line);
            for (string word; getline(line_stream, word, ' '); ) {   /*  2nd for loop  */
               out_guard.stream << word << ' '; // save to file every word  
               if (Insert_flag::FLAG == inserting) {
                  out_guard.stream << manual_dll << ' ';
                  inserting = Insert_flag::DONE;   /* change inserting to DONE protect before next inserting manual_dll */
               }
               else if (Insert_flag::NOT_YET == inserting) {
                  if (word == cflags || word == cppflags)  /* word is "CPPFLAGS" or "CFLAGS"  */
                     inserting = Insert_flag::FLAG;       /* signal to insert manual_dll in next iteration due to "=" is separated from "CPPFLAGS" or "CFLAGS" */
                  else if (cflags == word.substr(0, cflags_size) || cppflags == word.substr(0, cppflags_size)) {  
                     out_guard.stream << manual_dll << ' '; /* "=" is in "CPPFLAGS=" or "CFLAG="  */
                     inserting = Insert_flag::DONE;
                  }
               }
            }
            inserting = Insert_flag::NOT_YET;  /* set to NOT_YET before read next line  */
            continue;  /*  after process on interested line (flags_line before comment) continuing to 1st for loop  */
         }
      }
      out_guard.stream << line;    //  insert not interested line without changes 
   }
   out_guard.stream.close();
}
#endif
