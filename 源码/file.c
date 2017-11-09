#include "file.h"


f_list *jpglist, *mp3list, *mp4list;

char Path[MAX_PATH];
unsigned int num_jpg = 0, num_mp3 = 0, num_mp4 = 0;

/*初始化一个链表*/

void init_list(const char *path)

{

	bzero(Path, MAX_PATH);
	strcpy(Path, path);

	jpglist = malloc(sizeof(f_list));
	mp3list = malloc(sizeof(f_list));
	mp4list = malloc(sizeof(f_list));

	INIT_LIST_HEAD( &jpglist->list);
	INIT_LIST_HEAD( &mp3list->list);
	INIT_LIST_HEAD( &mp4list->list);

}





/*增加节点到链表*/

void new_node(f_list *mylist, unsigned int n, char *name, 

	unsigned char type, unsigned int size)

{

	f_list *new = malloc(sizeof(f_list));

	new->n = n;

	strcpy(new->name, name);

	new->type = type;

	new->size = size;

	list_add_tail(&new->list, &mylist->list);

}





/*有绝对路径获取文件名*/

void get_name(char *path, char *name)

{

	char buff[MAX_PATH];

	char *delim = "/";

	char *n, p[MAX_NAME];

	strcpy(buff, path);

	n = strtok(buff, delim);

	do{

		strcpy(p, n);

	}while((n = strtok(NULL, delim)) != NULL);

	strcpy(name, p);

}



/*打印链表里的所有信息*/

void show_list(f_list *mylist)

{

	f_list *this;

	list_for_each_entry(this, &mylist->list, list)

	{

		printf("%-5d", this->n);

		printf("%-30s", this->name);

		switch(this->type)

		{

			case 2:

				printf("\tjpg");

				break;

			case 3:

				printf("\tmp3");

				break;

			case 4:

				printf("\tmp4");

				break;

			default:

				printf("uknow type\n");

				break;

		}

		printf("%10d\n", this->size);

	}

}





/*返回指定序号的文件节点，未找到则返回空*/

f_list *find_by_order(f_list *mylist, unsigned int n)

{

	f_list *this;

	list_for_each_entry(this, &mylist->list, list)

	{

		if(this->n == n)

			return this;

	}



	return NULL;

}



/*根据文件名返回文件的节点，未找到则返回空*/

f_list *find_by_name(f_list *mylist, char *name)

{

	f_list *this;

	char buff[20];

	list_for_each_entry(this, &mylist->list, list)

	{

		bzero(buff,20);

		get_name(this->name, buff);

		if(strcmp(name, buff) == 0)

			return this;

	}

	return NULL;

}





/*将指定序号的绝对路径名，赋值给pathname*/

int find_by_order_name_tobuff(f_list *mylist, unsigned int n, char *pathname)

{

	f_list *this;

	if((this = find_by_order(mylist, n)) != NULL)

	{

		strcpy(pathname, this->name);

		return 0;

	}

	else

		return -1;

}





/*查找文件名并找到绝对路径名，赋值给pathname*/

int find_by_name_pathname_tobuff(f_list *mylist, char *name, char *pathname)

{

	f_list *this;

	if((this = find_by_name(mylist, name)) != NULL)

	{

		strcpy(pathname, this->name);

		return 0;

	}

	else

		return -1;

}


/*删除整条列表(头结点除外)*/

void clear_list(f_list *mylist)

{	

	f_list *pos, *n;

	list_for_each_entry_safe(pos, n, &mylist->list, list)

	{

		list_del_init(&pos->list);

		free(pos);

	}
	num_jpg = 0;
	num_mp3 = 0;
	num_mp4 = 0;

}


/*删除指定序号的文件*/

int remove_by_order(f_list *mylist, unsigned int n)

{

	f_list *this;

	if((this = find_by_order(mylist, n)) != NULL)

	{
		if(remove(this->name) != 0)
		{
			printf("remove fail\n");
			return -1;
		}

		list_del_init(&this->list);

		free(this);

		clear_list(jpglist);
		clear_list(mp3list);
		clear_list(mp4list);
		Read_dir();

		return 0;

	}

	else

		return -1;

}



/*删除指定名称的文件*/

int remove_by_name(f_list *mylist, char *name)

{

	f_list *this;

	if((this = find_by_name(mylist, name)) != NULL)

	{
		if(remove(this->name) != 0)
		{
			printf("remove fail\n");
			return -1;
		}

		list_del_init(&this->list);

		free(this);

		clear_list(jpglist);
		clear_list(mp3list);
		clear_list(mp4list);
		Read_dir();

		return 0;

	}

	else

		return -1;

}






/*识别各种文件并分类存放在不同的链表*/
void divide(struct dirent *p, char *childpath, f_list *jpglist, 
	f_list *mp3list, f_list *mp4list)
{
	char name[MAX_NAME];
	struct stat buf;
	get_name(childpath, name);
	char *nod;
	if((nod = strrchr(name, 46)) != 0)
	{
		stat(childpath, &buf);
		if( (strcmp(nod, ".jpg") == 0) || (strcmp(nod, ".jpeg") == 0) )
		{
			new_node(jpglist, num_jpg, childpath, 2, buf.st_size);
			num_jpg++;
		}
		else if(strcmp(nod, ".mp3") == 0)
		{
			new_node(mp3list, num_mp3, childpath, 3, buf.st_size);
			num_mp3++;
		}
		else if(strcmp(nod, ".mp4") == 0)
		{
			new_node(mp4list, num_mp4, childpath, 4, buf.st_size);
			num_mp4++;
		}
		else
		{

		}
	}
}


/*遍历目录*/
void read_dir(const char *path)
{
	char childpath[MAX_PATH];
	DIR *dir;
	struct dirent *ptr;
	if( (dir = opendir(path)) == NULL)
	{
		printf("opendir fail\n");
	}
	while((ptr = readdir(dir)) != NULL)
	{

		sprintf(childpath, "%s/%s", path, ptr->d_name);
		if(strlen(childpath) < MAX_PATH)
		{
			if(ptr->d_type == 4)
			{
				if((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
					continue;
				read_dir(childpath);
						
			}
			else
			{
				divide(ptr, childpath, jpglist, mp3list, mp4list);
			}
		}
		else
			printf("the path is too long\n");

	}

	closedir(dir);
}

/*遍历目录*/
void Read_dir()
{
	num_jpg = 0;
	num_mp3 = 0;
	num_mp4 = 0;
	read_dir(Path);
}


/*复制函数*/
void copy(unsigned char type, unsigned int n, char *path)
{
	char buff[MAX_PATH];
	char srcpath[MAX_PATH];
	bzero(srcpath, MAX_PATH);
	bzero(buff, MAX_PATH);
	switch(type)
	{
		case 2:
			find_by_order_name_tobuff(jpglist, n, srcpath);
			sprintf(buff, "cp %s %s", srcpath, path);
			system(buff);
			break;

		case 3:
			find_by_order_name_tobuff(mp3list, n, srcpath);
			sprintf(buff, "cp %s %s", srcpath, path);
			system(buff);
			break;

		case 4:
			find_by_order_name_tobuff(mp4list, n, srcpath);
			sprintf(buff, "cp %s %s", srcpath, path);
			system(buff);
			break;

		default :
			printf("cp fail\n");
			break;
	}

}



//播放mp3音乐
void mp3play(char *path)
{
	char buff[MAX_PATH];
	sprintf(buff, "madplay -Q %s &", path);
	system(buff);
}