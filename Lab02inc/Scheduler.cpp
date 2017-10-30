#include "Scheduler.hpp"

// - Scheduler constructor
Scheduler::Scheduler()
{
    m_u8OpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS);
    m_u8NextSlot = 0;
    m_IndexPila = 0;                         // Para recorrer la pila y asignar memoria a las tareas

    for(int i = 0; i < PilaSize ; i ++){
        m_PilaMsj[i] = 0;                    //Iniciar la pila de mensajes con puros ceros.
    }

    for(int index = 0; index < NUMBER_OF_SLOTS; index++)
    {
        m_aSchedule[index].pToAttach = (uintptr_t) 0; // Init to an invalid pointer
    }
    return;
}



// - The attach function, inserts the task into the schedule slots.
uint8_t Scheduler::attach(uint8_t Type ,Task * i_ToAttach, uint64_t i_u64TickInterval)
{
    uint8_t l_ErrorCode = NO_ERR;
    st_TaskInfo l_st_StructToAttach;

    l_st_StructToAttach.Type = Type;
    l_st_StructToAttach.pToAttach = i_ToAttach;
    l_st_StructToAttach.u64ticks = this->m_u64ticks;
    l_st_StructToAttach.u64TickInterval = 0;
	l_st_StructToAttach.u64TickIntervalInitValue = i_u64TickInterval;
	l_st_StructToAttach.ToExecute = false;
	l_st_StructToAttach.pToAttach->m_pSTaskMsj = &m_InfoBox[0];


    if((m_u8OpenSlots>0) && (m_u8NextSlot < NUMBER_OF_SLOTS))
    {
        m_aSchedule[m_u8NextSlot] =  l_st_StructToAttach;
        m_u8OpenSlots--;
        m_u8NextSlot++;
    }
    else
    {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}
// - Execute the current schedule
uint8_t Scheduler::run(void)
{
    int l_iNextTaskSlot = 0U;
    Task * l_pNextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;

    while(l_iNextTaskSlot < NUMBER_OF_SLOTS)
    {
        for(int a = 0; a < BufferSize ; a ++ ){
            if(m_InfoBox[a].TaskToCall != 0){
                CallTask(m_InfoBox[a].TaskToCall);
                m_InfoBox[a].TaskToCall=0;
            }
        }
    	l_pNextTask = static_cast<Task *> (m_aSchedule[l_iNextTaskSlot].pToAttach);
		if(l_pNextTask != ((uintptr_t) 0))
		{
//*****************************************************************************
		    if(m_aSchedule[l_iNextTaskSlot].ToExecute == true){
		        l_pNextTask->run();
		        m_aSchedule[l_iNextTaskSlot].ToExecute = false;
		        m_aSchedule[l_iNextTaskSlot].pToAttach->MailBox = false;
		    }

		    if(m_aSchedule[l_iNextTaskSlot].Type == OneShot){
		            // Ignoro el tiempo del Tick
		    }
		    else{
		        if(m_aSchedule[l_iNextTaskSlot].u64TickInterval == 0){
		            m_aSchedule[l_iNextTaskSlot].ToExecute = true;
		         }
		         m_aSchedule[l_iNextTaskSlot].u64TickInterval ++;
		         if(m_aSchedule[l_iNextTaskSlot].u64TickInterval > m_aSchedule[l_iNextTaskSlot].u64TickIntervalInitValue) {
		             m_aSchedule[l_iNextTaskSlot].u64TickInterval = 0;
		         }
		    }
		}
		l_iNextTaskSlot++;
	}
    CalculateNextSchedule(); // TODO

    return l_u8ReturnCode;
}
//*****************************************************************************
//			if(m_aSchedule[l_iNextTaskSlot].u64TickInterval == 0){
//				l_pNextTask->run();
//			}
//			m_aSchedule[l_iNextTaskSlot].u64TickInterval ++;
//
//			if(m_aSchedule[l_iNextTaskSlot].u64TickInterval > m_aSchedule[l_iNextTaskSlot].u64TickIntervalInitValue) {
//				m_aSchedule[l_iNextTaskSlot].u64TickInterval = 0;
//			}
/*		}
		l_iNextTaskSlot++;
    }
    CalculateNextSchedule(); // TODO

    return l_u8ReturnCode;
}
*/

// - Execute the setup function for all tasks
uint8_t Scheduler::setup(void)
{
    int l_iNextTaskSlot = 0U;
    Task * l_pNextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;

    while(l_iNextTaskSlot < NUMBER_OF_SLOTS)
    {
        l_pNextTask = static_cast<Task *> (m_aSchedule[l_iNextTaskSlot].pToAttach);
        if(l_pNextTask != ((uintptr_t) 0))
        {
            l_pNextTask->setup();
        }
        l_iNextTaskSlot++;
    }
    return l_u8ReturnCode;
}

uint8_t Scheduler::CalculateNextSchedule(void)
{
    //int l_iNextSchedule = 0U;
    //Task * l_pNextTask = (uintptr_t) 0;

    return(NO_ERR);
}
uint8_t Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{

    return(NO_ERR);
}

uint64_t * Scheduler::AssignMemoryToTask(uint8_t i_MailBoxSize){
    uint64_t * l_pTaskMsj = &m_PilaMsj[m_IndexPila];
    m_IndexPila += i_MailBoxSize;
    return(l_pTaskMsj);
}

uint8_t Scheduler::CallTask(uint8_t Task_ID){
   // Task * l_pNextTask = (uintptr_t) 0;
    for(int cont=0; cont< NUMBER_OF_SLOTS; cont++ )
        if(m_aSchedule[cont].pToAttach->m_u8TaskID == Task_ID){

            m_aSchedule[cont].ToExecute = true;
            m_aSchedule[cont].pToAttach->MailBox = true;
            m_aSchedule[cont].pToAttach->m_pRTaskMsj = m_InfoBox[Task_ID].DirDataRead;
        }
    return(NO_ERR);
}
