/**************************************************************************
	> File Name:     ans.c
	> Author:        likeqiang
	> mail:          likeqiang2020@gmail.com 
	> Created Time:  2021年05月31日 星期一 23时55分57秒
	> Description:   
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 128


/* key结构 */
struct list_key {
	char keyName[SIZE];
	char keyVal[SIZE];
	struct	list_key *next;
}list_key;


/* 标头结构 */
struct list_header {
	char headerName[SIZE];
	struct list_header *next;
	struct list_key *key;
}list_header;


struct list_header *dummy = NULL;                     // 第一层链表的头节点
struct list_header *suc   = NULL;                     // 第一层链表的尾节点
struct list_key *tail     = NULL;										  // 第二层链表的尾节点


/* 第一层链表初始化,即添加头节点 */
int init_haeder()
{
	dummy = malloc(sizeof(list_header));
	if(dummy == NULL) return -1;
	memset(dummy->headerName, '\0', SIZE);
	dummy->next = NULL;
	dummy->key  = NULL;
	suc = dummy;
	return 0;
}


/* 第一层链表添加标头节点 */
int add_header(char *name)
{
	if(name == NULL)	    return -1;
	if(strlen(name) <= 0)	return -1;
	struct list_header *newHeader = malloc(sizeof(list_header));
	if(newHeader == NULL)	return -1;

	memset(newHeader->headerName, '\0', SIZE);
	strncpy(newHeader->headerName, name, strlen(name));
	newHeader->next = NULL;
	newHeader->key  = NULL;

	suc->next = newHeader;
	suc  = newHeader;
	return 0;
}


/* 第二层链表添加key节点 */
void add_key(char *name, char *val)
{
	if(name == NULL || val == NULL) return;
	if(strlen(name) == 0 || strlen(val) == 0)	return;
	struct list_key *newKey = malloc(sizeof(list_key));
	if(newKey == NULL)	return;

	memset(newKey->keyName, '\0', SIZE);
	memset(newKey->keyVal, '\0', SIZE);
	strncpy(newKey->keyName, name, strlen(name));
	strncpy(newKey->keyVal, val, strlen(val));
	newKey->next = NULL;

	if(suc->key == NULL) {
		suc->key = newKey;
		tail = newKey;
	} else {	
		tail->next = newKey;
		tail = newKey;
	}
}


/* 释放链表 */
void destroy()
{
	while(dummy->next) {
		struct list_header *curHeader = dummy->next;
		struct list_key *curKey = curHeader->key;
		while(curKey) {
			curHeader->key = curKey->next;
			free(curKey);
			curKey = curHeader->key;
		}
		dummy->next = curHeader->next;
		free(curHeader);
		curHeader = dummy->next;
	}
	free(dummy);
}


/* 找到=的位置 */
int get_pos(char *buf)
{
	int len = strlen(buf);
	if(len <= 0)	return -1;
	for(int i = 0; i < len; i++) {
		if(buf[i] == '=') return i;
	}
	return -1;
}


/* 加载配置到链表上 */
int load_config(struct list_header *list, char *filename)
{
	if(list == NULL)	return -1;
	if(filename == NULL || strlen(filename) == 0)	return -1;

	FILE *fp = fopen(filename, "r");
	if(fp == NULL)
	{ perror("fopen"); return -1; }	

	char buf[SIZE]  = {'\0'};
	char name[SIZE] = {'\0'};
	char val[SIZE]  = {'\0'};

	while(fgets(buf, SIZE, fp) != NULL) {
		int len = strlen(buf) - 1;    // strlen() 会计算换行符,所以要-1
		if(len <= 0)	continue;
		if(buf[0] == '[' && buf[len - 1] == ']') {
			memset(name, '\0', SIZE);
			strncpy(name, buf + 1, len - 2);
			add_header(name);
		} else {
			int pos = get_pos(buf);
			if(pos == -1)	continue;
			memset(name, '\0', SIZE);
			memset(val, '\0', SIZE);
			strncpy(name, buf, pos);
			strncpy(val, buf + pos + 1, len - pos - 1);
			add_key(name, val);
		}
		memset(buf, '\0', SIZE);
	}
	fclose(fp);

	return 0;
}


/* 获取配置某字段内容 */
char *get_config_value(struct list_header *list, char *tag, char *key)
{
	if(!list || !list->next || !tag || !key)	return NULL;
	if(!strlen(tag) || !strlen(key))          return NULL;

	struct list_header *curHeader = list->next;
	struct list_key *curKey = NULL;
	int len_tag = strlen(tag) - 1;
	int len_key = strlen(key) - 1; 

	while(curHeader) {
		if(strncmp(curHeader->headerName, tag, len_tag) == 0) {
			curKey = curHeader->key;
			while(curKey) {
				if(strncmp(curKey->keyName, key, len_key) == 0) {
					return curKey->keyVal;
				}
				curKey = curKey->next;
			}
			return NULL;
		}
		curHeader = curHeader->next;
	}
	return NULL;
}


int main(int argc, char *argv[])
{
	if(argc != 3)
	{ printf("能且只能输入tag和key\n"); return 0; }

	int ret = -1;
	ret = init_haeder();
	if(ret == -1)
	{ printf("链表初始化失败.\n"); return -1; }

	ret = load_config(dummy, "/home/andy/Documents/ans/config.txt"); // 绝对路径
	if(ret == -1)
	{ printf("配置文件加载失败.\n"); return -1; }

	char *rev = get_config_value(dummy, argv[1], argv[2]);
	printf("%s\n", rev);

	destroy();
	
	return 0;
}
