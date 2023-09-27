#include "stm32l476xx.h"
#include "SysTick.h"
#include "LED.h"
#include "LCD.h"

void System_Clock_Init(void){
	
	RCC->CR |= RCC_CR_MSION; 
	
	// Select MSI as the clock source of System Clock
	RCC->CFGR &= ~RCC_CFGR_SW; 
	
	// Wait until MSI is ready
	while ((RCC->CR & RCC_CR_MSIRDY) == 0); 	
	
	// MSIRANGE can be modified when MSI is OFF (MSION=0) or when MSI is ready (MSIRDY=1). 
	RCC->CR &= ~RCC_CR_MSIRANGE; 
	RCC->CR |= RCC_CR_MSIRANGE_7;  // Select MSI 8 MHz	
 
	// The MSIRGSEL bit in RCC-CR select which MSIRANGE is used. 
	// If MSIRGSEL is 0, the MSIRANGE in RCC_CSR is used to select the MSI clock range.  (This is the default)
	// If MSIRGSEL is 1, the MSIRANGE in RCC_CR is used. 
	RCC->CR |= RCC_CR_MSIRGSEL; 
	
	// Enable MSI and wait until it's ready	
	while ((RCC->CR & RCC_CR_MSIRDY) == 0); 		
}

int generateRandNum(int seed) {
	seed = seed + 77; 
	seed = seed / 2; 
	return seed; 
}

int* generateRandArr(int randNum) {
	int arr[8];
	int i; 
	
	arr[0] = randNum % 4;
	
	for (i = 1; i < 8; i++) {
	//	arr[i] = generateRandNum(arr[0]) % 4;
		//arr[i] = (arr[0] + 2) % 4;
		arr[i] = 1;
	}
	return arr;
}

void readInput() {
	//if (GPIOA->IDR & GPIO_IDR_IDR_0) {
	//			return time;
	//}
}

void gameOver(int score) {
	char scoreStr[2]; 
	char msg[23] = "GAME OVER. SCORE ";
	
	sprintf(scoreStr, "%d", score);
	
	strncat(msg, &scoreStr, 3);
	
	LCD_DisplayText(msg, 25);
}

void startGame(int randNum, int phase, int score) {	
	//Convert each element from int to string
	int i; 
	char strArr[6];
	
	int randArr[8]; 

	randArr[0] = randNum % 4;
	
	for (i = 1; i < phase; i++) {
	//	arr[i] = generateRandNum(arr[0]) % 4;
		randArr[i] = (((randArr[i-1] + randNum) * i * randArr[0]) + i) % 4;
		//randArr[i] = 1;
	}
	
	//str = (char*) malloc(1);
	
	//Display generated numbers
//	for (i = 0; i < 6; i++) {
//		sprintf(strArr+i, "%d", randArr[i]);
//		LCD_WriteChar(strArr+i,0,0,i);
//		delay(1000); 
//	}
	for (i = 0; i < phase; i++) {
		if (randArr[i] == 0) {
			LCD_DisplayString("UP    ");
		}
		else if (randArr[i] == 1) {
			 LCD_DisplayString("RIGHT ");
		}
		else if (randArr[i] == 2) {
			LCD_DisplayString("DOWN  ");
		}
		else if (randArr[i] == 3) {
			LCD_DisplayString("LEFT  ");
		}
		
		delay(4000);
		LCD_DisplayString("       ");
		delay(100);
	}
	
	//Compare answers
	LCD_DisplayString("         ");
	for (i = 0; i < phase; i++) {
		if (readInp() == randArr[i]) {
			LCD_DisplayString("GOOD   ");
			score++;
		}
		else {
			LCD_DisplayString("OOPS!   ");
			delay(2000);
			while(1) {
				gameOver(score);
			}
		}
		delay(1000);
		LCD_DisplayString("      ");
	}
	
	//Loop again 
	startGame(randArr[0], phase + 1, score);
	//LCD_DisplayText("GOOD JOB YOU WON", 18);
	return;
}

void printScreen(char str) {
		char * ptr; 
		ptr = &str;
		LCD_WriteChar((uint8_t *) ptr,0,0,0);
}

int main(void){
	bool startBool = 0;
	int randNum = 0; 
	int time = 0; 
	char* str;
	
	int inputNum;
	char inputNumStr;
	char * ptr;
	str = (char*) malloc(25);
	
	//Initialize LCD 
	LCD_Initialization();
	
	//Init SysTick
	//System_Clock_Init(); // Switch System Clock = 80 MHz

	//For a 4MHz clock the delay should be 1000 
	//However  we are using an external clock (1MHz), so we are using
	//1000 as the tick. 
	SysTick_Initialize(1000); //setting 1 tick = 1000 clock cycles
	
	//uint8_t * startMsg = "Start"; 
	//LCD_DisplayString("       ");
		//Allocate memory to str
	
	strcpy(str, "Press to start");
	randNum = startScreen(str, 14);
	
	startGame(randNum, 2, 0); 
	
	while(1);
}
