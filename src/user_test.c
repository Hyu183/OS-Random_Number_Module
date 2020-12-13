#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_NODE "/dev/rand_numchar"

int main(){
    
    int fd, ret,res_num;
    fd = open(DEVICE_NODE,O_RDONLY);
    if(fd<0){
        perror("Can't open device\n");
        return -1;
    }
    ret = read(fd,&res_num,sizeof(res_num));
    if(ret<0){
        perror("Can't read the number from the device\n");
        close(fd);
        return -1;
    }
    else{
        printf("Here is your random number: %d\n",res_num);
        close(fd);
    }
    return 0;
}