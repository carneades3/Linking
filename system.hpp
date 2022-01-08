#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <string>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <system_error>
#include <iostream>

#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>

using std::string;
using std::system_error;
using std::invalid_argument;
using std::make_error_code;
using std::errc;
using std::error_code;
using std::string;
using std::cerr;

namespace system_utility {   
   
template <typename... T>
int execute(const char * file, T &&... args) {
   if (! file)
      throw std::invalid_argument("filename is null");
   pid_t  pid;
   int    status;
   pid = fork();
   if (pid < 0) 
      throw system_error(make_error_code(errc::no_child_process), "forking child process failed ");
   else if (pid == 0) {
      cerr << "Child process pid = " << getpid() << ", parent process pid = " << getppid() << '\n';
      if (execlp(file, std::forward<T>(args)..., 0 ) < 0)  
         throw std::system_error(error_code(static_cast<int>(errno), std::generic_category()),
                            "Call of execvp failed. Error: " + string(strerror(errno)));
   }
   else {
      cerr << "Parent process: pid = " << getpid() << '\n';
      cerr << "Parent process: waiting for completion of child process with pid = " << pid << '\n';
      pid_t result;
      do {
         result = wait(&status);
      } while (result != pid && result != -1);

      cerr << "status of child process: " << status << '\n';
      cerr << "wait(&status) on success returns the process ID of the terminated child - on failure, -1 is returned.\n \
      Result of waiting for child process: " << result << '\n';
      cerr << "result of waiting for child process " << (result == static_cast<int>(pid) ? "==" : "!=") << " child pid\n";
      if (result == -1) 
         throw system_error(error_code(static_cast<int>(errno), std::generic_category()),
                                       "Call of wait(&status) failed. Error: " +  string(strerror(errno)));
      return result;
   }
}
   
int call_system(const string & command);

}

#endif
