#include "file_modify.h"
#include "print.h"
#include "c_string.h"
#include "utility.h"
#include "result_codes.h"

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

bool_t exist_file(const char * const name) {
   REQUIRE_NON_NULL(name, "name is null");
   struct stat   buffer;
   int result = stat (name, &buffer);
   if (-1 == result) {
      LOG("Stat(): file with name: %s. Error: %s\n", name, strerror(errno));
   }
   else if (S_ISREG( buffer.st_mode ) == 0) {
      LOG("Error: file \'%s\' is not regular file\n", name);
      result = -2;
   }
   return result == 0;
}

FILE* open_file( const char * const filename, const char const * mode ) {
   if (! filename || ! mode) { 
      LOG_EXIT(__FUNCTION__, "filename is null / file access mode is null", EXIT_FAILURE);
   }
   FILE * file = fopen (filename, mode);
   if (file != NULL) {
      LOG("open file of %s\n", filename);
   }
   else {
      LOG("file of %s is not exist\n", filename);
      LOG("Call of fopen failed. Error: %s\n", strerror(errno));
   }
   return file;
} 

char * read_line(const FILE * const file) {
   if (! file) { 
      LOG_EXIT(__FUNCTION__, "file is null", EXIT_FAILURE);
   }
   int ch;  /*  int (not char) due to value of EOF is a negative integer constant. The precise value is implementation-defined.  */
   int index = 0;
   size_t buff_len = 1;
   char * buffer = calloc(buff_len, sizeof (char));
   char * tmp_buff;
   if (! buffer) {
      LOG_FUNC(__FUNCTION__);
      LOG("%s\n", "Out of memory");
      return NULL;
   }
   while ((ch = fgetc(file)) != '\n' && ch != EOF) {
      if (ferror (file))
         perror("Error reading file\n");
      tmp_buff = realloc(buffer, ++buff_len);
      if (! tmp_buff) {
         free(buffer);
         LOG_FUNC(__FUNCTION__);
         LOG("%s\n", "Out of memory");
         return NULL;
      }
      buffer = tmp_buff;
      buffer[index++] = (char) ch;
   }
   buffer[index] = '\0';
   if (ch == EOF && (index == 0 || ferror(file))) {
      free(buffer);
      return NULL;
   }
   
   return buffer;
}

#define CHECK_FILE(filename, null_message) \
REQUIRE_NON_NULL(filename, (null_message)) \
if (! exist_file(filename)) { \
      const char * message = concatenate("Error: ", strerror(errno)); \
      LOG_EXIT_FREE(__FUNCTION__, message, EXIT_FAILURE); \
   }

struct File_modify_t {
   char * filename;
};

File_modify_t* File_modify_malloc() {
   File_modify_t * new = NULL;
   ALLOCATE_STRING(new, sizeof (File_modify_t));
   return new;
}

Result_codes File_modify_init(File_modify_t ** object, const char * const filename) {
   REQUIRE_NON_NULL(object, "object is null");
   if (*object) {
      LOG_FUNC(__FUNCTION__);
      LOG("%s\n", "*object must be null");
      return INVALID_ARG;
   }
   CHECK_FILE(filename, "filename is null");
   *object = File_modify_malloc();
   if (! *object)
      return BAD_ALLOC;
   ALLOCATE_STRING((*object)->filename, strlen(filename) + 1);
   if (NULL == (*object)->filename)
      return BAD_ALLOC;
   strcpy((*object)->filename, filename);
   return OK;
}

void File_modify_destroy(File_modify_t ** object) {
   REQUIRE_NON_NULL(object, "object is null");
   REQUIRE_NON_NULL(*object, "*object is null");
   free((*object)->filename);
   free(*object);
   *object = NULL; /* to avoid double free or corruption (fasttop)  */
}

Result_codes File_modify_set(File_modify_t * const object, const char * const filename) {
   REQUIRE_NON_NULL(object, "object is null");
   CHECK_FILE(filename, "filename is null");
   free(object->filename);
   ALLOCATE_STRING(object->filename, strlen(filename) + 1);
   if (NULL == object->filename)
      return BAD_ALLOC;
   strcpy(object->filename, filename);
   return OK;
}

Result_codes File_modify_get_filename(const File_modify_t * const object, char ** const filename) {
   REQUIRE_NON_NULL(object, "object is null");
   REQUIRE_NON_NULL(filename, "filename is null");
   if (*filename) {
      LOG_FUNC(__FUNCTION__);
      LOG("%s\n", "*filename must be null");
      return INVALID_ARG;
   }
   ALLOCATE_STRING(*filename, strlen(object->filename) + 1);
   if (NULL == filename)
      return BAD_ALLOC;
   strcpy(*filename, object->filename);
   return OK;
}

