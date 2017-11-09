#include "ui.h"
#include <stdlib.h>
#include <string.h>

 WIN photo_win, audio_win, video_win;
 WIN copy_win, delete_win, play_win;
 
 WIN sub_bgd_win;
 WIN fun_win;
 WIN input_win;

int max_item_photo = 1;
int max_item_audio = 1;
int max_item_video = 1;

void ncur_init()
{
    /*基础环境初始化*/
    initscr();
    start_color();
    raw();
	curs_set(0);
    noecho();

    /*颜色配对信息初始化*/
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);

}

WINDOW *create_win(WINDOW *parent, struct win_prop wp)
{
    int p_oy, p_ox;
	getbegyx(parent, p_oy, p_ox);
    
    /*mvprintw(0, 0, "create_win: p_oy=%d,p_ox=%d,parent=%p\n", p_oy, p_ox, parent);*/
    WINDOW *local_win = newwin(wp.height, wp.width, wp.starty + p_oy, wp.startx + p_ox);
    wbkgd(local_win, COLOR_PAIR(wp.color_num));

    if(wp.en_border)
        wborder(local_win, wp.border.ls, wp.border.rs, wp.border.ts, wp.border.bs, wp.border.tl, wp.border.tr, wp.border.bl, wp.border.br);
	
	if(wp.title != NULL)
        mvwprintw(local_win, 0, 0, "%s", wp.title);

    if(wp.msg != NULL && (wp.en_border || (wp.title != NULL)))
        mvwprintw(local_win, 1, 1, "%s", wp.msg);
	else if(wp.msg != NULL)
        mvwprintw(local_win, 0, 0, "%s", wp.msg);
		

    wrefresh(parent);
    wrefresh(local_win);
    return local_win;
}

void destory_win(WINDOW *w, int pc_color)
{
    int wmax_y, wmax_x;
    getmaxyx(w, wmax_y, wmax_x);
	wmove(w, 0, 0);
	winsdelln(w, wmax_y);
	wbkgd(w, COLOR_PAIR(pc_color));
	wrefresh(w);
    delwin(w);
}

void clear_win(WINDOW *w, int pc_color)
{
	int wmax_y, wmax_x;
    getmaxyx(w, wmax_y, wmax_x);
	wmove(w, 0, 0);
	winsdelln(w, wmax_y);
	wbkgd(w, COLOR_PAIR(pc_color));
	wrefresh(w);
}

void wprin_msg(WIN w, char *msg, int y)
{
	int x = 0;
	if(w.wp.en_border || (w.wp.title != NULL))
		x = 1;
	mvwprintw(w.w, y, x, "%s", msg);
	wrefresh(w.w);
}

void wrev_item(WIN w, int y, int color_num)
{
	int max_y, max_x;
	getmaxyx(w.w, max_y, max_x);

	int x = 0;
	if(w.wp.en_border || (w.wp.title != NULL))
	{
		x = 1;
		max_x = max_x - 2;
	}
	else
	{
		x = 0;
		max_x = -1;
	}

	mvwchgat(w.w, y, x, max_x, A_NORMAL, color_num, NULL);
	wrefresh(w.w);
}

void winit_bkgd(struct win_prop sw, struct win_prop fw)
{
	sub_bgd_win.wp = sw;
	fun_win.wp = fw;

	bkgd(COLOR_PAIR(1));
	refresh();
	keypad(stdscr, TRUE);

	sub_bgd_win.w = create_win(stdscr, sub_bgd_win.wp);
	keypad(sub_bgd_win.w, TRUE);

	fun_win.w = create_win(sub_bgd_win.w, fun_win.wp);
	keypad(fun_win.w, TRUE);

}

