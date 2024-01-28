/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

// this is for use in syssec course only
// no guraenties of beeing bug free (quite the opposite ;) )
// Don't share this code. Don't use it for other purpose than this exercise. 
// There are some really ugly hacks that are there just there to make
// the program easier to exploit !


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <unistd.h>
#include <sys/wait.h>


#include <pwd.h>


#define BUFFLEN 512
#define MAXPATH 100

int SocketFD;
size_t stClient_len;

void get_privileged_info();
int UDP_process(void);
void process_cmds( void);
void diep(char *s);
void handler(int sig);
void doit();


int justup=1;
int *admin;
char pass[100];
int port;


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




int main(int argc, char **argv){
    char chrootpath[MAXPATH];

    if( argc < 4){
        printf("Usage %s <chrootpath> <port> <passwd> <username>\n",argv[0]);
        return -1;
    }
    


    // PATH
    if(strlen(argv[1])>MAXPATH){
        printf("[C] path too long\n");
        exit(EXIT_FAILURE);
    }
    strncpy(chrootpath,argv[1],MAXPATH);
    // force null termination 
    chrootpath[MAXPATH]=0;
    
    // PORT
    port=atoi(argv[2]);

    // Pasword
    strncpy(pass,argv[3],100);
    


        
    // get uid/gid information
    struct passwd *pw;
    int uid, gid;     
    pw=getpwnam(argv[4]);
    
    
    if (pw==0){
        printf("no such user %s, fall back to uid/gid 65534/65534\n",argv[4]);
        uid=65534;
        gid=65534;
    }
    else{
        uid=pw->pw_uid;
        gid=pw->pw_gid;
    }
    
    printf("[C] starting to chroot exec server %s into %s\n", 
           argv[0], chrootpath);
    if(chdir(chrootpath)!=0){
        printf("[X] chdir failed with %s\n",chrootpath);
        perror("[X] chdir");
        exit(EXIT_FAILURE);
    }
        
    if(chroot(chrootpath)!=0){
        printf("[X] chroot failed with %s\n",chrootpath);
        perror("[X] chroot");
        exit(EXIT_FAILURE);
    }


    printf("[C] switching to user : %s; uid %d gid %d \n",argv[4], uid, gid);
    
    //Drop privileges to nobody  
    if(setgid(gid)!=0){
        perror("[X] failed to set gid to nogroup");
        exit(EXIT_FAILURE);
    }
    if(setuid(uid)!=0){
        perror("[X] failed to set uid to nobody");
        exit(EXIT_FAILURE);
    }
    
    //if( setgid(getegid()) ) perror( "setgid" );
    //if( setuid(geteuid()) ) perror( "setuid" );
         



    // Listen to the network       
    UDP_process();
}

int UDP_process(void)
{
    int is_admin=0xDEADBEEF;
    char hint2[]="this looks like part of the stack of function UDP_process !!!";

    struct sockaddr_in stSockAddr;
    memset(&stSockAddr, 0, sizeof(stSockAddr));

    admin=&is_admin;

    SocketFD = socket(PF_INET, (int) SOCK_DGRAM, IPPROTO_UDP);
 
    if(-1 == SocketFD) {
        perror("[X] Can not create socket");
        exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(stSockAddr));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port);
    stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
 
    if(-1 == bind(SocketFD,(struct sockaddr *)&stSockAddr, sizeof(stSockAddr))) {
        perror("[X] Error bind failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    
    signal(SIGFPE, &handler);
    signal(SIGILL, &handler);
    signal(SIGINT, &handler);
    signal(SIGSEGV, &handler);
    signal(SIGTERM, &handler);


    // Start process requests 
    for(;;)  process_cmds();

    return EXIT_SUCCESS;  
}


#define len BUFFLEN
struct sockaddr_in *stClient;
int bytes_read;
int err;
char obuff[BUFFLEN];
char ibuff[BUFFLEN];

void process_cmds( void){

    stClient=malloc(sizeof(struct sockaddr_in));

    if (stClient==NULL){
        printf("allocation failed\n");
        exit(EXIT_FAILURE);
    }

    memset(stClient, 0, sizeof(struct sockaddr_in));

    /* perform read write operations ... */
    printf("== read from socket \n");
    
    stClient_len=sizeof(struct sockaddr);
    
    memset(obuff, 0, BUFFLEN);
    memset(ibuff, 0, BUFFLEN);
    
    bytes_read=recvfrom(SocketFD, ibuff, len, 0,
                        ( struct sockaddr *) stClient, (socklen_t *)&stClient_len );
    printf("[-] bytes_read %d data: %s\n", bytes_read,ibuff);
    
    doit();
}



void doit(){
    //might help: DEFCON-18-Haas-Adv-Format-String-Attacks.pdf
    char hint2[]="this looks like part of the stack of function process_cmds !!!";
    char ibuffloc[BUFFLEN];
    memcpy(ibuffloc,ibuff,BUFFLEN);
    
    if(justup==1){
        sprintf (obuff,"I'm now waking up... Gimmie a bunch of seconds ! \n (BTW, your command was %s)",ibuff);
        // check password
        sprintf(obuff,"string (%s)", ibuff);
        // This is stupid as this would allow anyone to obtain other student's pin ;)
        //" and goodpass (%s) \n",ibuff, pass);
        
        if (strcmp(ibuff,pass)==0)
            justup=0;
    } else {
        // TODO program crashes sometimes here...  fix before release
        snprintf(obuff,BUFFLEN,ibuff,hint2);
        printf("[-] writing %s\n",obuff);
    }
    
    if(*admin==0xCAFED0DD)
        get_privileged_info();
    else{        
        err = sendto(SocketFD, 
                     obuff, sizeof(obuff), 
                     0, (struct sockaddr *)stClient, stClient_len);
    }
    if( err ==-1)
        diep("[X] sendto()");
}



void get_privileged_info(){

    int i;
    char rc;
    char *fname="private";
    char buff[100];

    FILE *fp = fopen(fname,"r");
    if (fp == NULL) {
        snprintf(buff,100,"Failed to open file %s",fname);
        perror(buff);
        err = sendto(SocketFD, 
                     obuff, sizeof(obuff), 
                     0, (struct sockaddr *)stClient, stClient_len);
    }

    for (i = 0; (rc = getc(fp)) != EOF && i<BUFFLEN-200; obuff[200+i++] = rc) { }
    err = sendto(SocketFD, 
                 obuff, sizeof(obuff), 
                 0, (struct sockaddr *)stClient, stClient_len);

}
