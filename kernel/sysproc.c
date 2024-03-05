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
sys_sigalarm(void){
uint64 handler;
int alarm_ticks;

argint(0, &alarm_ticks);
argaddr(1, &handler);

//struct trapframe *tframe = kalloc();
//printf("bobaloo 1 \n");

//memmove(tframe, myproc()->trapframe, PGSIZE);
//memmove(myproc()->alarm_trap, tframe, PGSIZE);
//myproc()->alarm_trap = tframe;
//tframe = myproc()->trapframe;

//memmove(myproc()->alarm_trap, myproc()->trapframe, PGSIZE);
// printf("bobaloo 2 \n");
// myproc()->trapframe->epc = handler;
// printf("bobaloo 3 \n");

myproc()->alarm_ticks = alarm_ticks;
myproc()->handler = handler;

// kfree(tframe);
// printf("bobaloo 4 \n");

return 0; //Fix later
}

// should restore state
uint64
sys_sigreturn(void){
  // struct proc *p = myproc();
  memmove(myproc()->trapframe, myproc()->alarm_trap, PGSIZE); // restores original trap frame
  //printf("bobaloo 5 \n");
  kfree(myproc()->alarm_trap); // frees memory that was used
  // resets all of these back to default 
  // myproc()->alarm_trap = 0;
  //printf("bobaloo 6 \n");
  //myproc()->alarm_set = 0;
  //p->current_ticks = 0; 
  myproc()->current_ticks = 0; // for efficiency 
  myproc()->alarm_set = 0;
	//printf("return last lines\n");
  return 0;
}
