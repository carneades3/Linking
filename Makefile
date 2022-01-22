#CXX = g++
CXX = clang
CPPFLAGS = -DMANUAL_DLL_LOAD -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 #-fsanitize=address 
TEST_CPPFLAGS = -Wfatal-errors -Wall -Wextra -Wconversion -std=c++14 #-fsanitize=address 

#CC = gcc
CC = clang
CFLAGS = -DMANUAL_DLL_LOAD -Wfatal-errors -Wall -Wextra -Wconversion -std=c11 #-fsanitize=address 
TEST_CFLAGS = -Wfatal-errors -Wall -Wextra -Wconversion -std=c11 #-fsanitize=address 
#  -fcompare-debug-second -DMANUAL_DLL_LOAD

LDFLAGS = #-fsanitize=address -static-libasan 

PROGRAMS  = libdemo.so libdemoderived.so libbase_cpp.so libbase_cpp_connector.so libderived_cpp_connector.so libderived_cpp.so  libvariadic_template.so libconnector.so libdemoderivedconnector.so main_cpp test_cpp      libhuman.so libhumanderived.so libbase_c.so libderived_c.so c_main linking_test_cpp c_linking_test c_test 

GCC_CPPFLAGS = -DMESSAGE='"Compiled with G++"' 
CLANG_CPPFLAGS = -L/usr/lib -lstdc++ -lm -DMESSAGE='"Compiled with Clang"' 

GCC_CFLAGS = -DMESSAGE='"Compiled with GCC"' 
CLANG_CFLAGS = -lm -DMESSAGE='"Compiled with Clang"' 

ifeq ($(CXX), g++)
CPPFLAGS += -DMANUAL_DLL_LOAD $(GCC_CPPFLAGS) 
TEST_CPPFLAGS += $(GCC_CPPFLAGS) 
else ifeq ($(CXX), clang)
CPPFLAGS += -DMANUAL_DLL_LOAD $(CLANG_CPPFLAGS) 
TEST_CPPFLAGS += $(CLANG_CPPFLAGS) 
endif

ifeq ($(CC), gcc)
CFLAGS += -DMANUAL_DLL_LOAD $(GCC_CFLAGS) 
TEST_CFLAGS += $(GCC_CFLAGS) 
else ifeq ($(CC), clang)
CFLAGS += -DMANUAL_DLL_LOAD $(CLANG_CFLAGS) 
TEST_CFLAGS += $(CLANG_CFLAGS) 
endif

.PHONY: clean all

all: $(PROGRAMS) 

c_test: c_test.o system.o print.o c_string.o regular_expr.o
	$(CC) $(TEST_CFLAGS) $(LDFLAGS) c_test.o system.o print.o c_string.o regular_expr.o -o c_test 
	
c_test.o: test.c print.h system.h result_codes.h c_string.h
	$(CC) $(TEST_CFLAGS) -c test.c -o c_test.o 

c_main: c_main.o print.o file_modify.o c_string.o system.o
	$(CC) $(CFLAGS) $(LDFLAGS) c_main.o print.o file_modify.o c_string.o system.o -o c_main 
	
c_main.o: main.c print.h system.h file_modify.h result_codes.h c_string.h
	$(CC) $(CFLAGS) -c main.c -o c_main.o 

c_linking_test: c_linking_test.o libconnector.so shared_lib_open.o print.o c_string.o demo_functions.o human_functions.o human_derived_functions.o c_utility.o libhuman.so libdemo.so libdemoderived.so libhumanderived.so  c_variadic_template_test.o demo_derived_functions.o base_cpp_test.o c_hierarchy_test.o derived_cpp_test.o c_base_test.o c_derived_test.o libbase_cpp_connector.so libderived_cpp_connector.so libbase_c.so libderived_c.so
	$(CC) $(CFLAGS) -lm $(LDFLAGS) c_linking_test.o shared_lib_open.o print.o c_string.o demo_functions.o human_functions.o human_derived_functions.o c_utility.o c_variadic_template_test.o demo_derived_functions.o base_cpp_test.o c_hierarchy_test.o derived_cpp_test.o c_base_test.o c_derived_test.o -L. -lhumanderived -ldemoderivedconnector -lbase_cpp_connector -lderived_cpp_connector -lderived_c -Wl,--no-as-needed -lbase_cpp -lderived_cpp -lvariadic_template -lhuman -ldemoderived -lconnector -ldemo -lbase_c -Wl,--as-needed -ldl -o c_linking_test 
	