void winit_ff(WINDOW *parent, struct win_prop w1_wp, struct win_prop w2_wp, struct win_prop w3_wp, int set)
{
	WINDOW *w1 = create_win(parent, w1_wp);
	keypad(w1, TRUE);
	WINDOW *w2 = create_win(parent, w2_wp);
	keypad(w2, TRUE);
	WINDOW *w3 = create_win(parent, w3_wp);
	keypad(w3, TRUE);

	if(parent == fun_win.w)
	{
		copy_win.wp = w1_wp;
		copy_win.w = w1;

		delete_win.wp = w2_wp;
		delete_win.w = w2;

		play_win.wp = w3_wp;
		play_win.w = w3;

		curr_fun_item.curr_win = copy_win.w;
		/*if(set)*/
		/*{*/
			curr_fun_item.y = 0;
			curr_fun_item.max_y = 0;
			wrev_item(copy_win, 0, 0);
		/*}*/
	}
	else
	{
		photo_win.wp = w1_wp;
		photo_win.w = w1;

		audio_win.wp = w2_wp;
		audio_win.w = w2;

		video_win.wp = w3_wp;
        video_win.w = w3;
		curr_file_item.curr_win = photo_win.w;
		/*if(set)*/
		/*{*/
			curr_file_item.y = 1;
			curr_file_item.max_y = 0;
		/*}*/
	}
}

void wcp_get_path(char *input)
{
	struct win_prop scan_wp = 
	{
		.title = NULL,
		.msg = "PATH:",
		.height = 1,
		.width = fun_win.wp.width - 2,
		.starty = 1,
		.startx = 1,
		.en_border = 0,
		.color_num = fun_win.wp.color_num
	};
	input_win.wp = scan_wp;
	input_win.w = create_win(fun_win.w, input_win.wp);

	echo();
	curs_set(1);
	wattron(input_win.w, A_REVERSE);
	wscanw(input_win.w, "%s", input);
	wattroff(input_win.w, A_REVERSE);
	
	noecho();
	curs_set(0);
	wrefresh(input_win.w);
}

void winit_all()
{
	int scr_y, scr_x;
	getmaxyx(stdscr, scr_y, scr_x);

	struct win_prop sw_wp = 
	{
		.msg = NULL,
		.title = NULL,
		.height = 30,
		.width = 90,
		.en_border = 0,
		.color_num = 2
	};
	sw_wp.starty = (scr_y - sw_wp.height) / 2;
	sw_wp.startx = (scr_x - sw_wp.width) / 2;

	struct win_prop fw_wp = 
    {
        .msg = NULL,
		.title = "FUNCTION",
		.height = sw_wp.height * 0.1f,
		.width = sw_wp.width * 0.5f,
        .en_border = 1,
		.color_num = 3,
        .border = {'-', '-', '|', '|', '+', '+', '+', '+'}
    };
	fw_wp.starty = sw_wp.height - fw_wp.height - 1;
	fw_wp.startx = (sw_wp.width - fw_wp.width) / 2;

	
	struct win_prop photo_wp = 
	{
		.msg = NULL,
		.title = "JPG",
		.height = sw_wp.height * 0.8f,
		.width = sw_wp.width * 0.3f,
		.en_border = 1,
		.color_num = 3,
        .border = {'-', '-', '|', '|', '+', '+', '+', '+'}
	};
	photo_wp.starty = 1;
	photo_wp.startx = sw_wp.width - 6 - photo_wp.width * 3;

	struct win_prop audio_wp = 
	{
		.msg = NULL,
		.title = "AUDIO",
		.height = sw_wp.height * 0.8f,
		.width = sw_wp.width * 0.3f,
		.en_border = 1,
		.color_num = 3,
        .border = {'-', '-', '|', '|', '+', '+', '+', '+'}
	};
	audio_wp.starty = 1;
	audio_wp.startx = photo_wp.startx + 2 + photo_wp.width;

	struct win_prop video_wp = 
	{
		.msg = NULL,
		.title = "VIDEO",
		.height = sw_wp.height * 0.8f,
		.width = sw_wp.width * 0.3f,
		.en_border = 1,
		.color_num = 3,
        .border = {'-', '-', '|', '|', '+', '+', '+', '+'}
	};
	video_wp.starty = 1;
	video_wp.startx = audio_wp.startx + 2 + audio_wp.width;

	struct win_prop copy_wp = 
	{
		.msg = "COPY",
		.title = NULL,
		.height = 1,
		.width = strlen("COPY"),
		.en_border = 0,
		.color_num = 3,
	};

	struct win_prop delete_wp = 
	{
		.msg = "DELETE",
		.title = NULL,
		.height = 1,
		.width = strlen("DELETE"),
		.en_border = 0,
		.color_num = 3,
	};

	struct win_prop play_wp = 
	{
		.msg = "PLAY",
		.title = NULL,
		.height = 1,
		.width = strlen("PLAY"),
		.en_border = 0,
		.color_num = 3,
	};

	copy_wp.starty = fw_wp.height / 2;
	copy_wp.startx = (fw_wp.width - 6 - copy_wp.width - delete_wp.width - play_wp.width) / 2;

	delete_wp.starty = copy_wp.starty;
	delete_wp.startx = copy_wp.startx + copy_wp.width + 2;

	play_wp.starty = copy_wp.starty;
	play_wp.startx = delete_wp.startx + delete_wp.width + 2;
	
	winit_bkgd(sw_wp, fw_wp);
	winit_ff(sub_bgd_win.w, photo_wp, audio_wp, video_wp, 1);
	winit_ff(fun_win.w, copy_wp, delete_wp, play_wp, 1);
	
	
}

