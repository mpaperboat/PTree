/*
header file for syscall_ptree.c
author: Dongfeng Yu
*/

/*
linux header files and
*/ 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <asm/uaccess.h>

struct prinfo {
    pid_t parent_pid;/* process id of parent, set 0 if it has no parent*/
    pid_t pid;/* process id */
    pid_t first_child_pid;/* pid of youngest child, set 0 if it has no child */
    pid_t next_sibling_pid; /* pid of older sibling, set 0 if it has no sibling*/
    long state;/* current state of process */
    long uid;/* user id of process owner */
    char comm[64];/* name of program executed */
};
