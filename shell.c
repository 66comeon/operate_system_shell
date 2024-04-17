#include<unistd.h>
#include<stdio.h>
#include<pwd.h>
#define TRUE 1
#define NUM 100
#define MAX_CMD_LEN 30
struct user_info{
	char *user_name;
	char *user_passwd;
	__uid_t pw_uid;
	__gid_t pw_gid;
	char *pw_gecos;
	char *pw_dir;//用户目录
	char *pw_shell;
	char *host_name;
	char *user_authen;//用户权限
};
typedef struct{
	char *argv[MAX_CMD_LEN];
	int arg_num;
}cmdArgV;

char command[100];


void type_promot()
{
	struct user_info user = {"user1","123456",1,1,"user1","~","/bin/bash","ubuntu","$"};
	printf("%s@%s:%s%s",user.user_name,user.host_name,user.pw_dir,user.user_authen);
}

cmdArgV getPasedCmd(char *command)
{
	cmdArgV parsedCmd;

}
int main()
{
	while(TRUE)
	{
		//print a tip
		type_promot();
		fgets(command,100,stdin);
		//input a commant
		


		//excute a commant
	}
	return 0;
}
