#include "uart.h"
#include "stm32f4xx_hal.h"

enum states{
	  STATE_A = 0,
	  STATE_B,
	  STATE_C
};

typedef enum states State_Type;

void state_machine_init(void);
void state_a_function(void);
void state_b_function(void);
void state_c_function(void);
void SysTick_Handler(void);

static void (*state_table[])(void) = {state_a_function,
																			state_b_function,
																			state_c_function
};
static State_Type current_state;
static int Clock;

// Set state of clock at each second
int main() {
	// Hal library uses the SysTick timer as a clock piece
	HAL_Init();
	
	USART2_Init();
	
	while(1) {
		state_table[current_state]();
			
		HAL_Delay(1000);
		Clock++;
	}
			
}

// Initialize state to state A
void state_machine_init(void){
	  current_state = STATE_A;
	  Clock = 0;
}

uint32_t sa_prev_time = 0;
uint32_t sa_now;
float sa_delta;				//Difference between current and previous time

// Execute state A
void state_a_function(void){
	if (Clock == 2) {
		// Set state to state B
		current_state = STATE_B;
			
		sa_now = HAL_GetTick();
			
		sa_delta = sa_now - sa_prev_time;
		sa_prev_time = sa_now;
		printf("This is the output of STATE A : %f seconds ago \n\r",sa_delta);
		//printf("This is the execution of STATE A \n\r");
	}
}

uint32_t sb_prev_time = 0;
uint32_t sb_now;
float sb_delta;

// Execute state B
void state_b_function(void){
	if (Clock == 5) {
		// Set state to state C
		current_state = STATE_C;
			
		sb_now = HAL_GetTick();
		sb_delta = sb_now - sb_prev_time;
		sb_delta /= 1000;
		sb_prev_time = sb_now;
		printf("This is the output of STATE B : %f seconds ago\n\r",sb_delta);
		//printf("This is the execution of STATE B \n\r");
	}
}

uint32_t sc_prev_time = 0;
uint32_t sc_now;
float sc_delta;

// Execute state C
void state_c_function(void){
	if (Clock == 9) {
		Clock = 0;
		// Set state to state A
		current_state = STATE_A;
		sc_now = HAL_GetTick();
		sc_delta = sc_now - sc_prev_time;
		sc_delta /= 1000;
		sc_prev_time = sc_now;
		// printf("This is the execution of STATE C \n\r");
		printf("This is the output of STATE C : %f seconds ago\n\r",sc_delta);
	}
}

void SysTick_Handler(void){
	HAL_IncTick();
}
