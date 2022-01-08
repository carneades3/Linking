#ifndef FILE_MODIFY_H
#define FILE_MODIFY_H

#include <stdio.h>

struct File_modify_t;

typedef struct File_modify_t File_modify_t;

File_modify_t* File_modify_malloc();

int File_modify_init(File_modify_t**, const char * const);

void File_modify_destroy(File_modify_t**);

int edit_makefile(File_modify_t*);

int File_modify_set(File_modify_t * const, const char * const);

int File_modify_get_filename(const File_modify_t * const, char ** const filename);

typedef int bool_t;

bool_t exist_file(const char * const file_name);
FILE*  open_file( const char * const filename, const char * const mode );
char * read_line(const FILE * const file);

#endif
