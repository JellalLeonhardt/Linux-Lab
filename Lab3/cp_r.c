#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
void FileCopy(char StartDir[],char TargetDir[],char FileName[]){
	char From[251];
	char To[251];
	char *buf;
	int count;
	buf = (char *)malloc(10000);
	sprintf(From,"%s/%s",StartDir,FileName);
	sprintf(To,"%s/%s",TargetDir,FileName);
	FILE *ffp = fopen(From,"r");
	FILE *tfp = fopen(To,"wb");
	count = fread(buf,1,10000,ffp);
	fwrite(buf,1,count,tfp);
	fclose(ffp);
	fclose(tfp);
	free(buf);
}
int CopyDir(DIR *OriginDir,char StartDir[],char TargetDir[]){
	struct dirent *OriDir;
	char target[251];
	char origin[251];
	int flag;
	DIR * NextOriginDir;
	struct stat *buf =(struct stat*)malloc(sizeof(struct stat));
	while((OriDir = readdir(OriginDir))!=NULL){
		sprintf(target,"%s/%s",TargetDir,OriDir->d_name);
		sprintf(origin,"%s/%s",StartDir,OriDir->d_name);
		printf("Direction:%s\n",target);
		lstat(origin,buf);
		if(S_ISDIR(buf->st_mode) && OriDir->d_name[0]!='.'){
			NextOriginDir = opendir(origin);
			mkdir(target,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
			flag=CopyDir(NextOriginDir,origin,target);
		}
		else if(OriDir->d_name[0]!='.'){
			FileCopy(StartDir,TargetDir,OriDir->d_name);
		}
	}
	free(buf);
	return 1;
}		
int main(int args,char *argv[]){
	DIR *OriginDir;
	int flag = 0;
	int count;
	char RealTarget[251];
	struct stat *buf =(struct stat*)malloc(sizeof(struct stat));
	if(args == 3){
		printf("%s\n",argv[1]);
		printf("%s\n",argv[2]);
		OriginDir = opendir(argv[1]);
	}
	else{
		printf("Wrong arguments.\n");
		return 1;
	}
	if(access(argv[1],0)==-1){
		printf("源文件/目录不存在.\n");
	}
	char *l;
	char *p = argv[1];
	l = p;
	while(*p!='\0'){
		if(*p == '/' && *(p+1)!='\0'){ //防止末尾的斜杠
			l = p;
		}
		p++;
	}
	l++;
	sprintf(RealTarget,"%s/%s",argv[2],l);
	lstat(argv[1],buf);
	if(S_ISDIR(buf->st_mode)){
		printf("test.\n");
		if(access(argv[2],0)==-1){
		mkdir(argv[2],S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
		}
		mkdir(RealTarget,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
		printf("%s\n",RealTarget);
		flag = CopyDir(OriginDir,argv[1],RealTarget);
	
	}
	else{
		FILE *ffp = fopen(argv[1],"r");
		FILE *tfp = fopen(RealTarget,"wb");
		count = fread(buf,1,10000,ffp);
		fwrite(buf,1,count,tfp);
		fclose(ffp);
		fclose(tfp);
		free(buf);
	}
	if(flag){
		printf("Copy over.\n");
	}
	return 0;
}
