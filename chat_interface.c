#include "chat_interface.h"
#include "networking.h"

char * input_buffer;
int input_buffer_used_length;
int input_buffer_cursor_pos = 0;
dqueue * message_header_history;
dqueue * message_username_history;
dqueue * message_body_history;
char * last_user;

void strreplace(char s[], char s1[],  char chr);
int hangman(char *phrase);
int react();

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
	last_user = (char *)calloc(buffer_size + 1,sizeof(char));
}

int lines_for_input_window(int maxx){
	int i;
	int lines_needed = 0;
	for(i = buffer_size + 1; i >= 0; i -= maxx ){
		lines_needed++;
	}
	return lines_needed;
}

void create_input_window(int maxy, int maxx){
	input_buffer = (char *)calloc(buffer_size + 1,sizeof(char));
	input_buffer_used_length = 0;
	int height = lines_for_input_window(maxx);
	input_win = newwin(height, maxx, maxy - height,0);
	keypad(input_win,TRUE);
	wtimeout(input_win,200);
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
	wprintw(message_win,"messages\ntry using arrow keys and backspace :)");
}

void create_status_bar(int maxy, int maxx){
	int height = maxy - lines_for_input_window(maxx);
	status_bar = newwin(height, status_bar_width,0,maxx - status_bar_width);
	keypad(status_bar,TRUE);
	wborder(status_bar,' ',0,0,0,0,0,0,0);	
}

void display_message(char * username,char * body){
	if(strcmp(last_user,username)){
		int i,maxx;
		maxx = getmaxx(message_win);
		waddch(message_win,'\n');
		for(i = 0; i < maxx; i++){
			waddch(message_win,'~');
		}
		waddch(message_win,'\n');
		wprintw(message_win,"%s-\n%s\n",username,body);
	}
	else{
		wprintw(message_win,"%s\n",body);
	}
	strcpy(last_user,username);
	wrefresh(message_win);
//	push_input_dqueue(header,message_header_history);
//	push_input_dqueue(username,message_username_history);
//	push_input_dqueue(body,message_body_history);
}

char * pop_input_buffer(){
	if(ready_to_send){
		char * send = input_buffer;
		input_buffer = (char *)calloc(buffer_size + 1,sizeof(char));
		ready_to_send = 0;
		input_buffer_used_length = 0;
		input_buffer_cursor_pos = 0;
		return send;
	}
	else{
		return NULL;
	}
}

static void calculate_cursor_pos(int * cury,int * curx){
	int maxx = getmaxx(input_win);
	*cury = (input_buffer_cursor_pos - (input_buffer_cursor_pos % maxx)) / maxx;
	*curx = input_buffer_cursor_pos % maxx;
}

static void input_insert(char c, int pos){
	char temp;
	for(;pos < buffer_size; pos++){
		temp = input_buffer[pos];
		input_buffer[pos] = c;
		c = temp;
	}
}

static void input_delete(int pos){
	int i;
	char temp;
	char c = '\0';
	for(i = buffer_size - 1; i >= pos; i--){
		temp = input_buffer[i];
		input_buffer[i] = c;
		c = temp;
	}
}

void set_display_pipe(int pipefd){
	memset(poll_structs,0,sizeof(poll_structs));
	poll_structs[0].fd = pipefd;
	poll_structs[0].events = POLLIN;
	poll_structs[0].revents = 0;
}

