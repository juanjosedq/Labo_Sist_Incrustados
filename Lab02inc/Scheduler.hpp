/*
 * Scheduler.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef TASKS_SCHEDULER_HPP_
#define TASKS_SCHEDULER_HPP_
#include "msp.h"
#include "Task.hpp"
#include "Defines.hpp"

// - This structure defines the Task Information


struct st_TaskInfo {
	Task * pToAttach; // - Pointer to the Task
	uint8_t u8Priority; // Set
	uint64_t u64TickInterval; // - How often the task is executed
	uint64_t u64ticks; // - Current tick count
	uint64_t u64TickIntervalInitValue; // - Value to reset
	uint8_t Type;
	bool ToExecute;
};


class Scheduler
{
public:
    Scheduler();
    uint8_t m_uCantTask;
    uint8_t * m_MsjInfo;
    uint64_t m_PilaMsj[PilaSize];           // Espacio de Memoria Asignado para Scheduler. Este sera asignado a las tareas posteriormente por Scheduler
    uint16_t m_IndexPila;                   // Para asignar Memoria a los mensajes de tareas del Scheduler
    uint64_t m_u64ticks;
    st_MsjInfo m_InfoBox [BufferSize];  // Tamaño del buzon del Scheduler, puede almacenar mensajes de  hasta 10 tareas (BufferSize).


    uint8_t CallTask(uint8_t Task_ID); // Llamar una funcion cuando se le escribe un mensaje
    uint8_t attach(uint8_t Type, Task * i_ToAttach, uint64_t i_u64TickInterval);
    uint8_t run(void);
    uint8_t setup(void);
    uint64_t * AssignMemoryToTask(uint8_t i_MailBoxSize);
private:

    uint8_t m_u8OpenSlots; // - Available slots
    uint8_t m_u8NextSlot;  // - Next available slot
    st_TaskInfo m_aSchedule[NUMBER_OF_SLOTS]; // - Current schedule to be executed
    st_TaskInfo m_aNextSchedule[NUMBER_OF_SLOTS]; // - Next schedule to be executed (not implemented)
    uint8_t CalculateNextSchedule(void); // - Calculate next schedule tasks (not implemented)
    uint8_t SortScheduleByPriority(Task * i_pSchedule); // - Sorts a schedule based on priority (not implemented)

 };



#endif /* TASKS_SCHEDULER_HPP_ */
