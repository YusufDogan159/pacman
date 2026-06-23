/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/
void joystick_init(void) {
  /* Configure joystick pins */
  LPC_PINCON->PINSEL3 &= ~(3 << 26);  // P1.26 - P1.29 as GPIO
  LPC_GPIO1->FIODIR   &= ~((1 << 26) | (1 << 27) | (1 << 28) | (1 << 29)); // Set as input
}

/*----------------------------------------------------------------------------
  Function to get joystick direction
 *----------------------------------------------------------------------------*/
JoystickDirection get_joy_direction(void) {
  uint32_t pin_status = LPC_GPIO1->FIOPIN;

  if (!(pin_status & (1 << 26))) {
    return JOY_DOWN;
  } else if (!(pin_status & (1 << 27))) {
    return JOY_LEFT;
  } else if (!(pin_status & (1 << 28))) {
    return JOY_RIGHT;
  } else if (!(pin_status & (1 << 29))) {
    return JOY_UP;
  }

  return JOY_NONE; // No input detected
}
