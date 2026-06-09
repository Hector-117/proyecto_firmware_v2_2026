/**
 * @file hal.h
 * @brief Hardware Abstraction Layer (HAL) header file for ESP32.
 *
 * @details
 * This file contains function, enums, and typdef pointer callback declaration for 
 * expose abstract functionality of board elements.
 *
 * @author Hector Said Herrera Niño
 * @author José Francisco Padilla Torres
 * @date 2026-06-07
 */

#ifndef HAL_2026
#define HAL_2026

//# include "../../bsp_board/include/bsp_board.h"
# include "bsp_board.h"
#include "esp_intr_alloc.h"

#define HAL_USER_BTN0 BSP_PUSH_BUTTON_0
#define HAL_USER_BTN1 BSP_PUSH_BUTTON_1

/**
 * @brief enumerations for define RGB color
 * @details this constants are intended to be plugged in hal_set_RGB_color() as argument
 */
typedef enum{
	HAL_WHITE	= 0,
	HAL_CYAN 	= 1,
	HAL_MAGENTA = 2,
	HAL_BLUE 	= 3,
	HAL_YELLOW  = 4,
	HAL_GREEN 	= 5,
	HAL_RED 	= 6,
	HAL_BLACK	= 7
} hal_color_t;

/**
 * @brief Initialize the boards components.
 */
void hal_esp_init (void);

/**
 * @brief typedef pointer to function
 * @details this typedef is for a pointer to function that return void and has as parameter void.
 * this is used for callbacks in hal_falling_edge() & hal_rising_edge() functions
 */
typedef void (*hal_callback_t)(void);

/**
 * @brief excecutes callback when detects falling edge
 * @param button the button which is gonna be detected the falling edge
 * @param callback function that will be excecuted when falling edge is detected
 */
void hal_falling_edge(int button, hal_callback_t callback);

/**
 * @brief excecutes callback when detects rising edge
 * @param button the button which is gonna be detected the rising edge
 * @param callback function that will be excecuted when rising edge is detected
 */
void hal_rising_edge(int button, hal_callback_t callback);

/**
 * @brief Writes a value on the status vector
 * @param vector_value the value in hexadecimal
 * @details 
 * The function receives 8 bits, but only takes the first 5 less significant
 * bits, and it will turn on when bit value is 1, and turn off when bit value is 0
 */
void hal_status_vector(uint8_t vector_value);

/**
 * @brief Writes a value on the RGB led
 * @param vector_value the value in hexadecimal
 * @details
 * The function receives 8 bits, but only takes the first 3 less significant bits.
 * it will turn on the led when bit value is 1, and turn off when bit value is 0.
 * @note is easier to set a color value using hal_color_t enumerations
 */
void hal_set_RGB_color(uint8_t vector_value);

/**
 * @brief Reads the status and makes a toggle
 */
void hal_toggle_status_vector();

/**
 * @brief Reads the value of the vector 
 * @param button the button which is wanted to know his actual state
 * @details
 * this function returns the .bsp_actual_state value
 */
bool hal_btn_get_actual_state(int button);

/**
 * @brief function to call a callback periodically
 * @param time_us time period when the callback will be called
 * @param callback function pointer to callback
 * @details
 * This function configure timer and isr. always will configure for:
 * module 0 timer 0.
 * @note the callback is void return and void arguments
 */
void hal_periodic_fun(int time_us, hal_callback_t callback);


void hal_delay(int time_us);
#endif

