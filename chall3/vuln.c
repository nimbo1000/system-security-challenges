#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>


/*
 *   print out string with a '--ECHO: ' prefix
 */
void echo(char *s, unsigned int l, long int imveryimportant, short metoo)
{
  unsigned char len = (unsigned char) l;
  char errormsg[] = "argument length has to be between 40 and 100 bytes!\n";
  char buf[250] = "You typed: ";

  strcat(buf+11, s);

  if ((len > 100) || (len<40)) {
    fprintf(stderr, "%s", errormsg);
    exit(1);
  }
  else
    fprintf(stdout, "%s\n", buf);
}

/* 
 *   simple echo service that prints out its first argument
 */
int main(int argc, char **argv)
{
  gid_t r_gid, e_gid;

  /* check arguments */
  if (argc != 2) {
    fprintf(stderr, "please provide one argument to echo\n");
    return 1;
  }


  /* clear environment */ 
  clearenv();
  setenv ("PATH", "/bin:/usr/bin:/usr/local/bin", 1);
  setenv ("IFS", " \t\n", 1); 


  /* temporarily drop privileges */
  e_gid = getegid();
  r_gid = getgid();
  setregid(e_gid, r_gid);


  /* call the echo service */
  echo(argv[1], strlen(argv[1]), 0xdeadbeef, 323);

  return 0;
}

