#ifndef __CONTROL_H
#define __CONTRIL_H

#include "ui.h"
#include "commonheader.h"
#include "file.h"

void copy_event();
void delete_event();
void play_event();
void music_stop_event();

void interface_init();
void re_init();
void key_upDown_event();
void key_lefRig_event();
void arrow_key_event();
void key_enter_event();
void key_tab_event();
void key_f1_event();

void event();

#endif
