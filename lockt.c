#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

pid_t lockt(int fd, int type, off_t offset, int whence, off_t len);
struct flock lock;

int main(int argc, char *argv[]){
        int fd;
        int type, offset, whence, length;
        int result=0;

        if (argc<2){
                fprintf(stderr, "사용법 : %s file\n", argv[0]);
                exit(1);
        }

        if ((fd=open(argv[1], O_RDWR))==-1){
                perror(argv[1]);…
