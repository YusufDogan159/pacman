																			 /*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_joystick, funct_joystick .c files
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#ifndef LIB_JOYSTICK_H
#define LIB_JOYSTICK_H

/* Enumeration for joystick directions */
typedef enum {
  JOY_NONE, // No direction
  JOY_UP,
  JOY_DOWN,
  JOY_LEFT,
  JOY_RIGHT
} JoystickDirection;

/* Function prototypes */
void joystick_init(void);
JoystickDirection get_joy_direction(void);

#endif // LIB_JOYSTICK_H
