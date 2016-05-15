/*
a program to test ptree
author: Dongfeng Yu
*/
#include <unistd.h>  
#include <stdio.h>
#include <sys/wait.h> 

int main(int argc, char* argv[]){
    pid_t fpid=fork();
    if(fpid<0){//error there
        printf("fork error!\n");
        exit(-1);
    }
    if(fpid==0){//child process there
        printf("5140309553Child is %d\n",getpid());
        if(execl("/data/misc/ptree","ptree",NULL)<0){
            printf("execl error!\n");
            exit(-1);
        }
            
    }else{//parent process there
        printf("5140309553Parent is %d\n",getpid());
        wait(NULL);
    }
    return 0;
}
