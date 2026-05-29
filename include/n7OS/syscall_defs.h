#ifndef __SYSCALL_DEFS_H__
#define __SYSCALL_DEFS_H__

#include <n7OS/processus.h>

#define NB_SYSCALL 10

int sys_example();
int sys_shutdown();
int sys_write();
int sys_wait();
int sys_sleep();
process_pid_t sys_getpid();
process_pid_t sys_getppid();
void sys_ps();
int sys_kill();
process_pid_t sys_fork();

typedef int (*fn_ptr)();
extern fn_ptr syscall_table[NB_SYSCALL];

void add_syscall(int num, fn_ptr function);

#endif
