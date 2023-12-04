/*
 * fsm.c
 *
 *  Created on: Nov 17, 2023
 *      Author: ASUS
 */
#include "fsm.h"

int status_command = INIT_command, status_uart = INIT_uart;
char str[50];
uint8_t temp = 0;
uint8_t cmd[50];
int ADC_value = 0, index_cmd = 0; //%d
uint8_t index_buffer = 0;
uint8_t buffer[MAX_BUFFER_SIZE] = {0};
uint8_t buffer_flag = 0;
void reset(){
	index_cmd = 0;
	temp = 0;
}

void command_parser_fsm(){
	switch(status_command){
	case INIT_command:
		if (temp == '!') status_command = READ_command;
		break;

	case READ_command:
		if (temp == '!') reset();
		else if (temp != '#' && temp != '!') cmd[index_cmd++] = temp;
		else if (temp == '#') {
			reset(); // reset all para for the next state, RST
			//status_command = IMP_command; // state defines RST or OK
			//}
			if (cmd[0] == 'R' && cmd[1] == 'S' && cmd[2] == 'T'){ //fix character delay
				status_uart = RST;
				setTimer2(1);
			}
			else if (cmd[0] == 'O' && cmd[1] == 'K'){
				status_uart = OK;
			}
		}

			//status_command = INIT_command; //All para already reset in READ

		break;

//	case IMP_command: // !!! ***BUG: ENTER
//		if (cmd[0] == 'R' && cmd[1] == 'S' && cmd[2] == 'T'){
//			status_uart = RST;
//			setTimer2(1);
//		}
//		else if (cmd[0] == 'O' && cmd[1] == 'K'){
//			status_uart = OK;
//		}
//
//		status_command = INIT_command; //All para already reset in READ
//		break;

	default:
		break;
	}
}

void uart_communication_fsm(){
	switch(status_uart){
	case INIT_uart:
		break;

	case RST:
		if(timer2_flag==1){
		ADC_value = HAL_ADC_GetValue (&hadc1);
		HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "!ADC=%d#\r\n", ADC_value), 50);
		status_command = INIT_command;
		setTimer2(300);
		}
		break;


	case OK:
		ADC_value = 0;
		status_uart = INIT_uart;
		break;

	default:
		break;
	}
}