void wre_draw()
{
	destory_win(copy_win.w, 1);
	destory_win(delete_win.w, 1);
	destory_win(play_win.w, 1);
	destory_win(fun_win.w, 1);
	destory_win(photo_win.w, 1);
	destory_win(audio_win.w, 1);
	destory_win(video_win.w, 1);
	destory_win(sub_bgd_win.w, 1);
	clear_win(stdscr, 1);

	winit_bkgd(sub_bgd_win.wp, fun_win.wp);
	winit_ff(sub_bgd_win.w, photo_win.wp, audio_win.wp, video_win.wp, 0);
	winit_ff(fun_win.w, copy_win.wp, delete_win.wp, play_win.wp, 0);

	

}


void wclear_fun(int del_scan_win)
{
	if(del_scan_win)
	{
		destory_win(input_win.w, fun_win.wp.color_num);
		winit_ff(fun_win.w, copy_win.wp, delete_win.wp, play_win.wp, 1);
		wrev_item(copy_win, 0, 0);
	}
	else
	{
		destory_win(copy_win.w, fun_win.wp.color_num);
		destory_win(delete_win.w, fun_win.wp.color_num);
		destory_win(play_win.w, fun_win.wp.color_num);
	}
}

void wphoto_show_msg(char *msg, int set)
{
    if(!set)
    {
        max_item_photo = 1;
        clear_win(photo_win.w, 1);
    }
	wprin_msg(photo_win, msg, max_item_photo);
	max_item_photo++;
}

void waudio_show_msg(char *msg, int set)
{
    if(!set)
    {
        max_item_audio = 1;
        clear_win(audio_win.w, 1);
    }
	wprin_msg(audio_win, msg, max_item_audio);
	max_item_audio++;
}

void wvideo_show_msg(char *msg, int set)
{
    if(!set)
    {
        max_item_video = 1;
        clear_win(video_win.w, 1);
    }
	wprin_msg(video_win, msg, max_item_video);
	max_item_video++;
}

void wrev_up_item()
{
	if(curr_file_item.curr_win == photo_win.w)
	{
		wrev_item(photo_win, curr_file_item.y, 1);

		if(curr_file_item.y <= 1)
			curr_file_item.y = max_item_photo;

		wrev_item(photo_win, --curr_file_item.y, 0);
	}
	else if(curr_file_item.curr_win == audio_win.w)
	{
	
		wrev_item(audio_win, curr_file_item.y, 1);

		if(curr_file_item.y <= 1)
			curr_file_item.y = max_item_audio;

		wrev_item(audio_win, --curr_file_item.y, 0);
	}
	else
	{
		wrev_item(video_win, curr_file_item.y, 1);

		if(curr_file_item.y <= 1)
			curr_file_item.y = max_item_video;

		wrev_item(video_win, --curr_file_item.y, 0);

	}
}

