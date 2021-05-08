#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


/*type : 잠금 종류(F_RDLCK, F_WRLCK) / offset : whence 기준 오프셋 /  whence : SEEK_SET, SEEK_CUR, SEEK_END / len : 바이트 수 */
int locks(int fd, int type, off_t offset, int whence, off_t len);
struct flock lock;

int main(int argc, char *argv[]){
        int fd, re;
        int type=0, off_set=0, whence=0, length=0;


        if (argc<2){
                fprintf(stderr, "사용법 : %s file_name", argv[0]);
                exit(0);
        }

        if((fd = open(argv[1], O_RDONLY))==-1){
                perror(argv[1]);
                exit(2);
        }

        printf("\n잠금 종류(1:RD, 2:WR), offset, whence(1:SET, 2:CUR, 3:END), len 입력:");
        scanf("%d %d %d %d", &type, &off_set, &whence, &length);

        re = locks(fd, type, off_set, whence, length);
        if(re == -1)
                printf("잠금 실패\n");
        else
                printf("잠금 성공, 현재 파일 위치 : %d\n", re);

        return 0;
}

int locks(int fd, int type, off_t offset, int whence, off_t len){

        int result;

        if(type == 1)
                lock.l_type=F_RDLCK;
        else if(type == 2)
                lock.l_type=F_WRLCK;
        else{
                printf("type error\n");
                return -1;
        }

        lock.l_start = offset;

        if(whence==1)
                lock.l_whence = SEEK_SET;
        else if(whence==2)
                lock.l_whence = SEEK_CUR;
        else if (whence == 3)
                lock.l_whence = SEEK_END;
        else{
                printf("whence error\n");
                return -1;
        }

        lock.l_len = len;

        if (fcntl(fd, F_SETLKW, &lock)==-1)
                return -1;


        result = lseek(fd, lock.l_start*len, lock.l_whence);
        return result;
}
