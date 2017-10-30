#ifndef TASK_HPP_
#define TASK_HPP_

//#include "Scheduler.hpp"
#include "msp.h"
#include <string>

#define NO_ERR 0
#define RET_ERR 1

using namespace std;

struct st_MsjInfo {
    uint8_t TaskToCall;
    uint64_t * DirDataRead;
};


class Task
{
	public:
		Task();                         // Para definir el tipo de operación que será
		st_MsjInfo * m_pSTaskMsj;       //  Para Hablar con el Scheduler.
		uint64_t * m_pRTaskMsj;         // Puntero de la tarea para leer
        uint64_t * m_pWTaskMsj;         // Puntero al espacio de memoria asignado a la tarea para escribir.
		uint8_t m_u8TaskID;             // ID de la Tarea

		bool MailBox;              // Bandera para saber si la tarea tiene mensaje y asi leer el puntero RTaskMsj.

        uint8_t SendMessage(uint64_t Msj[3], uint8_t ID);
		virtual uint8_t     run(void){return(0);};
		virtual uint8_t     setup(void){return(0);};

		bool                IsTaskFinished(void){return m_bIsFinished;};
		uint8_t             GetTaskPriority(void) {return m_u8Priority;};
		void                SetTaskPriority(uint8_t i_u8NewPriority){m_u8Priority = i_u8NewPriority;};
	private:
	   static uint8_t m_u8NextTaskID;
	protected:
	   uint8_t m_u8Priority;
	   bool m_bIsFinished;
};

#endif /* TASK_HPP_ */
