#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  // chạy 5 vòng lặp để test, tránh hết slot
  for(int i = 0; i < 5; i++){
    printf("pgtbltest: running (loop %d)\n", i+1);

    // Test ugetpid so với getpid trong tiến trình hiện tại
    int pid1 = getpid();
    int pid2 = ugetpid();
    if(pid1 == pid2){
      printf("ugetpid: OK (pid=%d)\n", pid1);
    } else {
      printf("ugetpid: FAIL (getpid=%d ugetpid=%d)\n", pid1, pid2);
    }

    // Test trong tiến trình con (fork)
    int child = fork();
    if(child < 0){
      printf("fork failed\n");
      exit(1);
    }
    if(child == 0){
      // tiến trình con
      int cpid1 = getpid();
      int cpid2 = ugetpid();
      if(cpid1 == cpid2){
        printf("child ugetpid: OK (pid=%d)\n", cpid1);
      } else {
        printf("child ugetpid: FAIL (getpid=%d ugetpid=%d)\n", cpid1, cpid2);
      }
      exit(0);
    } else {
      // tiến trình cha chờ con
      wait(0);
    }
  }

  exit(0);
}