void tick(){
	static unsigned int sleepframes = 0;
	if(ready_to_send){
		usleep(500000);
		if(sleepframes++ >= 2){
			waddch(input_win,'.');
			sleepframes = 0;
			wrefresh(input_win);
		}
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
				input_buffer_used_length--;
				break;

			case KEY_DC :
				if(input_buffer_cursor_pos > 0){
					input_buffer_cursor_pos--;
				}
				input_delete(input_buffer_cursor_pos);
				input_buffer_used_length--;
				break;

			case 127 :
				if(input_buffer_cursor_pos > 0){
					input_buffer_cursor_pos--;
				}
				input_delete(input_buffer_cursor_pos);
				break;

			case '\n' :
				ready_to_send = 1;
				werase(input_win);
				mvwprintw(input_win,0,0,"sending...");
				wrefresh(input_win);
				return;

			default :
				if(isprint(c) && input_buffer_used_length < buffer_size){
					input_insert(c,input_buffer_cursor_pos);
					input_buffer_cursor_pos++;
					input_buffer_used_length++;
				}
		}
		werase(input_win);
		mvwprintw(input_win,0,0,input_buffer);
		int cury2,curx2;
		calculate_cursor_pos(&cury2,&curx2);
		wmove(input_win,cury2,curx2);
		wrefresh(input_win);
		return;
	}
}

int update_display_through_pipe(){
	int retpoll = poll(poll_structs,1,0);
	if(retpoll > 0){
		if(poll_structs[0].revents & POLLIN){
			char * header = (char *)malloc(header_size * sizeof(char));
			char * username = (char *)malloc(username_size * sizeof(char));
			char * body = (char *)malloc((buffer_size + 1) * sizeof(char));
			read(display_pipe_fd,header,header_size);
			read(display_pipe_fd,username,username_size);
			read(display_pipe_fd,body,buffer_size + 1);
			display_message(username,body);
		}
	}
	return retpoll;
}

void refresh_all(){
	refresh();
	wrefresh(input_win);
	wrefresh(output_win);
	wrefresh(message_win);
	wrefresh(status_bar);
}

#define DELAY 30000

void strreplace(char s[], char s1[],  char chr){
    int i=0;
    while(s[i]!='\0')
    {
        if(s[i]==chr)
        {
            s1[2*i]=chr;
        }  
        i++; 
    }
    //mvprintw(0, 0, "%s",s1);
}

int hangman(char *phrase){
	erase();
    curs_set(FALSE);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    wbkgd( stdscr ,COLOR_PAIR(2));
    clear();
    nodelay(stdscr, FALSE);
    char *f = malloc(sizeof(phrase) * 2);
    int i;
    strcpy(f, phrase);
    int x = 0, y = 0;
    int max_y = 0, max_x = 0;
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(3, max_x/2-4, "Hangman");
    int moves_wrong = 0;
    for(i=0; i<8; i++){
        mvprintw(max_y/2-5, max_x/5-6+i, "=");
    }
    for(i=0; i<8; i++){
        mvprintw(max_y/2-i+3, max_x/5-6, "|");
    }
    for(i=0; i<10; i++){
        mvprintw(max_y/2+3, max_x/5-6+i, "=");
    }
    mvprintw(max_y/2-4, max_x/5, "|");
    
    char *blanks= (char *)malloc(sizeof(phrase) * 3); 
    for(i=0; i<strlen(phrase); i++){
        strcat(blanks, "_ ");
    }

    strreplace(phrase, blanks, ' ');
    
    //printf("%s\n", blanks);
    //mvprintw(max_y/2, max_x/5+4, blanks);
        // mvprintw(max_y/2, max_x/5+15, phrase);
    int solved = 0;
    int man_y[] = {max_y/2-3,max_y/2-2,max_y/2-2,max_y/2-2,max_y/2-1,max_y/2,max_y/2};
    int man_x[] = {max_x/5, max_x/5, max_x/5-1, max_x/5+1, max_x/5,max_x/5-1,max_x/5+1};
    char *man_parts[] = {"O", "|", "\\", "/", "|", "/", "\\"};

    while(moves_wrong<7 && !solved) {

        mvprintw(max_y/2, max_x/5+15, blanks);

        char ch;
        ch = getch();   
        /*char *strnew = malloc(sizeof(char));
        strnew = &ch;
        if (strcmp(strnew, str) != 0){
            str = strnew;
        }*/
        if(ch <= 'z' && ch >= 'a'){
            strreplace(f, blanks, ch);
            if(ch != ' ' && moves_wrong < 7 && !strchr(phrase, ch)){
                moves_wrong++;
            }
            if(!strchr(blanks, '_')){
                mvprintw(max_y/2+5, max_x/5+13, "You are a winner!");
                mvprintw(max_y/2, max_x/5+15, blanks);
                refresh();
                sleep(3);
                solved = 1;
                //return 0;
            }

            for(i=0;i<moves_wrong;i++){
                mvprintw(man_y[i],man_x[i],man_parts[i]);
            }
            mvprintw(max_y/2, max_x/5+15, blanks);

            refresh();

            usleep(DELAY);
        }
    }
    if(moves_wrong == 7){
        mvprintw(max_y/2+5, max_x/5+13, "You are a loser!");
        refresh();
        sleep(3);
        //return 0;
    }
    
    sleep(2);
    //react();
    //return 0;
	curs_set(TRUE);

}

