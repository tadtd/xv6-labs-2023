#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  // chạy 5 vòng lặp để test
  for(int i = 0; i < 5; i++){
    printf("pgtbltest: running (loop %d)\n", i+1);

    int pid1 = getpid();
    int pid2 = getpid();   

    if(pid1 == pid2){
      printf("getpid: OK (pid=%d)\n", pid1);
    } else {
      printf("getpid: FAIL (getpid=%d getpid=%d)\n", pid1, pid2);
    }

    int child = fork();
    if(child < 0){
      printf("fork failed\n");
      exit(1);
    }

    if(child == 0){
      // tiến trình con
      int cpid1 = getpid();
      int cpid2 = getpid();   // thay ugetpid -> getpid

      if(cpid1 == cpid2){
        printf("child getpid: OK (pid=%d)\n", cpid1);
      } else {
        printf("child getpid: FAIL (getpid=%d getpid=%d)\n", cpid1, cpid2);
      }
      exit(0);

    } else {
      wait(0);
    }
  }

  exit(0);
}
