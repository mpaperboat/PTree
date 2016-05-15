/*
print the process tree
author: Dongfeng Yu
*/
#include "ptree.h"

struct prinfo* d;//store process from syscall_ptree

//linked list for hash table
//in order to have a satisfying performance
//hash table is used to get a process by pid
struct list_node{
    struct prinfo*p;
    struct list_node*next;
}*pid_to_proc[99971];

//insert a process to hash table
void insert_proc(struct prinfo*p){
    //printf("pid:%d inserted\n",p->pid);
    int t=p->pid%99971;
    struct list_node*n=malloc(sizeof(struct list_node));
    if(n==0){
        printf("malloc error!\n");
        exit(-1);
    }
    n->p=p;
    n->next=pid_to_proc[t];
    pid_to_proc[t]=n;
}

//find a process in hash table
struct prinfo*find_proc(int pid){
    //printf("begin%d\n",pid);
    int t=pid%99971;
    struct list_node*n=pid_to_proc[t];
    //printf("address%d,pid:%d,name:%s\n",(int)n->p,n->p->pid,n->p->comm);
    while(n->p->pid!=pid&&n->p->pid!=-pid){
        n=n->next;
        //printf("address%d,pid:%d,name:%s\n",(int)n->p,n->p->pid,n->p->comm);
    }
    //printf("end\n");
    return n->p;
}

//dfs search process tree
void dfs(struct prinfo*p,int t){
    //printf("dfs start\n");
    if(p->pid<0)
        return;//already visited
    int i;
    for(i=0;i<t;++i)
        printf("\t");
    printf("%s,%d,%ld,%d,%d,%d,%d\n",p->comm,p->pid,p->state,p->parent_pid,p->first_child_pid,p->next_sibling_pid,p->uid);
    p->pid*=-1;
    //printf("[11]%s\n",p->comm);
    if(p->first_child_pid!=0)
        dfs(find_proc(p->first_child_pid),t+1);
    //printf("[22]%s\n",p->comm);
    if(p->next_sibling_pid!=0)
        dfs(find_proc(p->next_sibling_pid),t);
    //printf("[33]%s\n",p->comm);
}

int main(int argc, char* argv[]){
    int n=1,i,t;
    d=malloc(sizeof(struct prinfo));
    if(d==0){
        printf("malloc error!\n");
        exit(-1);
    }
    //using doubling algorithm there
    //because we don't know how much space should be allocated
    while((t=syscall(356,d,&n))>n){
        free(d);
        n=n*2;
        d=malloc(sizeof(struct prinfo)*n);
        if(d==0){
            printf("malloc error!\n");
            exit(-1);
        }
    }
    //printf("%d\n",t);
    /*for(i=0;i<t;++i){
        struct prinfo p=d[i];
        printf("%s,%d,%ld,%d,%d,%d,%d\n",p.comm,p.pid,p.state,p.parent_pid,p.first_child_pid,p.next_sibling_pid,p.uid);
    }*/
    //initialize hash table
    for(i=0;i<t;++i)
        insert_proc(d+i);
    for(i=0;i<t;++i)
        if(d[i].parent_pid==0)
            dfs(d+i,0);
    return 0;
}
