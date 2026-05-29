#include <n7OS/sys.h>
#include <n7OS/syscall_defs.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <unistd.h>
#include <n7OS/cpu.h>
#include <n7OS/processus.h>

extern void handler_syscall();

void init_syscall() {
  // ajout de la fonction de traitement de l'appel systeme
  add_syscall(NR_example, sys_example);
  add_syscall(NR_shutdown, sys_shutdown);
  add_syscall(NR_write, sys_write);
  add_syscall(NR_wait, sys_wait);
  add_syscall(NR_sleep, sys_sleep);
  add_syscall(NR_getpid, sys_getpid);
  add_syscall(NR_getppid, sys_getppid);
  add_syscall(NR_ps, sys_ps);
  add_syscall(NR_kill, sys_kill);
  add_syscall(NR_fork, sys_fork);


  // initialisation de l'IT soft qui gère les appels systeme
  init_irq_entry(0x80, (uint32_t) handler_syscall);
}

// code de la fonction de traitement de l'appel systeme example
int sys_example() {
  // on ne fait que retourner 1
  return 1;
}

int sys_shutdown(int n) {
  if (n == 1) {
    outw(0x2000, 0x604);
    return 1;
  } else
    return n;
  
}

int sys_write(const char *s, int len) {
  console_putbytes(s, len);
  return 1;
}

int sys_wait(process_pid_t pid){
    return kernel_wait(pid);
}

int sys_sleep(int seconds) {
    return kernel_sleep(seconds);
}

process_pid_t sys_getpid(){
    return kernel_getpid();
}

process_pid_t sys_getppid(){
    return kernel_getppid();
}

void sys_ps(){
    kernel_ps();
}

int sys_kill(process_pid_t pid) {
    return kernel_kill(pid);
}

process_pid_t sys_fork(const char *name, fnptr function){
    return kernel_fork(name, function);
}
