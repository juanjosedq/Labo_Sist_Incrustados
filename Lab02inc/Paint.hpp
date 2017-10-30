
#ifndef Paint_HPP_
#define Paint_HPP_

#define __NOP __nop
#include "msp.h"
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


class Paint : public Task
{
	public:
		Paint(Graphics_Context a, Graphics_Rectangle b);
		uint16_t m_u16BITN;
        Graphics_Context g_sContext;
        Graphics_Rectangle g_rect;
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
    protected:
    private:
};

#endif /* Paint_HPP_ */
