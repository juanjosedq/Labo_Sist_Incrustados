#include "Caldif.hpp"

Caldif::Caldif(uint16_t i_BITN)
{
	m_u16BITN = i_BITN;
}

uint8_t Caldif::run()
{
	uint64_t l_u8Msj[3];
	double l_puntero = *(m_pRTaskMsj+2);
    double l_preCurrentLine = (l_puntero*(-32.0/1665.0)) + 222.3663;
    uint64_t l_u8CurrentLine = l_preCurrentLine;


    if(g_u8OldLine != l_u8CurrentLine){
        if(l_u8CurrentLine >= g_u8OldLine){
            l_u8Msj[0] = l_u8CurrentLine;
            l_u8Msj[1] = g_u8OldLine;
            l_u8Msj[2] = GRAPHICS_COLOR_BLUE;
            SendMessage(l_u8Msj, T_Paint);
        } else {
            l_u8Msj[0] = g_u8OldLine;
            l_u8Msj[1] = l_u8CurrentLine;
            l_u8Msj[2] = GRAPHICS_COLOR_SADDLE_BROWN;
            SendMessage(l_u8Msj, T_Paint);
        }
    }
	
	g_u8OldLine = l_u8CurrentLine;

    return(NO_ERR);
}

uint8_t Caldif::setup()
{
    g_u8OldLine = 64;
    return(NO_ERR);
}
