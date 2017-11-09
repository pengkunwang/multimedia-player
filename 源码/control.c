#include "control.h"

void copy_event()
{
    char path[MAX_PATH] = {0};
    wclear_fun(0);
    wcp_get_path(path);
    wclear_fun(1);
    /*复制文件*/
    if(curr_file_item.curr_win == photo_win.w)
    {

        copy(2, curr_file_item.y-1, path);
    }
    else if(curr_file_item.curr_win == audio_win.w)
    {
        copy(3, curr_file_item.y-1, path);
    }
    else
    {
        copy(4, curr_file_item.y-1, path);
    }
    mvprintw(0, 0, "copy_event:%s", path);
    refresh();
}

void delete_event()
{
    char path[MAX_PATH];
    char name[MAX_NAME];
    int i;
    /*提取文件名称，将文件删除*/
    if(curr_file_item.curr_win == photo_win.w)
    {
        remove_by_order(jpglist, curr_file_item.y-1);
        Read_dir();
        bzero(path, MAX_PATH);
        bzero(name, MAX_NAME);
        for(i=0; i<num_jpg; i++)
        {
            find_by_order_name_tobuff(jpglist, i, path);
            get_name(path, name);
            wphoto_show_msg(name, i);
        }

    }
    else if(curr_file_item.curr_win == audio_win.w)
    {
        remove_by_order(mp3list, curr_file_item.y-1);
        Read_dir();
        bzero(path, MAX_PATH);
        bzero(name, MAX_NAME);
        for(i=0; i<num_mp3; i++)
        {
            find_by_order_name_tobuff(mp3list, i, path);
            get_name(path, name);
            waudio_show_msg(name, i);
        }
    }
    else
    {
        remove_by_order(mp4list, curr_file_item.y-1);
        Read_dir();
        bzero(path, MAX_PATH);
        bzero(name, MAX_NAME);
        for(i=0; i<num_mp4; i++)
        {
            find_by_order_name_tobuff(mp4list, i, path);
            get_name(path, name);
            wvideo_show_msg(name, i);
        }
    }
}

void play_event()
{
    char path[MAX_PATH];
    /*提取文件名称，对文件进行操作*/
    if(curr_file_item.curr_win == photo_win.w)
    {
        find_by_order_name_tobuff(jpglist, curr_file_item.y-1, path);
        show_jpg(path);
        re_init();
    }
    else if(curr_file_item.curr_win == audio_win.w)
    {
        find_by_order_name_tobuff(mp3list, curr_file_item.y-1, path);
        mp3play(path);
	    move(0, 0);
    }
    else
    {
        
    }

}

void music_stop_event()
{
    system("killall -9 madplay");
    move(0, 0);
}

void interface_init()
{

    init_list(mount_dir());
	ncur_init();
	winit_all();
    Read_dir();

    int i =0;
    char path[MAX_PATH];
    char name[MAX_NAME];

    bzero(path, MAX_PATH);
    bzero(name, MAX_NAME);
    for(i=0; i<num_jpg; i++)
    {
        find_by_order_name_tobuff(jpglist, i, path);
        get_name(path, name);
        wphoto_show_msg(name, i);

    }
    
    bzero(path, MAX_PATH);
    bzero(name, MAX_NAME);
    for(i=0; i<num_mp3; i++)
    {
        find_by_order_name_tobuff(mp3list, i, path);
        get_name(path, name);
        waudio_show_msg(name, i);
    }

    
    bzero(path, MAX_PATH);
    bzero(name, MAX_NAME);
    for(i=0; i<num_mp4; i++)
    {
        find_by_order_name_tobuff(mp4list, i, path);
        get_name(path, name);
        wvideo_show_msg(name, i);
    }
    
}

void re_init()
{
	wre_draw();
	init_list("/home/ubu/Media");
    Read_dir();

    int i =0;
    char path[MAX_PATH];
    char name[MAX_NAME];

    bzero(path, MAX_PATH);
    bzero(name, MAX_NAME);
    for(i=0; i<num_jpg; i++)
    {
        find_by_order_name_tobuff(jpglist, i, path);
        get_name(path, name);
        wphoto_show_msg(name, i);

    }
    
    bzero(path, MAX_PATH);
    bzero(name, MAX_NAME);
    for(i=0; i<num_mp3; i++)
    {
        find_by_order_name_tobuff(mp3list, i, path);
        get_name(path, name);
        waudio_show_msg(name, i);
    }

    
    bzero(path, MAX_PATH);
    bzero(name, MAX_NAME);
    for(i=0; i<num_mp4; i++)
    {
        find_by_order_name_tobuff(mp4list, i, path);
        get_name(path, name);
        wvideo_show_msg(name, i);
    }
}

void key_upDown_event(int ch)
{
	if(ch == KEY_UP)
		wrev_up_item();
	else
		wrev_down_item();
}

void key_lefRig_event(int ch)
{
	if(ch == KEY_LEFT)
		wfile_switch_prev();
	else
		wfile_switch_next();
}

void arrow_key_event(int ch)
{
	if((ch == KEY_LEFT) || (ch == KEY_RIGHT))
		key_lefRig_event(ch);
	else
		key_upDown_event(ch);
}

void key_tab_event()
{
	wfun_switch_next();
}

void key_f1_event()
{
	endwin();
}

void key_enter_event()
{
    if(curr_fun_item.curr_win == copy_win.w)
    {
        copy_event();
    }
    else if(curr_fun_item.curr_win == delete_win.w)
    {
        delete_event();
    }
    else
    {
        play_event();
    }
}

void event()
{
	int ch;
	while((ch = getch()) != KEY_F(1))
	{
		switch(ch)
		{
			case KEY_LEFT:
			case KEY_RIGHT:
			case KEY_UP:
			case KEY_DOWN:
			arrow_key_event(ch);
			break;
			case 9:
			key_tab_event();
			break;
			case 10:
			key_enter_event();
			break;
            case 'q':
            music_stop_event();
            break;
		}
	}
	key_f1_event();
}

