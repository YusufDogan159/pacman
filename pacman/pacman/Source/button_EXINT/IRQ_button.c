#include "button.h"
#include "LPC17xx.h"


#include "../led/led.h"
#include "../timer/timer.h"
#include "../pacman.h"
#include "../GLCD/GLCD.h"

// Global variable to track game state
uint8_t game_running = 0; // 0 = paused, 1 = running

void EINT0_IRQHandler(void) /* INT0 */
{
	LED_On(1);
    if (game_running) {
        // If the game is running, pause it
        pause_game();
        game_running = 0; // Update state to paused
    } else {
        // If the game is paused, resume it
        continue_game();
        game_running = 1; // Update state to running
    }

    LPC_SC->EXTINT |= (1 << 0); /* Clear pending interrupt */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
  LED_On(1);
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	LED_Off(0);
	LED_Off(1);
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */  
	enable_timer(0);  
}


