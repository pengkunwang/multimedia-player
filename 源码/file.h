#ifndef __FILE_H_

#define __FILE_H_



#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "list.h"

#include <sys/types.h>

#include <dirent.h>

#include <sys/stat.h>

#include <unistd.h>

#define MAX_PATH 256
#define MAX_NAME 50

extern unsigned int num_jpg, num_mp3, num_mp4;



/*jpg图片信息链表*/

typedef struct F_list  

{

	unsigned int n;     //文件编号

	char name[MAX_PATH];  //文件绝对路径名称

	unsigned char type;  //类型  2:jpg  3:mp3   4:mp4

	unsigned int size;   //文件大小

	struct list_head list;  //链表

}f_list;



extern f_list *jpglist, *mp3list, *mp4list;



/**

*函数作用：初始化链表jpglist, mp3list, mp4list;
*
*函数原型:void init_list(const char *path);
*@path   :初始化的目录

*返回值:空*/

void init_list(const char *path);





/***

*函数作用：增加节点到链表

*函数原型:void new_node(f_list *mylist, unsigned char n, char *name, 

	unsigned char type, unsigned int size);

*@mylist:目标链表

*@n 	:序号

*@name  :文件绝对路径名

*@type  :文件类型 2:jpg   3:mp3   4:mp4

*返回值:空*/

void new_node(f_list *mylist, unsigned int n, char *name, 

	unsigned char type, unsigned int size);



/***

*函数作用：由绝对路径获取文件名

*函数原型:void get_name(char *path, char *name);

*@path  :绝对路径

*@name  :文件名

*返回值:空*/

void get_name(char *path, char *name);





/***

*函数作用：打印链表里的所有信息

*函数原型:void show_list(f_list *mylist);

*@mylist  :将要打印的数据链表体指针

*返回值:空*/

void show_list(f_list *mylist);





/***

*函数作用：查找序号并返回文件节点，未找到这返回空

*函数原型:f_list *find_by_order(f_list *mylist, unsigned int n);

*@mylist :从这个链表中查找

*@n      : 指定的序号

*返回值:f_list链表类型指针*/

f_list *find_by_order(f_list *mylist, unsigned int n);





/***

*函数作用：查找文件名返回文件的节点，未找到则返回空

*函数原型:f_list *find_by_name(f_list *mylist, char *name);

*@mylist :从这个链表中查找

*@name   : 将要查找的文件名

*返回值:f_list链表类型指针*/

f_list *find_by_name(f_list *mylist, char *name);





/***

*函数作用：查找指定序号的绝对路径名，赋值给pathname

*函数原型:int find_by_order_name_tobuff(f_list *mylist, unsigned int n, char *name)

*@mylist :从这个链表中查找

*@n      : 指定的序号

*@pathname   :返回的绝对路径名

*返回值  :成功返回0, 失败返回-1*/

int find_by_order_name_tobuff(f_list *mylist, unsigned int n, char *pathname);







/***

*函数作用：查找文件名并找到绝对路径名，赋值给pathname

*函数原型:int find_by_order_name_tobuff(f_list *mylist, unsigned int n, char *name)

*@mylist :从这个链表中查找

*@name      : 指定的文件名

*@pathname   :返回的绝对路径名

*返回值  :成功返回0, 失败返回-1*/

int find_by_name_pathname_tobuff(f_list *mylist, char *name, char *pathname);
 






/***

*函数作用：删除指定序号的文件

*函数原型:int remove_by_order(f_list *mylist, unsigned int n);

*@mylist :从这个链表中查找

*@n      : 指定将要删除的序号

*返回值  :成功返回0, 失败返回-1*/

int remove_by_order(f_list *mylist, unsigned int n);





/***

*函数作用：删除指定名称的文件

*函数原型:int remove_by_name(f_list *mylist, char *name);

*@mylist :从这个链表中查找

*@name      : 指定将要删除的文件名

*返回值  :成功返回0, 失败返回-1*/

int remove_by_name(f_list *mylist, char *name);



/***

*函数作用：删除整条列表(头结点除外)

*函数原型:int clear_list(f_list *mylist);

*@mylist :将要删除的列表结点

*返回值  :空*/

void clear_list(f_list *mylist);


/***

*函数作用：根据struct dirent结构体指针，区分.jpg、mp3、和mp4存放在不同链表

*函数原型:void divide(struct dirent *p, char *childpath, f_list *jpglist, 
	f_list *mp3list, f_list *mp4list);

*@p :struct dirent结构体指针
*@childpath ：文件的绝对路径
*@jpglist ：图片链表
*@mp3list :MP3链表
*@mp4list :mp4链表 

*返回值  :空*/

void divide(struct dirent *p, char *childpath, f_list *jpglist, 
	f_list *mp3list, f_list *mp4list);



/***

*函数作用：遍历参数提供的目录，并将.jpg、mp3、和mp4存放在不同链表

*函数原型:void read_dir(const char *path);

*@path :将要遍历的目录

*返回值  :空*/
void read_dir(const char *path);


/***

*函数作用：读取目录

*函数原型:void Read_dir();

*返回值  :空*/
void Read_dir();


/***

*函数作用：复制文件

*函数原型:void copy(unsigned char type, unsing int n, char *path)；
*@type   :将要复制的类型 2:jpg   3:mp3     4:mp4
*@n      :该文件在链表中的序号
*@path   :复制的目标路径
*返回值  :空*/
void copy(unsigned char type, unsigned int n, char *path);


/***

*函数作用：

*函数原型:void mp3play(char *path)；
*@path   :播放的目标路径
*返回值  :空*/
void mp3play(char *path);

#endif 