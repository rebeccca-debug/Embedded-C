#include "uart.h"

enum states{
		STATE_A = 0,
	  STATE_B,
	  STATE_C
}

typedef enum states State_Type;

void state_a_function(void);
void state_b_function(void);
void state_c_function(void);

static void (*state_table[])(void) = {state_a_function,
																			state_b_function,
																			state_c_function
};
static State_Type current_state;
static int Clock;

// Set state, increment clock
int main() {
	
		while(1) {
				state_table[current_state]();
			  Clock++;
		}
			
}

// Initialize state to state A
void state_machine_init(void){
		current_state = STATE_A;
	  Clock = 0;
}

// Execute state A
void state_a_function(void){
	  if (Clock == 2) {
			  // Set state to state B
				current_state = STATE_B;
			  printf("This is the execution of STATE A \n\r");
		}
}

// Execute state B
void state_b_function(void){
		if (Clock == 5) {
			  // Set state to state C
				current_state = STATE_C;
			  printf("This is the execution of STATE B \n\r");
		}
}

// Execute state C
void state_c_function(void){
		if (Clock == 9) {
			  // Set state to state A
				current_state = STATE_A;
			  printf("This is the execution of STATE C \n\r");
		}
}