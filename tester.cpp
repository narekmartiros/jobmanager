#include <iostream>
#include <string>

int job1() {
  return 1;
}
int job2() {
  return 0;
}
int job3() {
  return -1;
}

int main(int argc , char *argv[]) {

  int i = atoi(argv[0]);
  int (*jobs[])() = {&job1, &job2, &job3};

  return jobs[i]();
}