c_linking_test.o: connector.h linking_test.c shared_lib_open.h print.h c_string.h human.h utility.h demo_functions.h human_functions.h demo_derived_functions.h variadic_template_test.h base_test.h derived_test.h derived_cpp_test.h base_cpp_test.h
	$(CC) $(CFLAGS) -c linking_test.c -o c_linking_test.o 
	
c_derived_test.o: base_test.h derived_test.h derived_test.c utility.h shared_lib_open.h result_codes.h print.h base.h hierarchy_test.h
	$(CC) $(CFLAGS) -c derived_test.c -o c_derived_test.o 
	
c_base_test.o: base_test.h base_test.c utility.h shared_lib_open.h result_codes.h print.h base.h hierarchy_test.h
	$(CC) $(CFLAGS) -c base_test.c -o c_base_test.o 
	
derived_cpp_test.o: derived_cpp_test.c derived_cpp_test.h shared_lib_open.h derived_connector.h hierarchy_test.h result_codes.h
	$(CC) $(CFLAGS) -c derived_cpp_test.c 
	
base_cpp_test.o: base_cpp_test.c base_cpp_test.h shared_lib_open.h base_connector.h hierarchy_test.h result_codes.h
	$(CC) $(CFLAGS) -c base_cpp_test.c 
	
c_hierarchy_test.o: hierarchy_test.c hierarchy_test.h utility.h pair.h singleton.h hierarchy_test.h
	$(CC) $(CFLAGS) -c hierarchy_test.c -o c_hierarchy_test.o 
	
demo_derived_functions.o: demo_derived_functions.h demo_derived_functions.c utility.h shared_lib_open.h result_codes.h
	$(CC) $(CFLAGS) -c demo_derived_functions.c -o demo_derived_functions.o 
	
demo_functions.o: demo_functions.h demo_functions.c utility.h shared_lib_open.h result_codes.h
	$(CC) $(CFLAGS) -c demo_functions.c 
	
human_derived_functions.o: human_derived_functions.h human_derived_functions.c utility.h shared_lib_open.h result_codes.h
	$(CC) $(CFLAGS) -c human_derived_functions.c 
	
human_functions.o: human_functions.h human_functions.c utility.h shared_lib_open.h result_codes.h
	$(CC) $(CFLAGS) -c human_functions.c 
	
c_variadic_template_test.o: variadic_template_test.h variadic_template_test.c shared_lib_open.h c_string.h result_codes.h
	$(CC) $(CFLAGS) -c variadic_template_test.c -o c_variadic_template_test.o 
	
c_utility.o: utility.h utility.c
	$(CC) $(CFLAGS) -c utility.c -o c_utility.o 
	
system.o: system.c system.h print.h result_codes.h
	$(CC) $(CFLAGS) -c system.c 
	
file_modify.o: file_modify.h file_modify.c print.h c_string.h result_codes.h
	$(CC) $(CFLAGS) -c file_modify.c 
	
shared_lib_open.o: shared_lib_open.h shared_lib_open.c
	$(CC) $(CFLAGS) -c shared_lib_open.c -o shared_lib_open.o 
	
print.o: print.h print.c
	$(CC) $(CFLAGS) -c -fPIC print.c 
	
c_string.o: c_string.h c_string.c
	$(CC) $(CFLAGS) -c -fPIC c_string.c 
	
libhumanderived.so: libhuman.so humanderived.o allocate.o
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -L. -lhuman -o libhumanderived.so humanderived.o allocate.o 
	
humanderived.o: human_private.h human_derived.c human_derived.h print.h result_codes.h allocate.h utility.h
	$(CC) $(CFLAGS) -c -fPIC human_derived.c -o humanderived.o 
	
libhuman.so: human.o print.o c_string.o regular_expr.o
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o libhuman.so human.o print.o c_string.o regular_expr.o 
	
human.o: human_private.h human.h human.c regular_expr.h print.h result_codes.h singleton.h 
	$(CC) $(CFLAGS) -c -fPIC human.c 
	
regular_expr.o: regular_expr.h regular_expr.c print.h result_codes.h
	$(CC) $(CFLAGS) -c -fPIC regular_expr.c 
	
allocate.o: allocate.h allocate.c
	$(CC) $(CFLAGS) -c -fPIC allocate.c 
	
libderived_c.so: derived_c.o libbase_c.so
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -L. -lbase_c -o libderived_c.so derived_c.o 
	
derived_c.o: derived_private.h base.h derived.c derived.h abstract.h abstract_private.h interface.h interface_private.h print.h result_codes.h c_string.h utility.h allocate.h
	$(CC) $(CFLAGS) -c -fPIC derived.c -o derived_c.o 
	
