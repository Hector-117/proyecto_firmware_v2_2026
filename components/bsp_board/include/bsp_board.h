/**
 * @file bsp_board.h
 * @brief Board Support Package header file for ESP32.
 *
 * @details
 * This file contains function declaration for 
 * handling board elements (leds, RGB and buttons)
 *
 * @author Hector Said Herrera Niño
 * @author José Francisco Padilla Torres
 * @date 2026-06-01
 */
#ifndef BSP_BOARD_H
#define BSP_BOARD_H

#include "../../gpio_2026/include/gpio_2026.h"

// =============== Name abstraction for board =============== 
#define BSP_LED0 LL_GPIO2
#define BSP_LED1 LL_GPIO4
#define BSP_LED2 LL_GPIO16
#define BSP_LED3 LL_GPIO17
#define BSP_LED4 LL_GPIO5

#define BSP_RGB_REDLED   LL_GPIO14
#define BSP_RGB_GREENLED LL_GPIO13
#define BSP_RGB_BLUELED  LL_GPIO12

#define BSP_PUSH_BUTTON_0 LL_GPIO18
#define BSP_PUSH_BUTTON_1 LL_GPIO19

/**
 * @brief enumerations for access vector leds elements
 */
typedef enum{
	BSP_VECTOR_LED0 = 0,
	BSP_VECTOR_LED1 = 1,
	BSP_VECTOR_LED2 = 2,
	BSP_VECTOR_LED3 = 3,
	BSP_VECTOR_LED4 = 4,
} bsp_ivector_led_t;

/**
 * @brief enumerations for access vector RGB elements
 */
typedef enum{
	BSP_VECTOR_LEDRED = 0,
	BSP_VECTOR_LEDGREEN = 1,
	BSP_VECTOR_LEDBLUE = 2,
} bsp_ivector_rgb_t;

//  =============== Vector state of board leds =============== 
/**
 * @brief get the value of the bit in vector led.
 * @param bit desired bit position value.
 * @return the bit value
 */
bool bsp_get_bit_vector_led(uint8_t bit);

//  =============== Vector state of RGB vector ===============
/**
 * @brief get the value of the bit in vector RGB.
 * @param bit desired bit position value.
 * @return the bit value
 */ 
bool bsp_get_bit_vector_RGB(uint8_t bit);

// =============== Function prototypes for BSP =============== 
/**
 * @brief Initialize the boards components.
 */
void bsp_init(void);

// Abstraction for board leds
/**
 * @brief turn on the led.
 * @param led desired led turn on.
 */ 
void bsp_led_on(int led);

/**
 * @brief turn off the led.
 * @param led desired led turn off.
 */ 
void bsp_led_off(int led);

/**
 * @brief toggle the led.
 * @param led desired led to toggle.
 */ 
void bsp_led_toggle(int led);

// Abstraction for RGB leds
/**
 * @brief turn on the RGB led.
 * @param led desired led (RED, GREEN or BLUE) turn on.
 */ 
void bsp_RGB_on(int led);

/**
 * @brief turn off the RGB led.
 * @param led desired led (RED, GREEN or BLUE) turn off.
 */ 
void bsp_RGB_off(int led);

/**
 * @brief toggle the RGB led.
 * @param led desired led (RED, GREEN or BLUE) to toggle.
 */ 
void bsp_RGB_led_toggle(int led);


// Abstraction for board buttons
/**
 * @brief checks if a button is pressed.
 * @param button the button desired to know his state.
 * @return the button state
 */ 
bool bsp_pressed_button(int button);



#endif
