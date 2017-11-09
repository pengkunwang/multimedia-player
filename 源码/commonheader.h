#ifndef __COMMONHEADER__H__
#define __COMMONHEADER__H__

#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/mount.h>

#define _PATH_MAX 256

#define _PATH_NAME "/mnt/usb"

#define _U_TYPE "vfat"

char *mount_dir();

int show_jpg(char *path);



#endif // __MODLE__H__
