#ifndef __UNISTD_H__
#include <n7OS/processus.h>
#define __UNISTD_H__

#define NR_example 0
#define NR_shutdown 1
#define NR_write 2
#define NR_wait 3
#define NR_sleep 4
#define NR_getpid 5
#define NR_fork 6
#define NR_getppid 7
#define NR_ps 8
#define NR_kill 9

// Fonction d'enveloppe sans argument
#define syscall0(type,name) \
type name(void) \
{ \
type __res; \
__asm__ volatile ("int $0x80" \
	: "=a" (__res) \
	: "0" (NR_##name)); \
	return __res; \
}

// Fonction d'enveloppe sans argument (retour void)
#define syscall0_void(name) \
void name(void) \
{ \
__asm__ volatile ("int $0x80" \
	: \
	: "a" (NR_##name)); \
}

// Fonction d'enveloppe 1 argument
#define syscall1(type,name,atype,a) \
type name(atype a) \
{ \
type __res; \
__asm__ volatile ("int $0x80" \
	: "=a" (__res) \
	: "0" (NR_##name),"b" (a)); \
	return __res; \
}

// Fonction d'enveloppe 2 arguments
#define syscall2(type,name,atype,a,btype,b) \
type name(atype a,btype b) \
{ \
type __res; \
__asm__ volatile ("int $0x80" \
	: "=a" (__res) \
	: "0" (NR_##name),"b" (a),"c" (b)); \
	return __res; \
}

// Fonction d'enveloppe 3 arguments
#define syscall3(type,name,atype,a,btype,b,ctype,c) \
type name(atype a,btype b,ctype c) \
{ \
type __res; \
__asm__ volatile ("int $0x80" \
	: "=a" (__res) \
	: "0" (NR_##name),"b" (a),"c" (b),"d" (c)); \
return __res;\
}

int example();

int shutdown(int n);

int write(const char *s, int len);

int wait(process_pid_t pid);

int sleep(int seconds);

process_pid_t getpid();

process_pid_t getppid();

void ps();

int kill(process_pid_t pid);


process_pid_t fork(const char *name, void *function);

#endif
