#include<stdio.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char* argv[]) {
  FILE* fp;
  char* file_name;
  char c;
  if (argc == 1) {
    fprintf(stderr, "[X] Error using cat... \n");
    return 2;
  }

  file_name=argv[1];
  printf("[-] calling 'access'\n");
  if(!access(file_name, R_OK)){
    printf("[-] access granted\n");
    fp = fopen(file_name, "r");
    if (fp == NULL) {
      fprintf(stderr, "[X] File not found...Aborting \n");
      return 2;
    }
    printf("[-] file opened\n");
    while ((c = fgetc(fp)) != EOF)
      fputc(c, stdout);
    fclose(fp);
  }else{
    fprintf(stderr,"[X] suid_cat: %s: Permission denied\n",file_name);
    return 2;
  }

  return 0;
}
