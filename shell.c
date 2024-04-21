#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h> 
#include<sys/wait.h>
#include<pwd.h>

//宏定义
#define TRUE 1
#define MAX_HOSTNAME_LEN 100
#define MAX_PATH_LEN 1000
#define MAX_COMMEND_LEN 100

//结构体定义
struct commend{
    char *commend[MAX_COMMEND_LEN];
    char  filename[MAX_PATH_LEN];
    int cmdnum; //参数个数
};

typedef struct commend* cmdArgv;

//函数定义
void type_promote(){
    struct passwd * pwd;
    char hostname[MAX_HOSTNAME_LEN];//主机名
    char pathname[MAX_PATH_LEN];//当前路径

    pwd = getpwuid(getuid());
    getcwd(pathname,MAX_PATH_LEN);//获得当前工作目录
    gethostname(hostname,MAX_HOSTNAME_LEN);//获得当前主机名
    printf("%s@%s:%s#",pwd->pw_name,hostname,pathname);

}

//命令分解
cmdArgv getParsedCmdArgv(char *str,const char* delimiters){
    cmdArgv parsedCmdArgv = (cmdArgv)malloc(sizeof(struct commend));
    if(parsedCmdArgv == NULL){
        //申请内存失败
        printf("申请内存失败\n");
        return NULL;
    }
    //通过strtok分解命令
    int i = 0;
    //获得命令部分
    char* token = strtok(str,delimiters);
    if(strcmp(token,"cd") == 0){
        //该命令为内部命令，直接执行
        token = strtok(NULL,delimiters);
        if(strcmp(token,"~") == 0){
            strcpy(token,"/home/lb");
        }
        chdir(token);
        return NULL;
    }
    if(strcmp(token,"exit") == 0){
        //内部命令直接执行exit系统调用
        asm("movl $1,%eax\n\t"
                "movl $0,%ebx\n\t"
                "int $0x80");
    }
    parsedCmdArgv -> commend[i++] = token;
    char filePath[MAX_PATH_LEN] = "/bin/";
    //构造命令所在的文件
    strcat(filePath,token);
    strcpy(parsedCmdArgv -> filename,filePath);

    while(token){
        token = strtok(NULL,delimiters);
        if(token == NULL){
            break;
        }
        parsedCmdArgv -> commend[i++] = token;
    }
    parsedCmdArgv -> commend[i] = NULL;
    parsedCmdArgv -> cmdnum = i;
    return parsedCmdArgv;
}


int main()
{
    
    while(TRUE){
        //打印命令提示符
        type_promote();
        //输入命令
        char commend[MAX_COMMEND_LEN];
        fgets(commend,MAX_COMMEND_LEN,stdin);
        commend[strlen(commend)-1] = '\0';
        //分解命令
        cmdArgv parsecmd = getParsedCmdArgv(commend," ");
        
        
        //执行命令
        int status;
        if(fork() !=0){
            waitpid(-1,&status,0);
        }
        else{
            execve(parsecmd -> filename,parsecmd -> commend,NULL);
        }
    }


    return 0;
}