static void after_game_clean_up(){
	erase();
	refresh();
	werase(input_win);
	werase(output_win);
	werase(message_win);
	werase(status_bar);
	wborder(status_bar,' ',0,0,0,0,0,0,0);
	box(output_win,0,0);
	refresh_all();
}

int react(){
	erase();
    curs_set(FALSE);
    clear();
    int x = 0, y = 0;
    int max_y = 0, max_x = 0;
    getmaxyx(stdscr, max_y, max_x);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);
    wbkgd( stdscr ,COLOR_PAIR(1));
    refresh();
    srand(time(NULL));
    int r = rand() % 50000 + 30000;
    nodelay(stdscr, TRUE);

    while(r){
        if(getch() == ' '){
            erase();
            refresh();
            mvprintw(max_y/2, max_x/2-10, "You pressed too early!");
            refresh();
            sleep(2);
            return 0;
            //
        }
        else{
        //mvprintw(max_y/2, max_x/2-10, "%d", r);
            mvprintw(max_y/2, max_x/2-20, "Press spacebar when the screen turns green");
            refresh();
            usleep(1);
            r--;
        }
    }

    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    wbkgd( stdscr ,COLOR_PAIR(2));
    struct timeval start, end;
    gettimeofday(&start, NULL);
    while(getch() != ' ');
    gettimeofday(&end, NULL);
    erase();
    refresh();
    mvprintw(max_y/2, max_x/2-10, "You took %ld milliseconds\n", (((end.tv_sec - start.tv_sec)*1000000L +end.tv_usec) - start.tv_usec)/1000);
    refresh();
    sleep(6);
	curs_set(TRUE);
    return 0;
}
int hang = 0;
int main(int argc, char **argv) {

	int server_socket;
	char write_buffer[256];
	char read_buffer[256];

	if (argc == 2)
		server_socket = client_setup( argv[1]);
	else
		server_socket = client_setup( CS16 );

	init();
	refresh_all();
	set_display_pipe(server_socket);
	while(1){
		int retpoll = poll(poll_structs,1,0);
		if(retpoll > 0){
			if(poll_structs[0].revents & POLLIN){
				read(server_socket, read_buffer, sizeof(read_buffer));
				if(hang){
					//parse read_buffer here
					int hasBrackets = 0;
					char word[50];
					if(hasBrackets){
						hangman(word);
						hang = 0;
						after_game_clean_up();
					}
				}
				display_message("some_username",read_buffer);
			}
		}
		tick();
		//*strchr(write_buffer, '\n') = 0;
		if(ready_to_send){
			char * s = pop_input_buffer();
			write(server_socket, s, sizeof(write_buffer));
			if(strcmp(s,"!react") == 0){
				react();
				after_game_clean_up();
			}
			else if(strcmp(s,"!hangman") == 0){
				hang = 1;
				display_message("hangedman","waiting for word");
				write(server_socket,"playing hangman, waiting for word, put word in brackets plz []",sizeof(write_buffer));
			}
			free(s);
		}
	}
}
