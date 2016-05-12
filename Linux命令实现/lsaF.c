
#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>


static char *array[20];
static int i = 0;

void do_ls(char []);
void getMode(char *fname, int i);
void AlphaSort(char *array[], int size);
void printf_list(char *array[], int size);
int file_dir(char *fname);


int main(int ac, char *av[]){
 if(ac == 1){
   printf("默认当前目录.: \n");
   do_ls(".");//当前目录

   //排序
   AlphaSort(array, i);
   printf("\n");

 }else{
   
   while( --ac){
      ++av;
     //如果是文件
     if(file_dir(*av) == 0){
       printf("文件: %s\n", *av);
       getMode(*av, 0);
       printf_list(array, 1);
       return 0;
     }  
       
     //如果是目录排序并打印
     printf("目录: %s\n", *av);
     do_ls(*av);
     AlphaSort(array, i);
     printf("\n");

   }

 }
 
 
}


void do_ls(char dirname[]){
  
  DIR *dir_ptr; /*the directory*/
  struct dirent *direntp;
  

  if((dir_ptr = opendir(dirname)) == NULL){
    printf("打开失败"); 
    fprintf(stderr, "ls1:can not open %s\n", dirname);  
  }else{
    while((direntp = readdir(dir_ptr))!=NULL){
        
       // printf("%s", direntp->d_name);    
        getMode(direntp->d_name, i);
        i++;
      
    }
    
    closedir(dir_ptr);
  }
  
}

void getMode(char *fname, int i){
 
 struct stat infobuf;
 char *str;
 
 if( stat(fname, &infobuf) == -1){
    
    printf("%s", fname);
    printf("文件状态获取失败\n"); 
 }
 else{
   
   if(S_ISDIR(infobuf.st_mode)) str = "/"; //目录
   else if(S_ISREG(infobuf.st_mode)){
     //user有没有x
     if(infobuf.st_mode & S_IXUSR){
       str = "*"; //具有普通权限的可执行文件
     }else{
     //普通文件   
       str = "\0"; 
     }
   }
   else if(S_ISSOCK(infobuf.st_mode)) str ="="; //socket套接字
   else if(S_ISFIFO(infobuf.st_mode)) str = "|"; //命令管道 
   
   strcat(fname, str);
   
   
   array[i] = fname;  
   
 } 
 
}

//按字母排序
void AlphaSort(char *array[], int size){
  
  char *temp;int j,k;
  for(j =0; j<size-1; j++){
    for(k=0; k<size-1-j; k++){
      if(strcmp(array[k], array[k+1])>0){
          temp = array[k];
          array[k] = array[k+1];
          array[k+1] = temp;
       }
    }
  }

 //打印
 printf_list(array, size);
}

//打印
void printf_list(char *array[], int size){
  int j,k;
  for(j =0; j<size;j++){
  //具有普通权限的可执行文件显示绿色，文件显示蓝色

  int len = strlen(array[j]);

  if(array[j][len-1] == '*'){

     /*for(k=0; k<len-1; k++){
       printf("\e[1;32m%c\e[0m", array[j][k]);  
     }*/
     //strtok分割字符串，返回下一下分割后的字符串
     strtok(array[j], "*");
     printf("\e[1;32m%s\e[0m", array[j]);
     printf("*  ");

   }else if(array[j][len-1] == '/'){
     strtok(array[j], "/");
     printf("\e[1;34m%s\e[0m", array[j]);
     printf("/  ");

   }else{
     printf("%s  ", array[j]);
   }

 }

}


//判断是否是文件还是目录
int file_dir(char *fname){
  struct stat buf; 
  int result; 
  result = stat( fname, &buf ); 
    if(S_IFDIR & buf.st_mode){ 
        return 1;
    }else if(S_IFREG & buf.st_mode){ 
        return 0; 
    } 

}
