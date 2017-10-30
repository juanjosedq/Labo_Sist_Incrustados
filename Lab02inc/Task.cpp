#include "Task.hpp"


Task::Task()
{
    m_u8TaskID = m_u8NextTaskID;
    m_u8NextTaskID++;
    m_bIsFinished = false;

}

uint8_t Task::SendMessage(uint64_t Msj[3], uint8_t ID)
{
    for(int i = 0; i<3 ; i++){
    *(m_pWTaskMsj+i) = Msj[i];
    }
    (m_pSTaskMsj+ID)->TaskToCall = ID;                      // Escribo al Sheduler a quien debe llamar
    (m_pSTaskMsj+ID)->DirDataRead= this->m_pWTaskMsj;       // Le paso el puntero de donde guardé el dato al Scheduler para que la otra tarea lo lea
    return(NO_ERR);
}

