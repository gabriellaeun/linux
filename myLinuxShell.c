#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#define MAX 10

const char *prompt = "myLinuxShell>";
char* cmdvector[MAX];
char cmdline[BUFSIZ];
pid_t pid=-1;

void fatal(char *str){
        perror(str);
        exit(1);
}

int makelist(char *s, const char *limiters, char** list, int MAX_LIST){
        int i=0;
        int numtokens=0;
        char *new=NULL;

        if((s==NULL)||(limiters==NULL)) return -1;

        new=s+strspn(s, limiters);
        if((list[numtokens]=strtok(new, limiters))==NULL)
                return numtokens;
        numtokens=1;

        while(1){
                if((list[numtokens]=strtok(NULL, limiters))==NULL) break;
                if(numtokens==(MAX_LIST-1))return -1;
                numtokens++;
        }
        return numtokens++;
}


void cmd_cd(int argc, char** argv){
        if(argc==1) //cd만 입력받은 경우 home directory로 이동
                chdir(getenv("HOME"));
        else if(argc==2){ //cd dir : dir로 이동
                chdir(argv[1]);
                //else
                //      printf("No directory\n");
        }
        else
                printf("error\n");
}

void sighandler(int signal){
        if(pid==0){
void sigchldhandler(int signal){
        wait(NULL);
}

void execute_cmdline(char* cmdline){
        int count=0;
        int i=0;

        count=makelist(cmdline, "\t", cmdvector, MAX);
        if(count==0) return;

        if(strcmp("exit", cmdvector[0])==0)
                exit(1);//exit 입력 시 Shell 종료
        else if(strcmp("cd", cmdvector[0])==0)
                cmd_cd(count, cmdvector);//명령어 cd이면, fork() 실행 전에 디렉>토리 변경
        else if(strcmp("&", cmdvector[count-1])==0){ //마지막 글자 &면
                cmdvector[count-1]=NULL; //명령어 실행 위해 & 제거
                switch(pid=fork()){
                        case 0:
                                execvp(cmdvector[0], cmdvector);
                                fatal("main()");
                        case -1:
                                fatal("main()");
                        default:
                                signal(SIGCHLD, sigchldhandler);
                }
        }
        else{
                switch(pid=fork()){
                        case 0:
                                execvp(cmdvector[0], cmdvector);
                                fatal("main()");
                        case -1:
                                fatal("fork error");
                        default:
                                wait(NULL);
                                fflush(stdout);
                }
        }
}

int main(int argc, char** argv){
        int i=0;

        signal(SIGINT, sighandler);
        signal(SIGQUIT, sighandler);

        while(1){
                fputs(prompt, stdout); //모니터로 "myshell>" 문자열 출력 함수
                fgets(cmdline, BUFSIZ, stdin);
                cmdline[strlen(cmdline)-1]='\0';

                execute_cmdline(cmdline);
        }
        return 0;
}
