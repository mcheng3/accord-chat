#ifndef CHAT_INTERFACE
#define CHAT_INTERFACE

#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"dqueue.h"

typedef struct input_dqueue dqueue;/*
we are not going to use this to store input even though the name is
input_dqueue
*/

int buffer_size = 280;
int status_bar_width = 12;
int history_size = 30; //how many messages back do you want to store

WINDOW * input_win;
WINDOW * output_win;//the output window is the only one with a border
WINDOW * status_bar;
WINDOW * message_win;

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

void display_message(char * header,char * username, char *body);/*
displays stuff, yeah
*/

#endif
