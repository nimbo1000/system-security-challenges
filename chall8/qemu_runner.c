#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {

    int i;
    char cmd_buf[512];
    char secret_buf[256];
    FILE *fp;
    
    memset(cmd_buf, '\0', 512);
	
    fp = fopen("./private", "r");
    if(!fp){
        perror("Unable to open private file");
        return -1;
    }
    /* Assumption of format for private: 3 lines, last has the token */
    for(i=0; i<3; i++)
        fgets(secret_buf, 255, fp);
    fclose(fp);

    strtok(secret_buf, "\n");

	strcat(cmd_buf, "qemu-system-i386 -enable-kvm -hda squeeze.qcow2 -m 64 -kernel ./bzImage -serial stdio -monitor /dev/null -nographic -snapshot -append \"root=/dev/sda rw quiet console=ttyS1 token=");
    strncat(cmd_buf, secret_buf, 255);
    strcat(cmd_buf, "\"");

    puts("Starting QEMU - This may take a while...");

    system(cmd_buf);

    return 0;
}

