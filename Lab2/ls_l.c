#include<stdio.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<grp.h>
#include<time.h>
#include<pwd.h>
#include<fcntl.h>
#include<stdlib.h>
void timet_to_tm(const struct stat * get_message)
{
    struct tm * chtm = localtime(&(get_message->st_mtime));
    if(chtm == NULL){
        printf("localtime is error");
        exit(0);
    }
    else
        printf("%d月 %d  ",chtm->tm_mon+1,chtm->tm_mday);/*tm_mon属于[0,11]*/

    if(chtm->tm_hour < 10)/*0~9的数要写成0X格式*/
        printf("0");/*先打印0*/
    printf("%d:",chtm->tm_hour);/*再打印X*/
    if(chtm->tm_min < 10)
        printf("0");
    printf("%d  ",chtm->tm_min);
}
void print_type(const struct stat * get_message)
{
    mode_t mode = (*get_message).st_mode;
    if(S_ISREG(mode))
        printf("-");/*普通文件*/
    else if(S_ISDIR(mode))
        printf("d");/*目录文件*/
    else if(S_ISCHR(mode))
        printf("c");/*字符设备文件*/
    else if(S_ISBLK(mode))
        printf("b");/*块设备文件*/
    else if(S_ISFIFO(mode))
        printf("p");/*管道文件*/
    else if(S_ISLNK(mode))
        printf("l");/*链接文件*/
    else if(S_ISSOCK(mode))
        printf("s");/*socket文件*/

}
void file_power_char(mode_t mode,mode_t type,const char ch)
{
    if((mode & type) == type)
        printf("%c",ch);
    else
        printf("-");
}
void id_to_string(const struct stat * get_message)
{
    struct passwd * pwd;/*根据用户id获取用户名*/
    pwd = getpwuid(get_message->st_uid);
    printf("%s ",pwd->pw_name);

    struct group * grp;/*根据组id获取组名*/
    grp = getgrgid(get_message->st_gid);
    printf("%s ",grp->gr_name);

}
void file_power(const struct stat * get_message)
{
    mode_t mode = (*get_message).st_mode & 07777;/*取后四位*/

    file_power_char(mode,S_IRUSR,'r');/*判断user有无读权限*/
    file_power_char(mode,S_IWUSR,'w');/*判断user有无写权限*/
    file_power_char(mode,S_IXUSR,'x');/*判断user有无可执行权限*/

    file_power_char(mode,S_IRGRP,'r');/*判断group有无读权限*/
    file_power_char(mode,S_IWGRP,'w');/*判断group有无写权限*/
    file_power_char(mode,S_IXGRP,'x');/*判断group有无可执行权限*/

    file_power_char(mode,S_IROTH,'r');/*判断other有无读权限*/
    file_power_char(mode,S_IWOTH,'w');/*判断other有无写权限*/
    file_power_char(mode,S_IXOTH,'x');/*判断other有无可执行权限*/

    printf(" ");
}
void list_message(const struct stat * get_message)/*处理读取到的文件*/
{
    print_type(get_message);/*判断并打印文件类型*/
    file_power(get_message);/*判断并打印文件权限*/
    printf("%ld ",get_message->st_nlink);/*打印硬链接数*/
    id_to_string(get_message);/*用户id与组id转换成用户名与组名并打印*/
    printf("%ld\t",get_message->st_size);/*打印所占空间文件大小*/
    timet_to_tm(get_message);/*将GMT时间的秒数转换成标准时间格式输出*/
}
int main(){
	char FilePwd[256]={};
	int Size;
	struct stat StatBuf;
	getcwd(FilePwd,256);
	DIR *DirP=opendir(FilePwd);
	strcat(FilePwd,"/");
	Size = strlen(FilePwd);
	struct dirent* DIR;
	while((DIR=readdir(DirP))!=NULL){
		FilePwd[Size] = 0;
		printf("%s\t",DIR->d_name);
		strcat(FilePwd,DIR->d_name);
		stat(FilePwd,&StatBuf);
		list_message(&StatBuf);
		printf("\n");
	}
	return 0;
}