#ifdef MANUAL_DLL_LOAD
static void delete_manual_dll_load(FILE * file, FILE * edited_file) {
   if (! file || ! edited_file) { 
      LOG_EXIT(__FUNCTION__, "file is null / edited_file is null", EXIT_FAILURE);
   }
   const char comment[] = "#";
   const char * manual_dll = "-DMANUAL_DLL_LOAD"; 
   const size_t manual_dll_size = strlen(manual_dll); 
   char * line;
   char * word;
   char * manual_dll_line;
   char * comment_line;
   bool_t not_comment = 1;
   bool_t insert_word = 0;
   /* 1st for loop  */
   for (line = read_line(file); line != NULL; free(line), line = read_line(file), fprintf (edited_file, "%c", '\n')) {
      manual_dll_line = strstr (line, manual_dll);
      if (manual_dll_line) {
         comment_line = strstr (line, comment);
         if (! comment_line || (comment_line && comment_line > manual_dll_line)) {  /* interested line (flags_line before comment)  */
            not_comment = 1;   /* code, not comment */
            for (word = strtok (line, " "); word; word = strtok (NULL, " ")) {   /*  2nd for loop  */
               if (not_comment) { /* modify word only in code, not in comments */
                  comment_line = strstr (word, comment);
                  if (comment_line)
                     not_comment = 0;
                  manual_dll_line = strstr (word, manual_dll);
                  if (manual_dll_line) {
                     if (! comment_line || (comment_line && comment_line > manual_dll_line)) {
                        insert_word = 1;
                        size_t word_length = strlen(word);
                        for (size_t i = 0; i < word_length; ) {   /*  3rd for loop  */
                           if (word + i == manual_dll_line) {
                              for (size_t j = 0; j < manual_dll_size; j++)  /*  skip  manual_dll */
                                 i++;
                              continue;  /*  continue to 3rd for loop to check condition: i < word_length  */
                           }
                           fputc (word[i] , edited_file );
                           i++;
                        } 
                        fputc (' ' , edited_file );
                     }
                  }
               }
               if (0 == insert_word)
                  fprintf (edited_file, "%s ", word);
               else
                  insert_word = 0;
            }
            continue;  /*  after process on interested line (flags_line before comment) continuing to 1st for loop  */
         }
      }
      fputs (line, edited_file);  /*  insert not interested line without changes */
   }
}
#else
enum Insert_flag { NOT_YET, FLAG, DONE };   /* FLAG = "CPPFLAGS" or "CFLAGS" to distinct with
                                                 "CPPFLAGS=" or "CFLAGS="   */
static void insert_manual_dll_load(FILE * file, FILE * edited_file) {
   if (! file || ! edited_file) { 
      LOG_EXIT(__FUNCTION__, "file is null / edited_file is null", EXIT_FAILURE);
   }
   const char cppflags[] = "CPPFLAGS";
   const char cflags[] = "CFLAGS";
   const size_t cflags_size = strlen(cflags);
   const size_t cppflags_size = strlen(cppflags);
   const char comment[] = "#";
   char * line;
   char * word;
   char * flags_line; 
   char * comment_line;
   enum Insert_flag inserting = NOT_YET;
   const char * manual_dll = "-DMANUAL_DLL_LOAD";       
   /* 1st for loop  */
   for (line = read_line(file); line != NULL; free(line), line = read_line(file), fprintf (edited_file, "%c", '\n')) {
      flags_line = strstr (line, cppflags);
      if (! flags_line)
         flags_line = strstr (line, cflags);
      if (flags_line) {
         comment_line = strstr (line, comment);
         if (! comment_line || (comment_line && comment_line > flags_line)) {   /* interested line (flags_line before comment)  */
            for (word = strtok (line, " "); word; word = strtok (NULL, " ")) {   /*  2nd for loop  */
               fprintf (edited_file, "%s ", word);
               if (FLAG == inserting) {
                  fprintf (edited_file, "%s ", manual_dll);
                  inserting = DONE;   /* change inserting to DONE protect before next inserting manual_dll */
               }
               else if (NOT_YET == inserting) {
                  if (0 == strcmp(word, cflags) || 0 == strcmp(word, cppflags))  /* word is "CPPFLAGS" or "CFLAGS"  */
                     inserting = FLAG;       /* signal to insert manual_dll in next iteration due to "=" is separated from "CPPFLAGS" or "CFLAGS" */
                  else if (0 == strncmp(word, cflags, cflags_size) || 0 == strncmp(word, cppflags, cppflags_size)) {  
                     fprintf (edited_file, "%s ", manual_dll);  /* "=" is in "CPPFLAGS=" or "CFLAG="  */
                     inserting = DONE;
                  }
               }
            }
            inserting = NOT_YET;  /* set to NOT_YET before read next line  */
            continue;  /*  after process on interested line (flags_line before comment) continuing to 1st for loop  */
         }
      }
      fputs (line, edited_file);  /*  insert not interested line without changes  */
   }
}
#endif

Result_codes edit_makefile(File_modify_t * object) {
   REQUIRE_NON_NULL(object, "object is null");
   FILE* file = open_file(object->filename, "r");
   const char * filename_tmp = concatenate(object->filename, ".tmp");
   FILE * edited_file = open_file(filename_tmp, "w");
   free(filename_tmp);
   if (! file || ! edited_file)
      return OPEN_FILE_ERROR;
#ifdef MANUAL_DLL_LOAD
   delete_manual_dll_load(file, edited_file);
#else
   insert_manual_dll_load(file, edited_file);
#endif
   if (0 != fclose(file) || 0 != fclose(edited_file)) {
      LOG("Call of fclose failed. Error: %s\n", strerror(errno));
      return FILE_CLOSE_ERROR;
   }
   if (0 != rename("Makefile.tmp", "Makefile")) {
      LOG("Call of rename failed. Error: %s\n", strerror(errno));
      return RENAME_FILE_ERROR;
   }
   return OK;
}
