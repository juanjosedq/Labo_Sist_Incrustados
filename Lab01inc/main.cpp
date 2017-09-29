/*****************************************************************************
*
* Copyright (C) 2013 - 2016 Texas Instruments Incorporated - http://www.ti.com/
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the
*   distribution.
*
* * Neither the name of Texas Instruments Incorporated nor the names of
*   its contributors may be used to endorse or promote products derived
*   from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*
* MSP432 empty main.c template
*
* for GNU compiler
*
******************************************************************************/
/*****************************************************************************
 * Autores:
 *          Jose Ariel Fallas Pizarro
 *          Juan José Delgado Quesada
 ******************************************************************************/

#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "HAL_I2C.h"
#include "HAL_OPT3001.h"
#include <math.h>

#include <stdint.h>
#include <stdbool.h>

//uint8_t Func10W = 0U;
uint8_t Func10W = 0XFF;         //Opcion para 10W
uint8_t Func15W = 0U;
//uint8_t Func15W = 0XFF;        //Opcion para 15W

uint16_t ADC14Result = 0U;      //Variable donde se almacenara el resultado del convertidor
uint32_t CONTADOR_SOFT = 0U;    //Contador por software

volatile uint32_t k = 0U;
volatile uint32_t promedio = 0U;     // promedio
volatile uint32_t sonido = 0U;      //promedio + el 10%
volatile uint32_t n = 0U;
volatile uint32_t m = 0U;

float lux;                  // Variable para guardar la medida del sensor de luz
uint32_t VECTOR[500];       // Vector donde se guardan los niveles de sonido medidos

int main(void)
{

    volatile uint32_t i;
    volatile uint32_t j;


    WDTCTL = WDTPW | WDTHOLD;         // Stop watchdog timer

    P1->DIR = BIT0;
    P1->OUT = 0U;

    P2->DIR = BIT1;
    P2->OUT = 0U & Func10W;

    P5->DIR = BIT6;
    P5->OUT = 0U & Func15W;

    // Set P4.3 for Analog input, disabling the I/O circuit.
    P4->SEL0 = BIT3;
    P4->SEL1 = BIT3;
    P4->DIR &= ~BIT3;

  // Configura El puerto P3.5 como entrada y habilita interrupción /
       MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
       MAP_GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5);
       MAP_GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);
       MAP_Interrupt_enableInterrupt(INT_PORT3);
       GPIO_interruptEdgeSelect(GPIO_PORT_P3, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);



    const Timer_A_UpModeConfig upConfig =
        {
                TIMER_A_CLOCKSOURCE_ACLK,               // ACLK Clock SOurce
                TIMER_A_CLOCKSOURCE_DIVIDER_1,          // ACLK/1 = 3MHz
                200,                                    // 200 tick period
                TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
                TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,    // Disable CCR0 interrupt
                TIMER_A_DO_CLEAR                        // Clear value
        };

    // Configuring Timer_A0 for Up Mode and starting
        MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
        MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    //--------------------- Configuracion del TIMER32 1.0-------------------------------

    TIMER32_1->LOAD = 0x0000BB80; //~0.5s ---> a 48Mhz // Valor de donde se va a empezar a decrementar
    // 32 bits del contador | no preescala, clock dividido entre 1 | periodico | habilita interrupcion | habilita reloj
    TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
    NVIC_SetPriority(T32_INT1_IRQn,1); // Define la prioridad
    NVIC_EnableIRQ(T32_INT1_IRQn);     // Habilita la interrupcion

    //--------------------- Configuracion del ADC -------------------------------

    // Clock pre dividido por 1 | 14 bits | dividido por 7 | clock source | 4 muestras | encender | senal con clck muestreo
    ADC14->CTL0 = ADC14_CTL0_PDIV_0 | ADC14_CTL0_SHS_0 | ADC14_CTL0_DIV_7 |
                  ADC14_CTL0_SSEL__MCLK | ADC14_CTL0_SHT0_1 | ADC14_CTL0_ON
                  | ADC14_CTL0_SHP;
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_0;
    ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_ENC;
    ADC14->IER0 = ADC14_IER0_IE0;
    NVIC_SetPriority(ADC14_IRQn,1);               //Definir la prioridad de la interrupcion
    NVIC_EnableIRQ(ADC14_IRQn);                   //Habilitar la interrupcion

    // Initialize I2C communication
        Init_I2C_GPIO();
        I2C_init();

        // Initialize OPT3001 digital ambient light sensor
        OPT3001_init();

    for(i=6; i>0; i--){
            GPIO_toggleOutputOnPin(
                GPIO_PORT_P1,
                GPIO_PIN0
                );
            if(Func10W){
                GPIO_toggleOutputOnPin(
                        GPIO_PORT_P2,
                        GPIO_PIN1
                );
            }
            if(Func15W){
                GPIO_toggleOutputOnPin(
                        GPIO_PORT_P5,
                        GPIO_PIN6
               );
            }

            for(j= 96000; j>0; j--);
    }

    lux = OPT3001_getLux();

    if (lux < 100){
        CONTADOR_SOFT = 0U;                  // Contador de implementado por software para el tiempo de la Luz
        P1->OUT = 1U;
        P2->OUT = 2U & Func10W;
        P5->OUT = BIT6 & Func15W;
        while(CONTADOR_SOFT < 250){
                //CONTADOR_SOFT = CONTADOR_SOFT+1;
        }
    }else{
        P1->OUT = 0U;
        P2->OUT = 0U & Func10W;
        P5->OUT = 0U & Func15W;
    }



    while (1)
    {
        // Obtain lux value from OPT3001
        __disable_irq();
        lux = OPT3001_getLux();
        __enable_irq();

        promedio = 0U;
        for(m=0; m<500; m++){
        promedio = promedio + VECTOR[m];
        }
        promedio = promedio/500;
        sonido = promedio + promedio*0.1;

        if ((VECTOR[k] > sonido) & (lux < 100U) & (VECTOR[k] > 300U)){
            CONTADOR_SOFT = 0U;
            P1->OUT = 1U;
            P2->OUT = 2U & Func10W;
            P5->OUT = BIT6 & Func15W;

            while(CONTADOR_SOFT < 250){
            }
        }
            P1->OUT = 0U;
            P2->OUT = 0U & Func10W;
            P5->OUT = 0U & Func15W;


    }
    return 0;
}

extern "C"
{
    void T32_INT1_IRQHandler(void)
    {
        __disable_irq();
        TIMER32_1->INTCLR = 0XCAFECAFE;
        ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC; // Start
        CONTADOR_SOFT = CONTADOR_SOFT + 1U;
        __enable_irq();
        return;
    }

    void PORT3_IRQHandler(void)
    {
        __disable_irq();
        n = 0U;
        MAP_GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5);
        for(n = 60000; n > 0; n--){
        }
        GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
        GPIO_toggleOutputOnPin(GPIO_PORT_P2,GPIO_PIN1 & Func10W);
        GPIO_toggleOutputOnPin(GPIO_PORT_P5,GPIO_PIN6 & Func15W);
        if((P1->OUT) == 1U){
            n = 0U;
            for(n = 1500000; n > 0; n--){
            }
        }
        __enable_irq();
    }



    void ADC14_IRQHandler(void)
    {
        __disable_irq();
        if(k >= 500){
            k = 0;
        }else{
            k = k+1;
        };
        VECTOR[k] = abs(abs(ADC14->MEM[0]) - 8180U);
        if((VECTOR[k] > promedio) & (lux < 100) & (VECTOR[k] > 300)){
            CONTADOR_SOFT = 0U;
        }
        ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0;
        __enable_irq();
        return;
    }
}
