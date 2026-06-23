#ifndef PACMAN_H
#define PACMAN_H

#include <stdint.h>
#include <stdio.h>


// Constants for maze dimensions and tile size
#define MAZE_WIDTH 240
#define MAZE_LENGTH 276
#define TILE_SIZE 12


// Global variables for Pac-Man's state
extern uint8_t pacman_x;
extern uint8_t pacman_y;
extern uint8_t cur_direction;
extern uint8_t next_direction;

// Function prototypes
void init_pacman(void);
void draw_pacman(uint8_t x, uint8_t y, uint16_t color);
void move_pacman(void);
void update_pacman_direction(void);
void draw_maze(uint16_t color);
void draw_pills(void);
void display_lives(void);
void init_game(void);
void run_game(void);
void pause_game(void);
void update_time(void);
void continue_game(void);

#endif // PACMAN_H
