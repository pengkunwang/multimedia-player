#include "commonheader.h"


char *mount_dir ()
{
    char *p = malloc(strlen(_PATH_NAME));
    struct dirent *entry = NULL;
    DIR *dir = opendir("/dev");
    DIR *usb = NULL;

    while(1)  //judge
    {
        if((entry = readdir(dir)) == NULL)
            rewinddir(dir);
        else if(entry->d_type == DT_BLK && strlen(entry->d_name) == 4)
        {
            if(entry->d_name[0] == 's' && entry->d_name[1] == 'd' && entry->d_name[2] >= 98 && entry->d_name[2] <= 112)
                break;
        }
    }

    if((usb = opendir(_PATH_NAME)) == NULL)
        mkdir(_PATH_NAME,0777);
    closedir(usb);

    sleep(3);      //Waiting for U disk loading
    char buff[_PATH_MAX] = "/dev/";
    mount(strcat(buff,entry->d_name), _PATH_NAME, _U_TYPE, 0, 0);    //mount

    strcpy(p,_PATH_NAME);

    closedir(dir);

    return p;
}



int show_jpg(char *path)
{
    int fd;
    struct stat state;
    char buf[_PATH_MAX] = "fbi ";
    char *p =NULL;

    if((fd = open(path,O_RDONLY)) == -1)
        return -1;
    else if(fstat(fd,&state) == -1)
        return -1;
    else if(!S_ISREG(state.st_mode))
        return 0;
    else if(!(p = rindex(path,'.')))
        return 0;
    else if(!strcmp(p,".jpg")|| !strcmp(p,".jpeg"))
    {
        close(fd);
        system(strcat(buf,path));
        return 1;
    }
    else
        return 0;
}
