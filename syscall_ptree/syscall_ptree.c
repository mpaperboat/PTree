/*
ptree system call for getting all process information
author: Dongfeng Yu
*/
#include "syscall_ptree.h"

/*
may cause errors without a license
*/
MODULE_LICENSE("Dual BSD/GPL");

/*
system call number
*/
#define __NR_ptree 356

/*
backup old call
*/
static int (*oldcall)(void);

/*
main body of the system call
*/
int ptree(struct prinfo* buf, int *nr){
    struct task_struct *p, *k;
    int cnt=0,tnr;
    if(copy_from_user(&tnr,nr,sizeof(int))!=0){
        printk(KERN_INFO"copy_from_user error!\n");
        return -1;
    }
    //read_lock(&tasklist_lock);
    //lock task list
    rcu_read_lock();
    for_each_process(p){
        if(cnt<tnr){
            struct prinfo t;
            t.parent_pid=p->parent->pid;
            t.pid=p->pid;
            if(p->children.next==&p->children)
                t.first_child_pid=0;
            else
                t.first_child_pid=list_entry(p->children.next,struct task_struct,sibling)->pid;
            if(p->sibling.next==p->parent->children.next)
                t.next_sibling_pid=0;
            else
                t.next_sibling_pid=list_entry(p->sibling.next,struct task_struct,sibling)->pid;
            if(t.next_sibling_pid==1)
                t.next_sibling_pid=0;
            t.state=p->state;
            t.uid=p->cred->uid;
            strcpy(t.comm,p->comm);
            if(copy_to_user(buf+cnt,&t,sizeof(t))!=0){
                printk(KERN_INFO"copy_to_user error!\n");
                return -1;
            }
                
        }
        ++cnt;
    }
    //read_unlock(&tasklist_lock);
    if(cnt<tnr)
        if(copy_to_user(nr,&cnt,sizeof(int))!=0){
            printk(KERN_INFO"copy_to_user error!\n");
            return -1;
        }
    rcu_read_unlock();
    return cnt;
}

/*
put sys_tree to syscall
*/
static int addsyscall_init(void){
    long*syscall=(long*)0xc000d8c4;
    oldcall=(int(*)(void))(syscall[__NR_ptree]);
    syscall[__NR_ptree]=(unsigned long)ptree;
    printk(KERN_INFO"module load!\n");
    return 0;
}
module_init(addsyscall_init);

/*
restore old call
*/
static void addsyscall_exit(void){
    long*syscall=(long*)0xc000d8c4;
    syscall[__NR_ptree]=(unsigned long)oldcall;
    printk(KERN_INFO"module exit!\n");
}
module_exit(addsyscall_exit);
