#ifndef CHAT_INTERFACE
#define CHAT_INTERFACE

#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include<poll.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/time.h>
#include"dqueue.h"

typedef struct input_dqueue dqueue;/*
we are not going to use this to store input even though the name is
input_dqueue
*/

struct pollfd poll_structs[1];

int display_pipe_fd;
int buffer_size = 255;/*also body size, actual buffer size is buffer_size + 1
this is due to the null byte
*/
int status_bar_width = 12;
int history_size = 30; //how many messages back do you want to store
int header_size = 20;
int username_size = 20;

WINDOW * input_win;
WINDOW * output_win;//the output window is the only one with a border
WINDOW * status_bar;
WINDOW * message_win;

int ready_to_send = 0;//if this is true then you can call pop_input_buffer()

void init();/*
sets up ncurses and creates windows for the chat
*/

int lines_for_input_window(int maxx);/*
calculates the how high the input window will be given the width of the window
*/

void create_input_window(int maxy, int maxx);/*
creates the input window and sets input_win to point to it
creates and sets input_buffer
takes care of setting up window config
*/

void create_output_window(int maxy, int maxx);/*
creates the output window and sets output_win to point to it
draws the border
takes care of setting up window config
*/

void create_message_window(int maxy, int maxx);/*
creates the message window and sets message_win to point to it
sets up the history dqueue
takes care of setting up window config
*/

void create_status_bar(int maxy, int maxx);/*
creates the status bar and sets status_bar to point to it
takes care of setting up window config
draws border
*/

void display_message(char * username, char *body);/*
displays stuff, yeah
*/

char * pop_input_buffer();/*
returns the input_buffer and creates a new one for the next input
returns NULL if ready_to_send is false
remember to free the string obtained by this function
*/

void set_display_pipe(int pipefd);/*
prepares the struct for poll
*/

void tick();/*
does everything
except update the display
*/

int update_display_through_pipe();/*
exactly what it says
returns return value of poll
which is 0 if timed out and something bigger than 0 if not
*/

void refresh_all();/*
exactly what it says
*/

#endif
