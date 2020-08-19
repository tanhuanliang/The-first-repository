#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"



/********************************************************************************************
*功能：初始化头节点
*参数：无
*返回值：无
*********************************************************************************************/
struct rep_dict_head_t *init_rep_dict() {
  struct rep_dict_head_t *dhead = NULL;
  dhead = malloc(sizeof(*dhead));//为头节点开辟空间
  if (dhead == NULL) {//开辟空间失败
    perror("dict head");
    return NULL;
  }
  memset(dhead, 0, sizeof(*dhead));
  dhead->count = 0;//节点的个数置0
  dhead->head = NULL;//节点的头指向NULL
  return dhead;
}



/********************************************************************************************
*功能：销毁链表
*参数：链表头结点
*返回值：无
*********************************************************************************************/
void  destroy_rep_dict(struct rep_dict_head_t *dhead) {
  struct rep_dict_t *curNode,*tmpNode;
  int i;
  curNode = dhead->head;
  for (i = 0; curNode != NULL; i++){
    free(curNode->name);
    tmpNode = curNode;
    curNode = curNode->next;
    free(tmpNode);
  }
  free(dhead);
}


/********************************************************************************************
*功能：创建新节点，并通过尾插法插入链表末尾
*参数：
*	dhead 链表头
*	data 链表的数据
*返回值：新链表的头
*********************************************************************************************/
struct rep_dict_head_t *add_rep_dict_node(struct rep_dict_head_t *dhead,
    struct rep_dict_t *data) {
  struct rep_dict_t *tmpNode = NULL, *curNode = NULL;
  int i;
  tmpNode = malloc(sizeof(*tmpNode));
  if (tmpNode == NULL) {
    perror("dict add Node");
    return NULL;
  }
  memset(tmpNode, 0, sizeof(*tmpNode));
  tmpNode->name = strdup(data->name);//分别给链表的各个成员变量赋值
  tmpNode->nlen = strlen(tmpNode->name);
  tmpNode->type = data->type;
  tmpNode->fun = data->fun;
  tmpNode->next = NULL;

  if (dhead->head == NULL) {//如果头节点指向NULL，把赋值好的节点赋值给他
    dhead->count += 1;
    dhead->head = tmpNode;
    return dhead;
  }

  dhead->count += 1;
  curNode = dhead->head;
  for (i = 0;; i++) {//遍历链表找到最后一个节点的位置
    if (curNode->next == NULL) {
      break;
    }
    curNode = curNode->next;
  }
  curNode->next = tmpNode;//通过尾插法，把新创建的节点插入链表末尾
  return dhead;//返回链表的头结点
}

void add_dict_fun_string(struct rep_dict_head_t *dhead, char *name,
    fun_dict_string str) {
  struct rep_dict_head_t *dtmp;
  struct rep_dict_t data;

  data.name = name;
  data.type = REP_FUN_STRING;
  data.fun.fp = str;
  dtmp = add_rep_dict_node(dhead, &data);//把链表头赋值给dtmp
}

void add_dict_fun_fp(struct rep_dict_head_t *dhead, char *name,
    fun_dict_fp fp) {
  struct rep_dict_head_t *dtmp;
  struct rep_dict_t data;

  data.name = name;
  data.type = REP_FUN_FP;
  data.fun.fp = fp;
  dtmp = add_rep_dict_node(dhead, &data);//把链表头赋值给dtmp
}


/********************************************************************************************
*功能：遍历链表查找名字为name的节点
*参数：
*	dhead 链表头
*	name 节点名字
*返回值：
*	成功：查找到的节点
*	失败： NULL
*********************************************************************************************/
struct rep_dict_t *get_rep_dict_node(struct rep_dict_head_t *dhead, char *name) {
  struct rep_dict_t *curNode;
  int i;
  curNode = dhead->head;
  for (i = 0; curNode != NULL; i++, curNode = curNode->next) {
    if(strncmp(name,curNode->name,curNode->nlen)==0){//对比当前节点的名字与要查找的节点是否相同
      return curNode;
    }
  }
  return NULL;
}


/********************************************************************************************
*功能：打印链表的详细信息，包含链表的节点的个数，每个节点的名字和名字的长度
*参数：
*	dhead 链表头
*返回值：无
*********************************************************************************************/
void printf_rep_dict(struct rep_dict_head_t *dhead) {
  struct rep_dict_t *curNode;
  int i;
  curNode = dhead->head;
  printf("count:%d\n", dhead->count);//打印链表的节点的个数
  for (i = 0; curNode != NULL; i++, curNode = curNode->next) {
    printf("%d %s,%d\n", i, curNode->name, curNode->nlen);//打印节点的名字和名字的长度
  }
}

#if 0

void cb_shm(int a, int b) {
  printf("%d %d\n", a, b);
}
void cb_system(int a, char *cmd) {
  printf("%d %s\n", a, cmd);
}

int main() {
  struct rep_dict_head_t *dhead;
  struct rep_dict_t data,*curNode;
  dhead = init_rep_dict();//初始化头节点
  data.name = "abc1";//给节点的名字赋值
  data.fun.shm = &cb_system;
  add_rep_dict_node(dhead, &data);
  printf_rep_dict(dhead);
  curNode = get_rep_dict_node(dhead,"abc1");
  curNode->fun.system(1,"2");//参数；(标准输出， REP_FUN_FP)
}

#endif
