#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define stacksize 5012

char productions[6][2][4] = {
	{"E", "E+T"},
	{"E", "T"},
	{"T", "T*F"},
	{"T", "F"},
	{"F", "(E)"},
	{"F", "id"}
};

char terminals[6][3] = {"id", "+", "*", "(", ")", "$"};
int len_terminals = 6;

char non_terminals[3][2] = {"E", "T", "F"};
int len_non_terminals = 3;

char actions[12][6][4] = {
	{"S5", "", "", "S4", "", ""},
	{"", "S6", "", "", "", "ACC"},
	{"", "R2", "S7", "", "R2", "R2"},
	{"", "R4", "R4", "", "R4", "R4"},
	{"S5", "", "", "S4", "", ""},
	{"", "R6", "R6", "", "R6", "R6"},
	{"S5", "", "", "S4", "", ""},
	{"S5", "", "", "S4", "", ""},
	{"", "S6", "", "", "S11", ""},
	{"", "R1", "S7", "", "R1", "R1"},
	{"", "R3", "R3", "", "R3", "R3"},
	{"", "R5", "R5", "", "R5", "R5"}
};

char goto_table[12][3][3] = {
	{"1", "2", "3"},
	{"", "", ""},
	{"", "", ""},
	{"", "", ""},
	{"8", "2", "3"},
	{"", "", ""},
	{"", "9", "3"},
	{"", "", "10"},
	{"", "", ""},
	{"", "", ""},
	{"", "", ""},
	{"", "", ""}
};

int top_stack, top_handle, begin_input, counter;

int stack[stacksize];
int handle[stacksize];
char input[stacksize + 2];
char action[4];

void error() {
	printf("\nWord does not accept!\n");

	exit(0);
}

void accept() {
	printf("\nWord accept!\n");

	exit(0);
}

int str_to_int(char *value) {
	int i, result;

	
	result = 0;
	for(i = 0; value[i] != '\0'; ++i) {
		result += value[i] - '0';

		result *= 10;
	}

	result = (int) result / 10;

	return(result);
}

void stack_push(int value) {
	stack[++top_stack] = value;
}

void handle_push(int value) {
	handle[++top_handle] = value;
}

void stack_pop() {
	--top_stack;
}

void handle_pop() {
	--top_handle;
}

int input_shift() {
	char search_item[3];

	if(input[begin_input] == 'i') {
		++begin_input;
		if(input[begin_input] == 'd') {
			++begin_input;

			strcpy(search_item, "id");
		}
	} else {
		search_item[0] = input[begin_input];
		search_item[1] = '\0';

		++begin_input;
	}

	int i;
	for(i = 0; i < len_terminals; ++i){
		if(!strcmp(terminals[i], search_item)){
			return(i);
		}
	}

	error();
}

int current_token() {
	char search_item[3];

	if(input[begin_input] == 'i') {
		if(input[begin_input + 1] == 'd') {

			strcpy(search_item, "id");
		}
	} else {
		search_item[0] = input[begin_input];
		search_item[1] = '\0';
	}

	int i;
	for(i = 0; i < len_terminals; ++i){
		if(!strcmp(terminals[i], search_item)){
			return(i);
		}
	}

	error();
}

void shift(int state) {
	handle_push(input_shift());
	stack_push(state);
}

void reduce(int production) {
	int len_production, i;
	char reduction;

	if(production == 5){
		len_production = 1;
	} else {
		len_production = strlen(productions[production][1]);
	}

	for(i = 0; i < len_production; ++i) {
		handle_pop();
		stack_pop();
	}

	reduction = productions[production][0][0];

	for(i = 0; i < len_non_terminals; ++i){
		if(non_terminals[i][0] == reduction){
			break;
		}
	}

	handle_push(len_terminals + i);
	stack_push(str_to_int(goto_table[stack[top_stack]][i]));
}

int action_state_production() {
	int i, len_action = strlen(action), result;
	
	result = 0;
	for(i = 1; action[i] != '\0'; ++i) {
		result += action[i] - '0';

		result *= 10;
	}

	result = (int) result / 10;

	return(result);
}

void print() {
	int i, len_input = strlen(input);

	printf("\n");

	printf("%d\n", counter);

	printf("\tStack => %d", stack[0]);
	for(i = 1; i <= top_stack; ++i){
		printf(", %d", stack[i]);
	}
	printf("\n");

	printf("\tHandle => ");
	for(i = 0; i <= top_handle; ++i){
		if(handle[i] > (len_terminals - 1)) {
			printf("%s", non_terminals[handle[i] - len_terminals]);
		} else {
			printf("%s", terminals[handle[i]]);
		}
	}
	printf("\n");

	printf("\tInput => ");
	for(i = begin_input; i < len_input; ++i){
		printf("%c", input[i]);
	}
	printf("\n");

	printf("\tAction => %s\n", action);
	printf("\n=============================================================\n");
}

int main(void) {
	int state, production, token;

	begin_input = 0;
	top_stack = top_handle = -1;

	stack_push(0);

	scanf("%s", input);

	printf("\n=============================================================\n");

	strcat(input, "$");

	counter = 0;
	while(1) {
		counter += 1;

		state = stack[top_stack];
		token = current_token();
		strcpy(action, actions[state][token]);

		print();

		if(!strcmp(action, "")) {
			error();
		} else if(!strcmp(action, "ACC")) {
			accept();
		} else if(action[0] == 'S') {
			state = action_state_production();
			shift(state);
		} else if(action[0] == 'R') {
			production = action_state_production();
			reduce(production - 1);
		}
	}

	return(0);
}