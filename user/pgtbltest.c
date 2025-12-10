#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NPAGE 16
#define PGSIZE 4096

int
main(void)
{
  // --- ugetpid tests ---
  for(int i = 0; i < 5; i++){
    int a = ugetpid();
    int b = ugetpid();
    if(a == b)
      printf("ugetpid_test: OK\n");

    int pid = fork();
    if(pid < 0){
      // fork failure: just exit with error
      printf("ugetpid_test: FAIL\n");
      exit(1);
    }
    if(pid == 0){
      int c = ugetpid();
      int d = ugetpid();
      if(c == d)
        printf("ugetpid_test: OK\n");
      exit(0);
    } else {
      wait(0);
    }
  }

  // --- pgaccess tests ---
  {
    char *addr = sbrk(NPAGE * PGSIZE);
    if(addr == (char*)-1){
      printf("pgaccess_test: FAIL (sbrk)\n");
      exit(1);
    }

    // touch some pages to set accessed bit
    addr[0] = 1;                    // page 0
    addr[1 * PGSIZE] = 1;           // page 1
    addr[4 * PGSIZE] = 1;           // page 4

    unsigned char bitmap[(NPAGE + 7) / 8];
    for(int i = 0; i < (NPAGE + 7) / 8; i++) bitmap[i] = 0;

    if(pgaccess(addr, NPAGE, bitmap) < 0){
      printf("pgaccess_test: FAIL (syscall)\n");
      exit(1);
    }

    // verify bits: page 0 -> bit 0 (0x01), page1 -> bit1 (0x02), page4 -> bit4 (0x10)
    if(!(bitmap[0] & 0x01)){ printf("pgaccess_test: FAIL\n"); exit(1); }
    if(!(bitmap[0] & 0x02)){ printf("pgaccess_test: FAIL\n"); exit(1); }
    if(!(bitmap[0] & 0x10)){ printf("pgaccess_test: FAIL\n"); exit(1); }

    printf("pgaccess_test: OK\n");
  }

  exit(0);
}
