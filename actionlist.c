#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct actionlist_t{
  char *list[4096];
  int num;
  int cur;
};

struct actionlist_t actionlist;


/********************************************
*功能：读取文件的每一行内容，并存入链表
*参数：文件的路径
*返回值：链表节点的个数
*********************************************/
int init_actionlist(char *path){
  FILE *fp;
  char *line=NULL;
  char *tmp=NULL;
  int i;
  size_t len;
  //32位操作系统表示 unsigned int，     64位操作系统表示 unsigned long,
  //用于缓冲区大小等非负值的情况，如有多少东西被拷贝等,sizeof操作符的结果类型是size_t
  //在数组下标和内存管理函数之类的地方广泛使用
  ssize_t read;
  //32位操作系统int，     64位操作系统long,
  //1.用于被执行读写操作的数据块的大小
  //2.用于可能执行失败返回值<0 的函数，
  memset(&actionlist,0,sizeof(struct actionlist_t));
  fp = fopen(path,"r");
  if(fp==NULL){
    perror("fopen");
    return -1;
  }
  i=0;
  while ((read = getline(&line, &len, fp)) != -1){
  //完成读入一行数据
  //从istream中读取len个字符(包含结束标记符)保存在line对应的数组中
  //即使没读够len个字符,如遇fp或字数达到限制，则读取终止，fp不会被保存进line对应的数组中
    printf("actionlist:%s\n",line);
    actionlist.list[i]=strdup(line);//把getline读取到的每一行的内容，存放进actionlist链表
                                    //strdup内部调用了malloc()为变量分配内存
    tmp=strchr(actionlist.list[i],'\n');
    if(tmp!=NULL){
      *tmp='\0';//把每一行末尾的'\n'替换成'\0'
    }
    actionlist.num++;//链表每增加一个节点，计数器加一
    i++;
  }
  if(line)
    free(line);
  fclose(fp);
  return actionlist.num;//返回值：链表节点的个数
}


/********************************************
*功能：销毁链表
*参数：无
*返回值：无
*********************************************/
int destroy_actionlist(){
  int i;
  for(i=0;i<actionlist.num;i++){
    free(actionlist.list[i]);
  }
  actionlist.num=0;
  actionlist.cur=0;
}


char *get_actionlist(){
  char *tmp;
  tmp=actionlist.list[actionlist.cur];
  actionlist.cur++;
  if(actionlist.cur>=actionlist.num){
    actionlist.cur=0;
  }
  return tmp;
}

#if 0

int main(){
  int num,i;
  num = init_actionlist("./actionlist.txt");
  for(i=0;i<num;i++){
    printf("line:%s\n",get_actionlist());
  }
}

#endif
