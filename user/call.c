#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int g(int x) {
  return x+3;
}

int f(int x) {
  return g(x);
}

// __attribute__((noinline))
// void set_registers(int a, int b, int c) {
//   asm(""); // Do nothing, just return
// }

void main(void) {
  printf("%d %d\n", f(8)+1, 13);
  
  // unsigned int i = 0x00646c72; 
  // printf("H%x Wo%s", 57616, (char *) &i); 

  // set_registers(1, 2, 3);
  // printf("x=%d y=%d", 3);
  exit(0);
}
