
#include <reent.h>
#include <errno.h>
//#include <stdlib.h> /* abort */
#include <sys/types.h>
#include <sys/stat.h>


//#include "stm32f10x.h" /* for _get_PSP() from core_cm3.h*/

#undef errno
extern int errno;
extern int  _end;

int putChar( int ch );


extern "C" {

caddr_t _sbrk(int incr) {

	  static unsigned char *heap = NULL;
	  unsigned char *prev_heap;
	  if (heap == NULL) {
		  heap = (unsigned char *)&_end;
	  }

	  prev_heap = heap;
	  heap += incr;
	  return (caddr_t) prev_heap;
}

void _exit(int status)
{
	// xprintf("_exit called with parameter %d\n", status);
	while(1) {;} // Blocca tutto!
}

int _getpid(void)
{
	return 1;
}

int _kill(int pid, int sig)
{
	pid = pid; sig = sig; /* avoid warnings */
	errno = EINVAL;
	return -1;
}

int link(char *old, char *_new) {
	return -1;
}

int _close(int file)
{
	return -1;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir) {
	return 0;
}

int _read(int file, char *ptr, int len)
{
	return 0;
}

int _write(int file, char *ptr, int len)
{
	register int i;
	for (i=0; i<len; ++i) {
//		putChar(*ptr++);
	}

	return len;
}

// extern char _end; /* Defined by the linker (c'è gia' sopra)*/
static char *heap_end;

char* get_heap_end(void)
{
	return (char*) heap_end;
}

char* get_stack_top(void)
{
//	return (char*) __get_MSP();
}

}
