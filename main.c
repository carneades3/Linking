#include "print.h"
#include "connector.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define LANGUAGE "C90"

int main(void) {
   LOG("\n%s \n", LANGUAGE);
   LOG_FUNC(__FUNCTION__);
   demo_init("Nicolaus Copernicus");
   LOG("%s: %s human name is %s: ", LANGUAGE, __FUNCTION__, demo_get_name());
   demo_set_name("Socrates");
   LOG("%s: %s human name is %s: ", LANGUAGE, __FUNCTION__, demo_get_name());
   demo_destroy();
   
   void* money = Money_int__init_1("ANSI C");
   LOG("\nAddress of money is: %p\n", money);
   money = Money_int__create_1("ANSI C");
   LOG("\nAddress of money is: %p\n", money);
   money = Money_int__init_2("ANSI C", 9);
   LOG("\nAddress of money is: %p\n", money);
   money = Money_int__create_2("ANSI C", 7);
   LOG("\nAddress of money is: %p\n", money);
   
   return 0;
}
