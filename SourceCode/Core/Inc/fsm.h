/*
 * fsm.h
 *
 *  Created on: Dec 4, 2023
 *      Author: ASUS
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"
#include "SoftwareTimer.h"
#include "stdio.h"

#define MAX_BUFFER_SIZE 30

#define INIT_command 4
#define READ_command 5
#define IMP_command 6
#define INIT_uart 7
#define RST 8
//#define WAIT 9
#define OK 10

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;
extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;

void command_parser_fsm();
void uart_communication_fsm();

#endif /* INC_FSM_H_ */
