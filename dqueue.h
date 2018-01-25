struct input_dqueue{ //the input_dqueue's job is to store a history of inputs
	char ** dqueue;
	int size;
	int start;
	int end;
};

//creates and inits an input_dqueue with size n and returns a pointer to it
struct input_dqueue * input_dqueue_init(int n);

//frees the end of the dequeue
void free_input_dqueue_end(struct input_dqueue *);

//adds to the start of the dequeue
void push_input_dqueue(char * input, struct input_dqueue *);

//prints the dqueue
void print_input_dqueue_array(struct input_dqueue *);

//prints the dqueue in its intended form (start as 0)
void print_input_dqueue(struct input_dqueue *);

//gets the value of index in the dqueue (start as 0)
char * dqueue_get(int index, struct input_dqueue *);
