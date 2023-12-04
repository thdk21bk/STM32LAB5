/*
 * sched.c
 *
 *  Created on: Dec 1, 2023
 *      Author: ASUS
 */
#include "scheduler.h"
typedef struct {
	void ( * pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
} sTask;
sTask SCH_tasks_G[SCH_MAX_TASKS]; //store tasks
uint8_t current_index_task = 0;

void SCH_Init(){
	for(int i=0; i<SCH_MAX_TASKS; i++){
		SCH_tasks_G[i].pTask = 0;
		SCH_tasks_G[i].Delay = 0;
		SCH_tasks_G[i].Period = 0;
		SCH_tasks_G[i].RunMe = 0;
		SCH_tasks_G[i].TaskID = 0;
	}
}

void SCH_Update(void){
	if (SCH_tasks_G[0].Delay > 0) SCH_tasks_G[0].Delay--;
	else SCH_tasks_G[0].RunMe++;
}

void SCH_Add_Task (void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
	uint8_t adding_Task_index = 0;
	sTask adding_Task;

	if(current_index_task < SCH_MAX_TASKS){ //If the array is not full, it proceeds to add the new task.
		adding_Task.pTask = pFunction;
		adding_Task.Delay = DELAY;
		adding_Task.Period =  PERIOD;
		adding_Task.RunMe = 0;
		adding_Task.TaskID = current_index_task;

		if (current_index_task == 0){ //If empty (no tasks currently scheduled), the new task is directly added
			SCH_tasks_G[0] = adding_Task;
			current_index_task++;
			return;
		}

		while(adding_Task.Delay > SCH_tasks_G[adding_Task_index].Delay && adding_Task_index < current_index_task){
			adding_Task.Delay = adding_Task.Delay - SCH_tasks_G[adding_Task_index].Delay;
			adding_Task_index++;
		}
		current_index_task++;

		//Shift in order to insert the task
		for (uint8_t i = (current_index_task - 1); i>adding_Task_index; i--){
			SCH_tasks_G[i] = SCH_tasks_G[i-1];
		}

		SCH_tasks_G[adding_Task_index] = adding_Task;
		SCH_tasks_G[adding_Task_index+1].Delay -= adding_Task.Delay;
	}
}


void SCH_Delete_Task (uint32_t taskID){
	for (int i=taskID; i<(current_index_task-1); i++){
		SCH_tasks_G[i]=SCH_tasks_G[i+1];
	}
	//Clear the last task slot
	SCH_tasks_G[current_index_task-1].pTask = 0;
	SCH_tasks_G[current_index_task-1].Delay = 0;
	SCH_tasks_G[current_index_task-1].Period = 0;
	SCH_tasks_G[current_index_task-1].RunMe = 0;
	current_index_task--;
}

void SCH_Dispatch_Tasks(void){
	while(SCH_tasks_G[0].pTask != 0){
		if(SCH_tasks_G[0].RunMe > 0){
			SCH_tasks_G[0].RunMe--; //Reset RunMe
			(*SCH_tasks_G[0].pTask)(); //Run task

			// Set flag if the successive task runs out the delay
			if (SCH_tasks_G[1].Delay == 0) SCH_tasks_G[1].RunMe++;

			sTask temp_Task = SCH_tasks_G[0]; //store the first task into temp
			SCH_Delete_Task(0); // Delete the first task
			//Checks if the task is periodic, period!=0. => add the task back to the scheduler with its original period.
			if (temp_Task.Period != 0) SCH_Add_Task(temp_Task.pTask, temp_Task.Period, temp_Task.Period);
		}
	}
	// Enter low-power mode (Sleep mode). The MCU will wake up on the next interrupt
		    //HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}
