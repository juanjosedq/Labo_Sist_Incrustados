#ifndef Caldif_HPP_
#define Caldif_HPP_

#define __NOP __nop
#include "msp.h"
#include "Defines.hpp"
#include "Task.hpp"
//#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "Defines.hpp"
#include "Scheduler.hpp"
#include "msp.h"
#include "Task.hpp"


class Caldif : public Task
{
	public:
        Caldif(uint16_t i_BITN);
		//Paint(uint16_t);
		uint16_t m_u16BITN;
		uint64_t g_u8OldLine;
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
    protected:
    private:
};

#endif /* Caldif_HPP_ */
