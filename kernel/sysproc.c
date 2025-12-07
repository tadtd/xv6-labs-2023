#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_pgaccess(void)
{
    uint64 va;              // địa chỉ bắt đầu
    int npage;              // số page
    uint64 user_bitmap;     // buffer trả về
    struct proc *p = myproc();
    pte_t *pte;

    argaddr(0, &va);
    argint(1, &npage);
    argaddr(2, &user_bitmap);

    // validate npage
    if(npage <= 0 || npage > 256)
        return -1;

    // bắt đầu page phải căn chỉnh
    va = PGROUNDDOWN(va);

    char mask[32];             // 256 pages = 32 bytes
    int nbytes = (npage + 7)/8;
    memset(mask, 0, sizeof(mask));

    for(int i = 0; i < npage; i++){
        uint64 addr = va + i * PGSIZE;

        pte = walk(p->pagetable, addr, 0);
        if(!pte)
            continue;

        if(*pte & PTE_A){
            mask[i/8] |= (1 << (i%8));
            *pte &= ~PTE_A;       // clear bit
        }
    }

    if(copyout(p->pagetable, user_bitmap, mask, nbytes) < 0)
        return -1;

    return 0;
}


