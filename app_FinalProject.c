/*
 * app_FinalProject.c
 *
 * Created on: 11/13/2024
 * Author: Benjamin Goldberg
 */

/* Includes */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "app.h"

/* Private Define */
#define		CNTRL_PORT		GPIOA
#define		CNTRL_PIN		GPIO_PIN_15

/* Private Function Prototypes */
void ShowCommands(void);
void UART_TransmitString(UART_HandleTypeDef *p_huart, char a_string[], int newline);

/* Private Variables */
extern UART_HandleTypeDef huart2;

//Should be declared as volatile if variables' values are changed in ISR.
volatile char rxData;  //One byte data received from UART

void App_Init() {
	HAL_GPIO_WritePin(CNTRL_PORT, CNTRL_PIN, GPIO_PIN_SET);
	UART_TransmitString(&huart2, "-----------------", 1);
	UART_TransmitString(&huart2, "~ Nucleo-L476RG ~", 1);
	UART_TransmitString(&huart2, "-----------------", 1);

	ShowCommands();

	HAL_UART_Receive_IT(&huart2, (uint8_t*) &rxData, 1); //Start the Rx interrupt.

}

void App_MainLoop() {
	// Nothing done in Main Loop
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *p_huart) {
	//Process the data received from UART.
	switch (rxData) {
	case 'I':
	case 'i':
		HAL_GPIO_WritePin(CNTRL_PORT, CNTRL_PIN, GPIO_PIN_RESET);
		break;
	case 'O':
	case 'o':
		HAL_GPIO_WritePin(CNTRL_PORT, CNTRL_PIN, GPIO_PIN_SET);
		break;
	}

	HAL_UART_Receive_IT(p_huart, (uint8_t*) &rxData, 1); //Restart the Rx interrupt.
}

void ShowCommands(void) {
	UART_TransmitString(&huart2, "Type on keyboard to send command from PC to MCU:", 1);
	UART_TransmitString(&huart2, "> I: turn on Plug, O: turn off Plug, H: show commands", 1);
}

void UART_TransmitString(UART_HandleTypeDef *p_huart, char a_string[], int newline) {
	HAL_UART_Transmit(p_huart, (uint8_t*) a_string, strlen(a_string), HAL_MAX_DELAY);
	if (newline != 0) {
		HAL_UART_Transmit(p_huart, (uint8_t*) "\n\r", 2, HAL_MAX_DELAY);
	}
}
