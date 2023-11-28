/*
 * Sched.c
 *
 *  Created on: Nov 29, 2023
 *      Author: ASUS
 */
#include "Sched.h"

typedef struct {
	void ( * pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
} sTask;

// The array of tasks
static sTask SCH_tasks_G[SCH_MAX_TASKS];
uint8_t current_index_task = 0;
/*Quản lý task trong array, xem thử:
	(1): Vị trí đó còn trống
	(2): Vị trí đã đầy
*/

void SCH_Init(void){
	//TIMER_Init();
}

void SCH_Update(void){
	// Check if there is a task at this location
	// Dùng để trừ counter; NHƯNG ở đây có 02 counter
	for(int i=0; i<current_index_task;i++){
		if(SCH_tasks_G[i].Delay>0) SCH_tasks_G[i].Delay--;
		else {
			SCH_tasks_G[i].RunMe +=1;
			SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
			/*
			1st: Task thực hiện dựa trên Delay.
			2nd: Từ lần 2 trở đi, theo Period
			*/
		}
	}
}

void SCH_Add_Task(void (* pFunction)(), uint32_t DELAY, uint32_t PERIOD){
	if(current_index_task < SCH_MAX_TASKS){
	//Có biến current_index_task ==> Sắp xếp task từ 0 1 2 ... ==> Check (index <MAX ), nếu nhỏ hơn thì thêm nó vào với các tham số lấy từ hàm Add
	SCH_tasks_G[current_index_task].pTask = pFunction;
	SCH_tasks_G[current_index_task].Delay = DELAY;
	SCH_tasks_G[current_index_task].Period = PERIOD;

	SCH_tasks_G[current_index_task].RunMe  = 0; //CHÚ Ý: cho RunMe=0

	SCH_tasks_G[current_index_task].TaskID = current_index_task; // Lưu giữ vị trí task trong hệ thống => trong tương lai có thể Delete khỏi hệ thống.
	current_index_task++; //!!! CHƯA TỐI ƯU (-> Slide: Vòng lặp For, tìm vị trí trống -> thêm vào)
	}
	//Ban đầu là uint32_t: return SCH_tasks_G[newTaskIndex].TaskID;
}

void SCH_Dispatch_Tasks(void){
	for (int i=0; i <current_index_task; i++){
		if(SCH_tasks_G[i].RunMe > 0) {
			(*SCH_tasks_G[i].pTask)(); // Run the task
			SCH_tasks_G[i].RunMe--; // Reset / reduce RunMe flag
			// sTask temtask = SCH_tasks_G[0];
			// SCH_Delete_Task(temtask.TaskID);
			// if (temtask.Period != 0) {
			// 	SCH_Add_Task(temtask.pTask, temtask.Period, temtask.Period);
			// }
		}
	}
}
