#ifndef __UI_H
#define __UI_H

#include <ncurses.h>

struct item_msg
{
    int y, max_y;
    WINDOW *curr_win;
    WINDOW *next_win;
    WINDOW *prev_win;
}curr_file_item, curr_fun_item;

struct win_border
{
    chtype ts, bs, ls, rs;
    chtype tl, tr, bl, br;
};

struct win_prop 
{
    int starty, startx;
    int height, width;
	int color_num;
    int en_border;
    char *msg;
	char *title;
    struct win_border border;
};

typedef struct
{
	WINDOW *w;
	struct win_prop wp;
}WIN;

extern WIN photo_win, audio_win, video_win;
extern WIN copy_win, delete_win, play_win;
extern WIN fun_win;

void ncur_init();

WINDOW *create_win(WINDOW *parent, struct win_prop wp);
void destory_win(WINDOW *w, int pc_num);
void wprin_msg(WIN w, char *msg, int y);
void wrev_item(WIN w, int y, int color_num);

void winit_bkgd(struct win_prop sw, struct win_prop fw);
void winit_ff(WINDOW *parent, struct win_prop w1, struct win_prop w2, struct win_prop w3, int set);
void wcp_get_path(char *input);
void winit_all();

void wclear_fun(int del_scan_win);
void clear_win(WINDOW *w, int pc_color);
void wre_draw();
void wphoto_show_msg(char *msg, int set);
void waudio_show_msg(char *msg, int set);
void wvideo_show_msg(char *msg, int set);

void wrev_up_item();
void wrev_down_item();
void wfun_switch_next();
void wfun_switch_prev();
void wfile_switch_next();
void wfile_switch_prev();

#endif
