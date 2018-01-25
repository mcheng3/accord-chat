#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define DELAY 30000


int hangman(char *phrase){
	int i;
	int x = 0, y = 0;
	int max_y = 0, max_x = 0;
	int next_x = 0;
	int direction = 1;
	getmaxyx(stdscr, max_y, max_x);
	int moves_wrong = 0;
	for(i=0; i<8; i++){
		mvprintw(max_y/2-10, max_x/5-6+i, "=");
	}
	for(i=0; i<8; i++){
		mvprintw(max_y/2-i-2, max_x/5-6, "|");
	}
	for(i=0; i<10; i++){
		mvprintw(max_y/2-2, max_x/5-6+i, "=");
	}
	mvprintw(max_y/2-9, max_x/5, "|");
	
	while(1) {
		
		char ch;
		ch = getch();	
		/*char *strnew = malloc(sizeof(char));
		strnew = &ch;
		if (strcmp(strnew, str) != 0){
			str = strnew;
		}*/		
		if(ch != ' ' && moves_wrong < 7){
			moves_wrong++;
		}	
		
		int man_y[] = {max_y/2-8,max_y/2-7,max_y/2-7,max_y/2-7,max_y/2-6,max_y/2-5,max_y/2-5};
		int man_x[] = {max_x/5, max_x/5, max_x/5-1, max_x/5+1, max_x/5,max_x/5-1,max_x/5+1};
		char *man_parts[] = {"O", "|", "\\", "/", "|", "/", "\\"};
		for(i=0;i<moves_wrong;i++){
			mvprintw(man_y[i],man_x[i],man_parts[i]);
		}
		
	    mvprintw(max_y/2, max_x/5+5, phrase);	
		refresh();

		usleep(DELAY);
	}
}

int main(int argc, char *argv[]) {
	

	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);

 // Global var `stdscr` is created by the call to `initscr()`

	char ch;
	char *str = malloc(sizeof(char));
	hangman("potato");

	endwin();
}
