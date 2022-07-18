#include<stdlib.h>
#include<unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>

void printReport(const std::vector<pid_t> &pids, const std::vector<int> &status) {
  for(int i = 0; i < pids.size(); ++i) {
    std::cout << "Process <" << pids[i] << "> exited with status "; 
    switch(status[i]) {
      case 1:
        std::cout << " UNRESOLVED" << std::endl;
        break;
      case 0:
        std::cout << " SUCCESS" << std::endl;
        break;
      case 255:
        std::cout << " FAILURE" << std::endl;
        break;
      default:
        std::cout << " UNKNOWN ERROR CODE";
        break;
    }
  }
}

int main(int argc , char *argv[]) {
  std::string tester;
  if(argc > 1) {
    tester = std::string(argv[1]);
  } else {
    std::cerr << "Please provide test executor name" << std::endl;
    exit(-1);
  }

  std::vector<pid_t> pids;
  std::vector<int> status;
  int temp_status = 0;
  int wpid = 0;

  char *args1[] = {"0", nullptr};
  char *args2[] = {"1", nullptr};
  char *args3[] = {"2", nullptr};
  char **args[] = {args1, args2, args3};

  for(int i = 0; i < 3; ++i) {
    pids.push_back(fork());
    if(pids[i] == 0) {
      const char *testerName = tester.c_str();
      if(execv(testerName, args[i]) == -1) {
        std::string msg = tester + " failed to run";
        perror(msg.c_str());
      }
      exit(EXIT_FAILURE);
    } else {
      do {
        wpid = waitpid(pids[i], &temp_status, WUNTRACED);
      } while (!WIFEXITED(temp_status) && !WIFSIGNALED(temp_status));
    }
    int exit_status = WEXITSTATUS(temp_status);
    status.push_back(exit_status);
  }
  printReport(pids, status);

  return 1;
}
