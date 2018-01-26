#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "game.h"

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
    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);
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

}

int react(){
    initscr();
    noecho();
    cbreak();
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
            //return 0;
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
    return 0;
}

int main(int argc, char *argv[]) {

 // Global var `stdscr` is created by the call to `initscr()`

    //char ch;
    //char *str = malloc(sizeof(char));
    hangman("cool");
    hangman("hello world");
    hangman("potatoes");
    hangman("keyboard qwerty");
    //hangman("");
    //react();
    endwin();
}
