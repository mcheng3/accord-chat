#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DELAY 30000

void strreplace(char s[], char s1[],  char chr, char repl_chr)
{
     int i=0;
     while(s[i]!='\0')
     {
           if(s[i]==chr)
           {
               s1[i]=repl_chr;
           }  
           i++; 
     }
     //printf("%s",s);
}

int hangman(char *phrase){
  char *f = malloc(sizeof(char *));
  int i;
  strcpy(f, phrase);
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
	char *length = malloc(sizeof(int));
	sprintf(length, "%d", strlen(phrase));
	char *blanks=malloc(strlen(phrase)); 
	for(i=0; i<strlen(phrase); i++){
	  strcat(blanks, "_");
	}
	//mvprintw(max_y/2, max_x/5+4, blanks);
        // mvprintw(max_y/2, max_x/5+15, phrase);
	int solved = 0;
	while(moves_wrong<7 && !solved) {
		
		char ch;
		ch = getch();	
		/*char *strnew = malloc(sizeof(char));
		strnew = &ch;
		if (strcmp(strnew, str) != 0){
			str = strnew;
		}*/
	  
		strreplace(f, blanks, ch, ch);
		if(ch != ' ' && moves_wrong < 7 && !strchr(phrase, ch)){
			moves_wrong++;
		}
		if(!strchr(blanks, '_')){
		  mvprintw(max_y/2+5, max_x/5+10, "You are a winner!");
		   mvprintw(max_y/2, max_x/5+15, blanks);
		  refresh();
		  sleep(2);
		  solved = 1;
   
		}
		int man_y[] = {max_y/2-8,max_y/2-7,max_y/2-7,max_y/2-7,max_y/2-6,max_y/2-5,max_y/2-5};
		int man_x[] = {max_x/5, max_x/5, max_x/5-1, max_x/5+1, max_x/5,max_x/5-1,max_x/5+1};
		char *man_parts[] = {"O", "|", "\\", "/", "|", "/", "\\"};
		for(i=0;i<moves_wrong;i++){
			mvprintw(man_y[i],man_x[i],man_parts[i]);
		}
		 mvprintw(max_y/2, max_x/5+15, blanks);
	   	
		refresh();

		usleep(DELAY);
	}
	if(moves_wrong ==7){
	  mvprintw(max_y/2+5, max_x/5+10, "You are a loser!");
	}
	 refresh();
	 sleep(2);
}

int react(){
  int x = 0, y = 0;
  int max_y = 0, max_x = 0;
  	getmaxyx(stdscr, max_y, max_x);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_RED);
  wbkgd( stdscr ,COLOR_PAIR(1));
  refresh();
  srand(time(NULL));
  int r = rand() % 10000000 + 1000000;
  usleep(r);
  init_pair(2, COLOR_WHITE, COLOR_GREEN);
  wbkgd( stdscr ,COLOR_PAIR(2));
  time_t start = time(NULL);
  time_t end;
  while(getch() != ' '){
    usleep(10);
    end = time(NULL);

  }
  mvprintw(max_y/2, max_x/2, "Took %6.3f seconds\n", difftime(end, start));
  
  refresh();
  sleep(6);
}


int main(int argc, char *argv[]) {
    
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);

 // Global var `stdscr` is created by the call to `initscr()`

	char ch;
	char *str = malloc(sizeof(char));
	//hangman("unpredictably");
	react();
	endwin();
}
