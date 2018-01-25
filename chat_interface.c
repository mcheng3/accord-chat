#include "chat_interface.h"

char * input_buffer;
int input_buffer_used_length;
int input_buffer_cursor_pos = 0;
dqueue * message_header_history;
dqueue * message_username_history;
dqueue * message_body_history;
char * last_user;

void init(){
	time_t t;
	srand((unsigned)time(&t));
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
	mvwprintw(output_win,1,1,"output");
}

void create_message_window(int maxy, int maxx){
 	message_header_history = input_dqueue_init(history_size);
	message_username_history = input_dqueue_init(history_size);
	message_body_history = input_dqueue_init(history_size);
	int height = maxy - lines_for_input_window(maxx) - 2;
	message_win = newwin(height, maxx - status_bar_width - 2,1,1);
	scrollok(message_win,TRUE);
	wprintw(message_win,"messages\n");
}

void create_status_bar(int maxy, int maxx){
	int height = maxy - lines_for_input_window(maxx);
	status_bar = newwin(height, status_bar_width,0,maxx - status_bar_width);
	keypad(status_bar,TRUE);
	wborder(status_bar,' ',0,0,0,0,0,0,0);	
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
	push_input_dqueue(header,message_header_history);
	push_input_dqueue(username,message_username_history);
	push_input_dqueue(body,message_body_history);
}

char * pop_input_buffer(){
	if(ready_to_send){
		char * send = input_buffer;
		input_buffer = (char *)calloc(buffer_size,sizeof(char));
		return send;
	}
	else{
		return NULL;
	}
}

static void calculate_cursor_pos(int * cury,int * curx){

}

static void input_insert(char c, int pos){

}

static void input_delete(int pos){

}

void tick(){
	if(ready_to_send){
		return;
	}
	else{
		int cury,curx;
		calculate_cursor_pos(&cury,&curx);
		int c = mvwgetch(input_win,cury,curx);
		switch(c){
			case KEY_LEFT :
				if(input_buffer_cursor_pos > 0){
					input_buffer_cursor_pos--;
				}
				break;

			case KEY_RIGHT :
				if(input_buffer_cursor_pos < buffer_size && 
					input_buffer_cursor_pos < input_buffer_used_length){
					input_buffer_cursor_pos++;
				}
				break;

			case KEY_UP :
				break;

			case KEY_DOWN :
				break;

			case KEY_BACKSPACE :
				if(input_buffer_cursor_pos > 0){
					input_buffer_cursor_pos--;
				}
				input_delete(input_buffer_cursor_pos);
				break;

			case KEY_DC :
				if(input_buffer_cursor_pos > 0){
					input_buffer_cursor_pos--;
				}
				input_delete(input_buffer_cursor_pos);
				break;

			case 127 :
				if(input_buffer_cursor_pos > 0){
					input_buffer_cursor_pos--;
				}
				input_delete(input_buffer_cursor_pos);
				break;

			default :
				if(isprint(c) && input_buffer_used_length < buffer_size){
					input_insert(c,input_buffer_cursor_pos);
					input_buffer_cursor_pos++;
					input_buffer_used_length++;
				}
		}
		clearok(input_win,TRUE);
		mvwprint(input_win,0,0,input_buffer);
		wrefresh(input_win);
		return;
	}
}

int main(){
	init();
	refresh();
	wrefresh(input_win);
	wrefresh(output_win);
	wrefresh(message_win);
	wrefresh(status_bar);
	getch();
	endwin();
	return 0;
}
