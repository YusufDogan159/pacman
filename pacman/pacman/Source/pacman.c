// pacman.c
#include "pacman.h"
#include "GLCD/GLCD.h"
#include "joystick/joystick.h"
#include "timer/timer.h"
#include "button_EXINT/button.h"

extern uint8_t game_running; // 0 = paused, 1 = running

// Pac-Man's current position
uint8_t pacman_x = 0;
uint8_t pacman_y = 0;

// Directions
uint8_t cur_direction = 0;
uint8_t next_direction = 0;

// Maze grid size
uint8_t grid_size = TILE_SIZE;

// Score
uint16_t score = 0;

// Global variable to track remaining time
uint8_t remaining_time = 60;

// Pac-Man map array
// 0: corridor, 1: wall, 2: pill, 3: powerpill, 4: unreachable
int pacman_map[23][20] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
    {4, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 4},
    {1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1},
    {4, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 4},
    {1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// Initialize Pac-Man's position
void init_pacman(void) {
    pacman_x = 120;
    pacman_y = 204;
    cur_direction = 0;  // Default to no movement
    next_direction = 0;
    draw_pacman(pacman_x, pacman_y, Yellow);
}

// Draw Pac-Man at specified coordinates
void draw_pacman(uint8_t x, uint8_t y, uint16_t color) {
    for (uint8_t i = 0; i < TILE_SIZE; i++) {
        for (uint8_t j = 0; j < TILE_SIZE; j++) {
            LCD_SetPoint(x + i, y + j, color);
        }
    }
}


void move_pacman(void) {
    uint8_t new_x = pacman_x;
    uint8_t new_y = pacman_y;

    switch (cur_direction) {
        case 1: // Up
            if (pacman_y >= grid_size) new_y -= grid_size;
            break;
        case 2: // Down
            if (pacman_y <= MAZE_LENGTH - grid_size) new_y += grid_size;
            break;
        case 3: // Left
            if (pacman_x >= grid_size) new_x -= grid_size;
            break;
        case 4: // Right
            if (pacman_x <= MAZE_WIDTH - grid_size) new_x += grid_size;
            break;
    }

    // Teleport Pac-Man if out of bounds
    if (new_x >= MAZE_WIDTH) {
        new_x = 0;
    } else if (new_x < grid_size) {
        new_x = MAZE_WIDTH - grid_size;
    }

    // Check for wall collision
    if (pacman_map[new_y / TILE_SIZE][new_x / TILE_SIZE] == 1) {
        return; // Wall detected, do nothing
    }
		
		 // Check for pill
    if (pacman_map[new_y / TILE_SIZE][new_x / TILE_SIZE] == 0) {
        pacman_map[new_y / TILE_SIZE][new_x / TILE_SIZE] = 2; // Mark as eaten
        score += 10; // Increase score

        // Display the updated score at the bottom of the screen
        char score_text[20];
        sprintf(score_text, "SCORE: %d", score);
        GUI_Text(0, 280, (uint8_t *)score_text, White, Black);
    }
		
		// Display lives
    display_lives();

    // Erase the previous Pac-Man position
    draw_pacman(pacman_x, pacman_y, Black);

    // Update position
    pacman_x = new_x;
    pacman_y = new_y;

    // Draw Pac-Man at the new position
    draw_pacman(pacman_x, pacman_y, Yellow);
}


// Update the direction based on joystick input
void update_pacman_direction(void) {
    uint8_t joystick_status = get_joy_direction();
    if (joystick_status != JOY_NONE) { // Only update if joystick input is not NONE
        next_direction = joystick_status;
    }

    uint8_t new_x = pacman_x;
    uint8_t new_y = pacman_y;

    switch (next_direction) {
        case 1: // Up
            if (pacman_y >= grid_size) new_y -= grid_size;
            break;
        case 2: // Down
            if (pacman_y <= MAZE_LENGTH - grid_size) new_y += grid_size;
            break;
        case 3: // Left
            if (pacman_x >= grid_size) new_x -= grid_size;
            break;
        case 4: // Right
            if (pacman_x <= MAZE_WIDTH - grid_size) new_x += grid_size;
            break;
    }

    // Check for wall collision
    if (pacman_map[new_y / TILE_SIZE][new_x / TILE_SIZE] != 1) {
        cur_direction = next_direction;
    }
}

// Draw the maze
void draw_maze(uint16_t color) {
    for (uint8_t row = 0; row < 23; row++) {
        for (uint8_t col = 0; col < 20; col++) {
            uint16_t tile_color = (pacman_map[row][col] == 1) ? Blue : Black;
            for (uint8_t i = 0; i < TILE_SIZE; i++) {
                for (uint8_t j = 0; j < TILE_SIZE; j++) {
                    LCD_SetPoint(col * TILE_SIZE + i, row * TILE_SIZE + j, tile_color);
                }
            }
        }
    }
}

// Draw the pills
void draw_pills(void) {
    for (uint8_t row = 0; row < 23; row++) {
        for (uint8_t col = 0; col < 20; col++) {
            if (pacman_map[row][col] == 0) { // Corridor tile
                uint16_t pill_x = col * TILE_SIZE + TILE_SIZE / 2 - 1;
                uint16_t pill_y = row * TILE_SIZE + TILE_SIZE / 2 - 1;
                for (uint8_t i = 0; i < 2; i++) {
                    for (uint8_t j = 0; j < 2; j++) {
                        LCD_SetPoint(pill_x + i, pill_y + j, Magenta);
                    }
                }
            }
        }
    }
}


void display_lives(void) {
    static uint8_t lives = 1; // Start with 1 life

    // Increase lives every 1000 points
    if (score / 1000 >= lives) {
        lives++;
    }

    // Display "LIVES:" text
    GUI_Text(120, 280, (uint8_t *)"LIVES:", White, Black);

    // Draw hearts representing lives
    for (uint8_t i = 0; i < lives; i++) {
        uint16_t heart_x = 180 + i * 12; // Position hearts horizontally
        uint16_t heart_y = 280;

    // Draw a heart shape
			for (uint8_t j = 0; j < 14; j++) {
					for (uint8_t k = 0; k < 16; k++) {
							if ((j < 6 && ((k >= 3 - j / 2 && k <= 7 + j / 2) || (k >= 8 - j / 2 && k <= 12 + j / 2))) ||  // Upper rounded parts
									(j >= 6 && k >= (j - 6) && k <= (15 - (j - 6)))) {  // Lower triangle
									LCD_SetPoint(heart_x + k, heart_y + j, Red);
							}
					}
			}
   }
}


void update_time(void) {
    if (remaining_time > 0) {
        remaining_time--; // Decrease the countdown
    }

    // Clear the previous time display
    GUI_Text(0, 300, (uint8_t *)"TIME:    ", Black, Black);

    // Prepare the new time display
    char time_display[10];
    sprintf(time_display, "TIME: %02d", remaining_time);

    // Display the updated time
    GUI_Text(0, 300, (uint8_t *)time_display, White, Black);

    // If the timer reaches 0, display "GAME OVER" and stop the game
    if (remaining_time == 0) {
        disable_timer(0); // Stop TIM0 (game timer)
        disable_timer(1); // Stop TIM1 (countdown timer)
        GUI_Text(80, 140, (uint8_t *)"GAME OVER", White, Black);
    }
}


// 32MHZ


// Initialize the game and timer
void init_game(void) {
    init_pacman();
    draw_maze(Blue);
		draw_pills();

    // Initialize Timer
    init_timer(0, 0x00300000);  // Set TIM0
		init_timer(1,0x01441780);							// 1min 0x2CB41780
}


void continue_game(void) {
    // Clear the "PAUSED" text from the screen
    GUI_Text(120, 300, (uint8_t *)"PAUSED", Black, Black);

    // Set game state to running
    game_running = 1;

    // Start the timer
    enable_timer(0);
		enable_timer(1);
}


// Main game loop
void run_game(void) {

    game_running = 1;

    enable_timer(0); // Start the timer
		enable_timer(1);
    while (1) {
        __ASM("wfi");  // Wait for interrupt
    }
}


void pause_game(void) {
		
		game_running = 0;
	
    // Stop the timer
    disable_timer(0);
		disable_timer(1);

    // Display "PAUSED" in the middle of the screen
    GUI_Text(120, 300, (uint8_t *)"PAUSED", White, Black);
		
}

