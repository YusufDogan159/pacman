/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "/pacman.h"
#include "GLCD/GLCD.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER0_IRQHandler(void) {
    // Check if the interrupt flag for Match Register 0 (MR0) is set
    if ((LPC_TIM0->IR & (1 << 0)) != 0) {
        update_pacman_direction();
        LPC_TIM0->IR = (1 << 0); // Clear MR0 interrupt flag
    }

    // Check if the interrupt flag for Match Register 1 (MR1) is set
    if ((LPC_TIM0->IR & (1 << 1)) != 0) {
        move_pacman();
        LPC_TIM0->IR = (1 << 1); // Clear MR1 interrupt flag
    }
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler(void) {
    // Check if the interrupt flag for Match Register 0 (MR0) is set
    if ((LPC_TIM1->IR & (1 << 0)) != 0) {
        update_time();
        LPC_TIM1->IR = (1 << 0); // Clear MR0 interrupt flag
    }
}


/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler(void)
{
    // Check if the interrupt flag for Match Register 0 (MR0) is set
    if ((LPC_TIM2->IR & (1 << 0)) != 0) {
        LED_On(7); 								// Turn on LED 7
        LPC_TIM2->IR = (1 << 0); 	// Clear the interrupt flag for MR0
        return;
    }

    // Check if the interrupt flag for Match Register 1 (MR1) is set
    if ((LPC_TIM2->IR & (1 << 1)) != 0) {
        LED_Off(7); 							// Turn off LED 7
        LPC_TIM2->IR = (1 << 1); 	// Clear the interrupt flag for MR1
        return;
    }
}

/******************************************************************************
** Function name:		Timer3_IRQHandler
**
** Descriptions:		Timer/Counter 3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler(void)
{
    // Check if the interrupt flag for Match Register 0 (MR0) is set
    if ((LPC_TIM3->IR & (1 << 0)) != 0) {
        LED_On(6); 								// Turn on LED 7
        LPC_TIM3->IR = (1 << 0); 	// Clear the interrupt flag for MR0
        return;
    }

    // Check if the interrupt flag for Match Register 1 (MR1) is set
    if ((LPC_TIM3->IR & (1 << 1)) != 0) {
        LED_Off(6); 							// Turn off LED 7
        LPC_TIM3->IR = (1 << 1); 	// Clear the interrupt flag for MR1
        return;
    }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
