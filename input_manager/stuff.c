#include "data_structures/dqueue.h"
#include "input.h"
#include <stdio.h>
#include <sys/types.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "stuff.h"

static void move_back(WINDOW * win,int spaces){
	int y,x;
	getyx(win,y,x);
	int my,mx;
	getmaxyx(win,my,mx);
	x -= spaces;
	if(x < 0){
		x *= -1;
		int rows = (x / mx) + 1;
		x = mx - (x % mx);
		y -= rows;
		if(y < 0){
			y = 0;
			x = 0;
		}
	}
	wmove(win,y,x);
}
static void move_forward(WINDOW * win,int spaces){
	int y,x;
	getyx(win,y,x);
	int my,mx;
	getmaxyx(win,my,mx);
	x += spaces;
	if(x >= mx){
		int rows = (x / mx);
		x = (x % mx);
		y += rows;
		if(y >= my){
			y = my - 1;
			x = mx - 1;
		}
	}
	wmove(win,y,x);
}

//variables here
char * head;
char * path;
char * end;
WINDOW * selected_win;
int history_range;
int history_index;

void parse(){
	new_buffer(default_buffer_size);
	//gets input
	wprintw(selected_win,"%s%s%s",head,path,end);
	int prompt_start_y,prompt_start_x;
	getyx(selected_win,prompt_start_y,prompt_start_x);
	while(1){
		if(listen()){
			//non-printable character
			if(keycode == '\n'){
				move_forward(selected_win,strlen(&buffer[buffer_index]));
				wprintw(selected_win,"\n");
				break;
			}
			else if(keycode == KEY_LEFT){
				if(buffer_index){
					buffer_index--;
					move_back(selected_win,1);
				}
			}
			else if(keycode == KEY_RIGHT){
				if(buffer[buffer_index]){
					buffer_index++;
					move_forward(selected_win,1);
				}
			}
			else if(keycode == KEY_UP){
				if(history_index >= 0){
					wmove(selected_win,prompt_start_y,prompt_start_x);
					int i,s;
					s = strlen(buffer);
					for(i = 0; i < s; i++){
						addch(' ');
					}
					wmove(selected_win,prompt_start_y,prompt_start_x);
					free(buffer);
					history_index -= 1;
					if(history_index < 0){
						history_index = history_range - 1;
					}
					buffer = buffer_cpy(dqueue_get(history_index,history));
					wprintw(selected_win,"%s",buffer);
					
				}
			}
			else if(keycode == KEY_DOWN){
				if(history_index >= 0){
					wmove(selected_win,prompt_start_y,prompt_start_x);
					int i,s;
					s = strlen(buffer);
					for(i = 0; i < s; i++){
						addch(' ');
					}
					wmove(selected_win,prompt_start_y,prompt_start_x);
					free(buffer);
					history_index += 1;
					if(history_index >= history_range){
						history_index = 0;
					}
					buffer = buffer_cpy(dqueue_get(history_index,history));
					wprintw(selected_win,"%s",buffer);
					
				}
			}
			else if(keycode == KEY_BACKSPACE){
				if(backspace()){
					move_back(selected_win,1);
				}
				int y,x;
				getyx(selected_win,y,x);
				wprintw(selected_win,"%s ",&buffer[buffer_index]);
				wmove(selected_win,y,x);
			}
			else if(keycode == EOF){
				wprintw(selected_win,"EOF, exiting");
				endwin();
				exit(0);
			}
		}
		else{
			//printable characters	
			buffer_add(key);
			int y,x;
			wprintw(selected_win,"%c",buffer[buffer_index-1]);
			getyx(selected_win,y,x);
			wprintw(selected_win,"%s",&buffer[buffer_index]);
			move(y,x);
		}
		wrefresh(selected_win);
	}
	//puts input into history
	push_input_dqueue(buffer, history);
	//updates history_range and histroy_index
	if(history->dqueue[0] && history->dqueue[1]){
		history_range = history_size;
	}
	else{
		history_range++;
		if(history_range >= history_size){
			history_range = history_size;
		}
	}
	history_index = history_range;
	//makes a copy of the input and parses it
	
	//writes to variables
}

void input_manager_init(){
	input_init(20,10);
	selected_win = stdscr;
	history_index = -1;
	history_range = 0;
	head = "Username:";
	//remember to actually set path
	path = "channel";
	end = "$ ";
}

void tick(){
	parse();
}
