#include "uart.h"

void USART2_Init(void){

//1. Enable clock access to uart2
	RCC->AHB1ENR |= 0x20000;
//2. Enable clock access to port A
	RCC->AHB1ENR = 0x001;
//3. Enable pins for alternate functions, pa2, pa3

	GPIOA->MODER &=~0x00F0;
	GPIOA->MODER |= 0x00A0; /*Enable alt. Function for PA2, PA3*/
//4. Configure type of alternate function
	
	GPIOA->AFR[0] &= ~0xFF00;
	GPIOA->AFR[0] |= 0x7700;

//Configure uart

	USART2->BRR = 0x0683;
	USART2->CR1 = 0x000C; //control register 1 - enable tx rx, set to 8 bit data
	USART2->CR2 = 0x000;  //control register 2 - pass default value
	USART2->CR3	= 0x000;	//control register 3 - no flow control
	USART2->CR1 = 0x2000; //enable usart module by going back to CR1
	
}

/* While the transfer buffer is not empty, wait.
   While the transfer buffer is empty, add character to data register. */
int USART2_write(int ch){
	//SR - status register
	while(! (USART2->SR & 0x0080) ){}
	//DR - data register
	USART2->DR = (ch & 0xFF);
	//Return character for confirmation
	return ch;
}

/* While transfer buffer is not empty, wait.
	 Then, return the contents from the data register.
*/
int USART2_read(void) {
	while(!(USART2->SR & 0x0020)){}
	//return contents of the data register
	return USART2->DR;
}

/* The code below is the interface to the C standard I/O library.
 * All the I/O are directed to the console.
 */
//struct __FILE { int handle; };
FILE __stdin = {0};
FILE __stdout = {1};
FILE __stderr = {2};

int fgetc(FILE *f) {
	int c;
	
	c = USART2_read();	/* Read the character from the console*/
	
	if (c == '\r') {		/* If '\r', after it is echoed, a '\n' is appended */
		USART2_write(c);	/* echo */
		c = '\n';
	}
	
	USART2_write(c);		/* echo */
	
	return c;
}

/* Called by C library console/file output */
int fputc(int c, FILE *f) {
	return USART2_write(c); /* Write the character to console */
}

int n;
char str[80];

void test_setup(void){
	printf("please enter a number: ");
	scanf("%d", &n);
	printf("the number entered is: %d\r\n", n);
	printf("please type a character string: ");
	gets(str);
	printf("the character string entered is: ");
	puts(str);
	printf("\r\n");
}