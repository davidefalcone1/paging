#include <types.h>
#include <kern/unistd.h>
#include <clock.h>
#include <copyinout.h>
#include <syscall.h>
#include <limits.h>
#include <lib.h>
#include "opt-open.h"
#include <uio.h>
#include <vnode.h>
#include <vfs.h>

struct vnode *file_table[OPEN_MAX];

int sys_write(int fd, const void *buff, int count)
{	
#if OPT_OPEN
	struct iovec iov;
	struct uio ku;
	char *p;
	int result, chars = 0;

	p = (char *)buff;
	if(fd < 3 && file_table[fd] == NULL){
		while(chars<count){
			putch(p[chars]);
			chars++;
		}

		return chars;
	}
	else {
		uio_init(&iov, &ku, (void *) buff, count, 0, UIO_READ, 0);
		result = VOP_WRITE(file_table[fd], &ku);
		if(result)
			return -1;
		return ku.uio_offset;
	}
	
#else
	char *p = (char *)buff;
	if(fd != STDOUT_FILENO && fd != STDERR_FILENO){
		kprintf("sys_write supports only stdout and stderr\n");
		return -1;
	}	
	
	int chars=0;
	while(chars<count){
		putch(p[chars]);
		chars++;
	}

	return chars;
#endif
}

int sys_read(int fd, const void *buff, int count)
{	
#if OPT_OPEN
	struct iovec iov;
	struct uio ku;
	char *p;
	int result, chars = 0;

	p = (char *)buff;
	if(fd < 3 && file_table[fd] == NULL) {
		while(chars<count){
			p[chars] = getch();
			chars++;
		}

		return chars;
	}
	else {
		uio_init(&iov, &ku, (void *) buff, count, 0, UIO_READ, 0);
		result = VOP_READ(file_table[fd], &ku);
		if(result)
			return -1;
		return ku.uio_offset;
	}
	

#else
	char *p = (char *)buff;
	if(fd != STDIN_FILENO){
		kprintf("sys_read supports only stdin\n");
		return -1;
	}
		
	int chars=0;
	while(chars<count){
		p[chars] = getch();
		chars++;
	}

	return chars;
#endif
}
#if OPT_OPEN
int get_first_free(){
	int i = 3;
	while(i < OPEN_MAX){
		if(file_table[i] == NULL)
			return i;
		i++;
	}
	return -1;
}

int sys_open(const char *pathname, int flags, mode_t mode){
	struct vnode *v;
	int fd, result;
	
	result = vfs_open((char *)pathname, flags, mode, &v);
	if(result) 
		return result;
	fd = get_first_free();
	file_table[fd] = v;
	return fd;
}

int sys_close(int fd){
	KASSERT(file_table[fd] != NULL);
	file_table[fd] = NULL;
	return 0;
}

#endif

