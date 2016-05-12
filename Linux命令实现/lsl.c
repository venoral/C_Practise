#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>
#include<dirent.h>


void show_stat_info(char *fname, struct stat *buf);
char * trans_time(const time_t *timep);
void trans_mode(int mode, char str[]);
char * trans_owner(uid_t uid);
char * trans_gid(gid_t gid);



int main(int ac, char *av[]){
   struct stat infobuf; 
    
   if(ac>1){
     if(stat(av[1], &infobuf) != -1){
        show_stat_info(av[1], &infobuf);
        return 0; 
     }else{
        
        perror(av[1]); 
     }
   } 
   return 1;
}

void show_stat_info(char *fname, struct stat *buf){
  char str[10];
  trans_mode(buf->st_mode, str);
  printf("%s", str); //mode
  printf("%4d", buf->st_nlink); //links 
  printf("%-8s", trans_owner(buf->st_uid));// owner
  printf("%-8s", trans_gid(buf->st_gid)); // group
  printf("%-8ld", buf->st_size);// size
  printf("%.12s", trans_time(&buf->st_mtime));// time
  printf("%s\n", fname); //filename
}

char * trans_time(const time_t *timep){
  char *str;
  str = ctime(timep); //返回的是字符串的首地址
  int len = strlen(str);
  char s = str[len-1];
  //此处换掉换行字符为Null字符\0，修正输出格式
  if(s == '\n'){
    str[len-1] = '\0';
   // printf("替换成功");
  }
  return  str;
}

/*转换文件类型和权限*/
void trans_mode(int mode, char str[]){
  
  strcpy(str, "----------");
  if(S_ISDIR(mode)) str[0] = 'd';
  if(S_ISCHR(mode)) str[0] = 'c';
  if(S_ISBLK(mode)) str[0] = 'b';
  
  if(mode & S_IRUSR) str[1] = 'r';
  if(mode & S_IWUSR) str[2] = 'w';
  if(mode & S_IXUSR) str[3] = 'x';

  if(mode & S_IRGRP) str[4] = 'r';
  if(mode & S_IWGRP) str[5] = 'w';
  if(mode & S_IXGRP) str[6] = 'x';
  
  if(mode & S_IROTH) str[7] = 'r';
  if(mode & S_IWOTH) str[8] = 'w';
  if(mode & S_IXOTH) str[9] = 'x'; 
}

/*文件所有者的转换*/
char * trans_owner(uid_t uid){
  return getpwuid(uid)->pw_name;
}

/*文件组的转换*/
char *trans_gid(gid_t gid){
  return getgrgid(gid)->gr_name;
}