libbase_c.so:  base_c.o abstract_c.o interface_c.o
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -lm -o libbase_c.so base_c.o abstract_c.o interface_c.o 
	
base_c.o: base_private.h base.h base.c abstract.h abstract_private.h interface.h interface_private.h print.h result_codes.h c_string.h utility.h allocate.h
	$(CC) $(CFLAGS) -c -fPIC base.c -o base_c.o 
	
abstract_c.o: abstract.c abstract.h abstract_private.h interface.h interface_private.h print.h result_codes.h c_string.h utility.h allocate.h
	$(CC) $(CFLAGS) -c -fPIC abstract.c -o abstract_c.o 
	
interface_c.o: interface.c interface.h interface_private.h print.h result_codes.h c_string.h utility.h
	$(CC) $(CFLAGS) -c -fPIC interface.c -o interface_c.o 
	
	
	
	
test_cpp: test_cpp.o system_cpp.o
	$(CXX) $(TEST_CPPFLAGS) $(LDFLAGS) test_cpp.o system_cpp.o -o test_cpp 
	
test_cpp.o: test.cpp print.hpp system.hpp result_codes.h
	$(CXX) $(TEST_CPPFLAGS) -c test.cpp -o test_cpp.o 

main_cpp: main_cpp.o file_edit.o system_cpp.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) main_cpp.o file_edit.o system_cpp.o -o main_cpp 
	
linking_test_cpp: linking_test_cpp.o variadic_template_test_cpp.o demo_test.o demo_derived_test.o base_test.o derived_test.o hierarchy_test_cpp.o human_test.o human_derived_test.o base_c_test.o derived_c_test.o shared_lib_open.o print.o libhuman.so libdemo.so libdemoderived.so
	$(CXX) $(CPPFLAGS) $(LDFLAGS) linking_test_cpp.o variadic_template_test_cpp.o demo_test.o demo_derived_test.o base_test.o derived_test.o hierarchy_test_cpp.o human_test.o human_derived_test.o base_c_test.o derived_c_test.o shared_lib_open.o -L. -lderived_c -lbase_cpp -lderived_cpp -lconnector -ldemo -ldemoderived -lhumanderived -Wl,--no-as-needed -lvariadic_template -lhuman -lbase_c -Wl,--as-needed -ldl -o linking_test_cpp 
	
hierarchy_test_cpp.o: hierarchy_test.hpp hierarchy_test.cpp
	$(CXX) $(CPPFLAGS) -c hierarchy_test.cpp -o hierarchy_test_cpp.o 
	
derived_test.o: derived_test.hpp derived_test.cpp utility.hpp shared_lib_open.h result_codes.h print.hpp derived.hpp base_test.hpp base_test.cpp hierarchy_test.hpp
	$(CXX) $(CPPFLAGS) -c derived_test.cpp 
	
base_test.o: base_test.hpp base_test.cpp utility.hpp shared_lib_open.h result_codes.h print.hpp base.hpp hierarchy_test.hpp
	$(CXX) $(CPPFLAGS) -c base_test.cpp 
	
demo_test.o: demo_test.hpp demo_test.cpp utility.hpp shared_lib_open.h result_codes.h connector.h
	$(CXX) $(CPPFLAGS) -c demo_test.cpp 
	
demo_derived_test.o: demo_derived_test.hpp demo_derived_test.cpp utility.hpp shared_lib_open.h result_codes.h connector.h
	$(CXX) $(CPPFLAGS) -c demo_derived_test.cpp -o demo_derived_test.o 
	
base_c_test.o: base_c_test.hpp base_c_test.cpp utility.hpp shared_lib_open.h result_codes.h print.hpp
	$(CXX) $(CPPFLAGS) -c base_c_test.cpp 
	
derived_c_test.o: derived_c_test.hpp derived_c_test.cpp utility.hpp shared_lib_open.h result_codes.h
	$(CXX) $(CPPFLAGS) -c derived_c_test.cpp 
	
human_test.o: human_test.hpp human_test.cpp utility.hpp shared_lib_open.h result_codes.h human.h human_functions.h
	$(CXX) $(CPPFLAGS) -c human_test.cpp 
	
human_derived_test.o: human_derived_test.hpp human_derived_test.cpp utility.hpp shared_lib_open.h result_codes.h human_derived_functions.h human.h human_derived.h
	$(CXX) $(CPPFLAGS) -c human_derived_test.cpp 
	
