#include "Paint.hpp"

Paint::Paint(Graphics_Context a, Graphics_Rectangle b)
{
    g_sContext = a;
    g_rect = b;
}

uint8_t Paint::run()
{
	int64_t l_u8Msj[3];
	
	l_u8Msj[0] = *m_pRTaskMsj;
	l_u8Msj[1] = *(m_pRTaskMsj+1);
	l_u8Msj[2] = *(m_pRTaskMsj+2);

	g_rect.xMin = 0;
    g_rect.yMin = l_u8Msj[1];
    g_rect.xMax = 128;
    g_rect.yMax = l_u8Msj[0];

    Graphics_setForegroundColor(&g_sContext, l_u8Msj[2]);
	Graphics_fillRectangle(&g_sContext, &g_rect);

    return(NO_ERR);
}

uint8_t Paint::setup()
{

	//Graphics_Context g_sContext;
	//Graphics_Rectangle g_rect;
	
	/* Initializes graphics context */

    return(NO_ERR);
}
