#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h> 
#include<sys/wait.h>
#include<assert.h>
#include<pwd.h>

//宏定义
#define TRUE 1
#define MAX_HOSTNAME_LEN 100
#define MAX_PATH_LEN 1000
#define MAX_COMMEND_LEN 100

//结构体定义
typedef struct{
    char *commend[MAX_COMMEND_LEN];
    char  filename[MAX_PATH_LEN];
    int cmdnum; //参数个数
}cmdArgv;


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
cmdArgv* parsecommend(char *str,const char delimiters){
    cmdArgv *commendArgv = (cmdArgv*)malloc(sizeof(cmdArgv));
    if(commendArgv == NULL){
        //申请内存失败
        printf("申请内存失败\n");
        return NULL;
    }
    //遍历 str
    int row = 0,colum = 0,flag = 1,index = 0; //指向命令二维数组的行指针,列的指针
    for(int i=0;i<strlen(str);i++){
        //第一个分割符之前的都为命令
        if(str[i] != delimiters){
            commendArgv -> commend[row][colum] = str[i];
            if(flag == 1){
                commendArgv -> filename[colum] = str[i];
                index = colum;
            }
            colum++;
            
        }
        else{
            commendArgv -> commend[row][colum] = '\0';
            row++;
            colum = 0;
            flag = 0;
            commendArgv -> cmdnum++;
        }

    }
    commendArgv -> cmdnum = commendArgv -> cmdnum+1;//命令数等于空格数加一
    commendArgv -> filename[++index] = '\0';
    //命令数组最后一行加一个NULL
    commendArgv -> commend[row+1] = NULL;
    return commendArgv;

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
        cmdArgv* parsecmd = parsecommend(commend,' ');
        printf("%d",parsecmd->cmdnum);
        for(int i=0;i<parsecmd -> cmdnum;i++){
            for(int j=0;j<strlen(parsecmd -> commend[i]);j++){
                printf("%c",parsecmd -> commend[i][j]);
            }
            printf("\n");
        }
        //执行命令
        char filepath[MAX_PATH_LEN] = "/bin/";
        strcat(filepath,parsecmd -> filename);
        printf("%s\n",filepath);
        int status;
        if(fork() !=0){
            waitpid(-1,&status,0);
        }
        else{
            execve(filepath,parsecmd -> commend,NULL);
        }
        commend[0] = '\0';
    }


    return 0;
}