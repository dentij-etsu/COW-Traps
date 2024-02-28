#include "types.h"
#include "riscv.h"
#include "param.h"
#include "spinlock.h"
#include "defs.h"
#include "sysinfo.h"
#include "proc.h"

int systeminfo(uint64 addr) {
	struct proc *p = myproc(); // Stole these right out of filestat.c
	struct sysinfo sys_info;
	
	sys_info.freemem = freememory();
	sys_info.nproc = numproc();
	
	if(copyout(p->pagetable, addr, (char *)&sys_info, sizeof(sys_info)) < 0)
		return -1;
    return 0;
}