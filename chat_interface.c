#include "chat_interface.h"

char * input_buffer;
int input_buffer_used_length;
dqueue * message_history;
char * last_user;

void init(){
	initscr();
	cbreak();
	noecho();
	int max_y,max_x;
	getmaxyx(stdscr,max_y,max_x);
	create_input_window(max_y, max_x);
	create_output_window(max_y, max_x);
	create_message_window(max_y, max_x);
	create_status_bar(max_y, max_x);
	last_user = (char *)calloc(buffer_size,sizeof(char));
}

int lines_for_input_window(int maxx){
	int i;
	int lines_needed = 0;
	for(i = buffer_size; i >= 0; i -= maxx ){
		lines_needed++;
	}
	return lines_needed;
}

void create_input_window(int maxy, int maxx){
	input_buffer = (char *)calloc(buffer_size,sizeof(char));
	input_buffer_used_length = 0;
	int height = lines_for_input_window(maxx);
	input_win = newwin(height, maxx, maxy - height,0);
	keypad(input_win,TRUE);
}

void create_output_window(int maxy, int maxx){
	int height = maxy - lines_for_input_window(maxx);
	output_win = newwin(height, maxx - status_bar_width,0,0);
	box(output_win,0,0);
}

void create_message_window(int maxy, int maxx){
 	message_history = input_dqueue_init(history_size);
	int height = maxy - lines_for_input_window(maxx) - 2;
	message_win = newwin(height, maxx - status_bar_width - 2,1,1);
	scrollok(message_win,TRUE);
}

void create_status_bar(int maxy, int maxx){
	int height = maxy - lines_for_input_window(maxx);
	status_bar = newwin(height, status_bar_width,0,maxx - status_bar_width);
	keypad(status_bar,TRUE);
	wborder(status_bar,' ',0,0, 0,0,0,0,0);	
}

void display_message(char * header,char * username,char * body){
	if(strcmp(last_user,username)){
		int i,maxx;
		maxx = getmaxx(message_win);
		waddch(message_win,'\n');
		for(i = 0; i < maxx; i++){
			waddch(message_win,'~');
		}
		waddch(message_win,'\n');
		wprintw(message_win,"%s\n-%s-\n%s\n",header,username,body);

	}
	else{
		wprintw(message_win,"%s\n",body);
	}
	strcpy(last_user,username);
	wrefresh(message_win);
}

int main(){
	init();
	refresh();
	wrefresh(input_win);
	wrefresh(output_win);
	wrefresh(message_win);
	wrefresh(status_bar);
	getch();
	display_message("header","username","body");
	getch();
	display_message("header2","username","body2");
	getch();
	display_message("header3","username2","body3");
	getch();
	display_message("header","username","body");
	getch();
	display_message("header2","username","body2");
	getch();
	display_message("header3","username2","body3");
	getch();

	endwin();
	return 0;
}