variadic_template_test_cpp.o: variadic_template_test.hpp variadic_template_test.cpp shared_lib_open.h result_codes.h
	$(CXX) $(CPPFLAGS) -c variadic_template_test.cpp -o variadic_template_test_cpp.o 
	
libconnector.so: utility_cpp.o connector.o libdemo.so libvariadic_template.so
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -L. -ldemo -lvariadic_template -o libconnector.so connector.o utility_cpp.o 
	
connector.o: connector.h connector.cpp null.hpp variadic_template.hpp variadic_template.cpp utility.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC connector.cpp 
	
libdemoderivedconnector.so: libconnector.so libdemoderived.so demoderivedconnector.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -L. -lconnector -ldemoderived -o libdemoderivedconnector.so demoderivedconnector.o 
	
demoderivedconnector.o: connector.h connector.cpp null.hpp result_codes.h demo_derived.hpp demo_derived_connector.h demo_derived_connector.cpp utility.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC demo_derived_connector.cpp -o demoderivedconnector.o 
	
main_cpp.o: main.cpp print.hpp file_edit.hpp system.hpp result_codes.h
	$(CXX) $(CPPFLAGS) -c main.cpp -o main_cpp.o 

linking_test_cpp.o: linking_test.cpp variadic_template.hpp variadic_template.cpp demo.hpp demo.cpp utility.hpp print.hpp connector.h shared_lib_open.h human.h derived_c_test.hpp base_c_test.hpp derived_test.hpp base_test.hpp
	$(CXX) $(CPPFLAGS) -c linking_test.cpp -o linking_test_cpp.o 
	
system_cpp.o: system.cpp system.hpp
	$(CXX) $(CPPFLAGS) -c system.cpp -o system_cpp.o 
	
file_edit.o: file_edit.hpp file_edit.cpp
	$(CXX) $(CPPFLAGS) -c file_edit.cpp 
	
libvariadic_template.so: variadic_template_instances.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -o libvariadic_template.so variadic_template_instances.o 

variadic_template_instances.o: variadic_template.hpp variadic_template.cpp variadic_template_instances.cpp
	$(CXX) $(CPPFLAGS) -c -fPIC variadic_template_instances.cpp -o variadic_template_instances.o 
	
libdemoderived.so: demoderived.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -o libdemoderived.so demoderived.o 

demoderived.o: demo_derived.hpp demo_derived.cpp null.hpp print.hpp utility.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC demo_derived.cpp -o demoderived.o 
	
utility_cpp.o: utility.hpp utility.cpp
	$(CXX) $(CPPFLAGS) -c -fPIC utility.cpp -o utility_cpp.o 
	
libdemo.so: demo.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -o libdemo.so demo.o 

demo.o: demo.hpp demo.cpp null.hpp print.hpp utility.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC demo.cpp 
	
libderived_cpp_connector.so: libderived_cpp.so derived_cpp_connector.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -L. -lderived_cpp -o libderived_cpp_connector.so derived_cpp_connector.o 
	
derived_cpp_connector.o: derived_connector.cpp derived_connector.h null.hpp result_codes.h derived.hpp utility.hpp abstract_connector.h
	$(CXX) $(CPPFLAGS) -c -fPIC derived_connector.cpp -o derived_cpp_connector.o 
	
libderived_cpp.so:  derived_cpp.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -o libderived_cpp.so derived_cpp.o 
	
derived_cpp.o: derived.cpp base.hpp derived.hpp print.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC derived.cpp -o derived_cpp.o 
	
libbase_cpp_connector.so: libbase_cpp.so base_cpp_connector.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -L. -lbase_cpp -o libbase_cpp_connector.so base_cpp_connector.o 
	
base_cpp_connector.o: base_connector.cpp base_connector.h null.hpp result_codes.h base.hpp utility.hpp abstract_connector.h
	$(CXX) $(CPPFLAGS) -c -fPIC base_connector.cpp -o base_cpp_connector.o 
	
libbase_cpp.so:  base_cpp.o abstract_cpp.o interface_cpp.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -shared -o libbase_cpp.so base_cpp.o abstract_cpp.o interface_cpp.o 
	
base_cpp.o: base.cpp abstract.hpp base.hpp print.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC base.cpp -o base_cpp.o 
	
abstract_cpp.o: abstract.cpp abstract.hpp interface.hpp print.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC abstract.cpp -o abstract_cpp.o 
	
interface_cpp.o: interface.cpp interface.hpp print.hpp
	$(CXX) $(CPPFLAGS) -c -fPIC interface.cpp -o interface_cpp.o 
	
clean :
	rm $(PROGRAMS) *.o
