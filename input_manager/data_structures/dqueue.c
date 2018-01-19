#include<stdio.h>
#include<stdlib.h>
#include "dqueue.h"

struct input_dqueue * input_dqueue_init(int n){
	struct input_dqueue * in_dqueue = (struct input_dqueue *)malloc(sizeof(struct input_dqueue));
	in_dqueue->size = n;
	in_dqueue->start = 0;
	in_dqueue->end = 0;
	in_dqueue->dqueue = (char **)calloc(n,sizeof(char *));
	in_dqueue->dqueue[0] = (char *)NULL;
	return in_dqueue;
}

void push_input_dqueue(char * input, struct input_dqueue * in_dqueue){
	if(in_dqueue->dqueue[in_dqueue->start]){
		in_dqueue->start -= 1;
		if(in_dqueue->start < 0){
			in_dqueue->start += in_dqueue->size;
		}
		if(in_dqueue->start == in_dqueue->end){
			free_input_dqueue_end(in_dqueue);
		}
	}
	in_dqueue->dqueue[in_dqueue->start] = input;
}

void free_input_dqueue_end(struct input_dqueue * in_dqueue){
	free(in_dqueue->dqueue[in_dqueue->end]);
	in_dqueue->end -= 1;
	if(in_dqueue->end < 0){
		in_dqueue->end += in_dqueue->size;
	}
}

void print_input_dqueue_array(struct input_dqueue * in_dqueue){
	int i;
	for(i = 0; i < in_dqueue->size; i++){
		printf("%d: %s\n",i,in_dqueue->dqueue[i]);
	}
	printf("start: %d, end: %d\n",in_dqueue->start,in_dqueue->end);
}

void print_input_dqueue(struct input_dqueue * in_dqueue){
	int i;
	for(i = 0; i < in_dqueue->size; i++){
		printf("%d: %s\n",i,dqueue_get(i,in_dqueue));
	}
}

char* dqueue_get(int index,struct input_dqueue * in_dqueue){
	return in_dqueue->dqueue[(in_dqueue->end + (in_dqueue->size - index)) % in_dqueue->size];
}

//for testing purposes
/*
int main(){
	struct input_dqueue * buffer = input_dqueue_init(5);
	int i;
	for(i = 0; i < 15; i++){
		char * str;
		scanf("%ms",&str);
		push_input_dqueue(str,buffer);
		print_input_dqueue(buffer);
		printf("actual array\n");
		print_input_dqueue_array(buffer);
	} 
}
*/
