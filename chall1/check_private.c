#include <stdlib.h>
#include <stdio.h>

#define FILENAME "/var/challenges/plaku/chall1/private"

int main(void)
{
    int ret;
    ret = system("ls " FILENAME);
    if(!ret)
        puts("\x1B[32mYour solution is at " FILENAME "\x1B[0m");
    else
        puts("\x1B[31mUh oh... Your solution file is gone\x1B[0m");
}