void wrev_down_item()
{
	if(curr_file_item.curr_win == photo_win.w)
	{
		wrev_item(photo_win, curr_file_item.y, 1);

		if(curr_file_item.y >= max_item_photo - 1)
			curr_file_item.y = 0;

		wrev_item(photo_win, ++curr_file_item.y, 0);
	}
	else if(curr_file_item.curr_win == audio_win.w)
	{
	
		wrev_item(audio_win, curr_file_item.y, 1);

		if(curr_file_item.y >= max_item_audio - 1)
			curr_file_item.y = 0;

		wrev_item(audio_win, ++curr_file_item.y, 0);
	}
	else
	{
		wrev_item(video_win, curr_file_item.y, 1);

		if(curr_file_item.y >= max_item_video - 1)
			curr_file_item.y = 0;

		wrev_item(video_win, ++curr_file_item.y, 0);

	}
}

void wfile_switch_next()
{
	if(curr_file_item.curr_win == photo_win.w)
	{
		wrev_item(photo_win, curr_file_item.y, 1);
		curr_file_item.curr_win = audio_win.w;
		wrev_item(audio_win, 1, 0);
		curr_file_item.max_y = max_item_audio;
	}
	else if(curr_file_item.curr_win == audio_win.w)
	{
		wrev_item(audio_win, curr_file_item.y, 1);
		curr_file_item.curr_win = video_win.w;
		wrev_item(video_win, 1, 0);
		curr_file_item.max_y = max_item_video;
	}
	else
	{
		wrev_item(video_win, curr_file_item.y, 1);
		curr_file_item.curr_win = photo_win.w;
		wrev_item(photo_win, 1, 0);
		curr_file_item.max_y = max_item_photo;
	}
	curr_file_item.y = 1;
}

void wfile_switch_prev()
{
	if(curr_file_item.curr_win == photo_win.w)
	{
		wrev_item(photo_win, curr_file_item.y, 1);
		curr_file_item.curr_win = video_win.w;
		wrev_item(video_win, 1, 0);
		curr_file_item.max_y = max_item_video;
	}
	else if(curr_file_item.curr_win == audio_win.w)
	{
		wrev_item(audio_win, curr_file_item.y, 1);
		curr_file_item.curr_win = photo_win.w;
		wrev_item(photo_win, 1, 0);
		curr_file_item.max_y = max_item_photo;
	}
	else
	{
		wrev_item(video_win, curr_file_item.y, 1);
		curr_file_item.curr_win = audio_win.w;
		wrev_item(audio_win, 1, 0);
		curr_file_item.max_y = max_item_audio;
	}
	curr_file_item.y = 1;

}

void wfun_switch_next()
{
	mvprintw(0, 0, "wfun_switch_next");
	refresh();
	if(curr_fun_item.curr_win == copy_win.w)
	{
		wrev_item(copy_win, 0, 1);
		curr_fun_item.curr_win = delete_win.w;
		wrev_item(delete_win, 0, 0);

	}
	else if(curr_fun_item.curr_win == delete_win.w)
	{
		wrev_item(delete_win, 0, 1);
		curr_fun_item.curr_win = play_win.w;
		wrev_item(play_win, 0, 0);
	}
	else if(curr_fun_item.curr_win == play_win.w)
	{
		wrev_item(play_win, 0, 1);
		curr_fun_item.curr_win = copy_win.w;
		wrev_item(copy_win, 0, 0);
	}
}

void wfun_switch_prev()
{
	if(curr_fun_item.curr_win == copy_win.w)
	{
		wrev_item(copy_win, 0, 1);
		curr_fun_item.curr_win = play_win.w;
		wrev_item(play_win, 0, 0);

	}
	else if(curr_fun_item.curr_win == delete_win.w)
	{
		wrev_item(delete_win, 0, 1);
		curr_fun_item.curr_win = copy_win.w;
		wrev_item(copy_win, 0, 0);
	}
	else
	{
		wrev_item(play_win, 0, 1);
		curr_fun_item.curr_win = delete_win.w;
		wrev_item(delete_win, 0, 0);
	}	
}


