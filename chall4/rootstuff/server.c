/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

// this is for use in syssec course only
// no guraenties of beeing bug free (quite the opposite ;) )
// please don't share this code.


#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <unistd.h>
#include <sys/wait.h>

#define BUFFLEN 512 

int SocketFD;
size_t stClient_len;

void diep(char *s);
void handler(int sig);
int runcmd(char *cmd, char *path, char *port, char* pass, char **envp);

void diep(char *s)
{
    perror(s);
    exit(1);
}

void handler(int sig){
    printf("[X] bye (got sig %d)\n",sig);
    close(SocketFD);
    exit(-1);
}



#define MAXPATH 100

int main(int argc, char **argv, char ** envp ){
    char slave[200]="";

    if( (argc==1 || argc > 4) && strcmp(argv[1],"cli")==0){
        printf("Usage %s <chrootpath> <port> <passwd> \n",argv[0]);
        return -1;
    }
    strncat(slave,argv[1],200);    
    strncat(slave,"/server_slave",200);
    if (argc>1 && strcmp(argv[1],"cli")!=0 ){

        // this must be the server process
        printf("[M] I'm the master !\n");
        
        while (1){
            runcmd(slave,argv[1],argv[2],argv[3],envp);
            usleep(100000); // 100ms before restart  ...
        }
        exit(EXIT_SUCCESS);
    }
}


int runcmd(char *cmd, char *path, char *port, char* pass, char **envp ) {

    char *argv[]={cmd, 
                  NULL, 
                  port, 
                  pass,
                  "secretchall4",
                  "secretchall4",
                  NULL };
    
    pid_t child_pid;
    int child_status;
    char chrootpath[200]="";

    strncat(chrootpath,path,200);    
    strncat(chrootpath,"/chroot",200);    
    argv[1]=chrootpath;

    child_pid = fork();
    
    if(child_pid == 0) {
        /* This is done by the child process. */
        printf("[C] now child executes %s %s %s %s \n",argv[0],argv[1], argv[2], argv[3]);


        envp = 0; /* blocks IFS attack on non-bash shells */

        // use execvp to force re-randomisation of memory layout on every execution (i.e. crash)
        execvp(cmd, argv);
        
        /* If execvp returns, it must have failed. */        
        printf("[C] Dammit !! execvp failed  ! (was cmd=%s\n",cmd); 
        exit(0);
    }
    else {
        // parent monitors the child, wait for the child to die.
        pid_t tpid;
        do {
            tpid = wait(&child_status);
        } while(tpid != child_pid);
        usleep(10000); // 10ms before restart not very nice ...
        printf("[-] Meee ... sauvages m'ont tout cassé !\n");      

        return child_status;
    }
}
