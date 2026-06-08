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
#include "../../timer_2026/include/timer_2026.h"

// ======================================================= GPIO ======================================================= 
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

/**
 * @brief enumerations for access vector button elements
 */
typedef enum{
	BSP_VECTOR_BTN0 = 0,
	BSP_VECTOR_BTN1 = 1,
} bsp_ivector_button_t;

/**
 * @brief struct for vector button elements
 * @details 
 * Each element of vector has 2 components (bsp_actual_state & bsp_last_state)
 */
typedef struct{
	bool bsp_actual_state;
	bool bsp_last_state;
} bsp_button_state_t;

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

/**
 * @brief function to update the last button state
 * @param button the button desired to store his last state.
 * @brief this function stores actual state (bsp_actual_state) in last state (bsp_last_state)
 */ 
void bsp_update_last_btn_state(int button);

/**
 * @brief function to get the actual button state
 * @param button the button desired to get his actual state.
 * @return bool .bsp_actual_state button element structure
 */ 
bool bsp_btn_get_actual_state(int button);

/**
 * @brief function to get the last button state
 * @param button the button desired to get his last state.
 * @return bool .bsp_last_state button element structure
 */ 
bool bsp_btn_get_last_state(int button);

// ======================================================= TIMER =======================================================
#define BSP_TIMER0 LL_TIMG0_T0
#define BSP_TIMER1 LL_TIMG0_T1
#define BSP_TIMER2 LL_TIMG1_T0
#define BSP_TIMER3 LL_TIMG1_T1

#define BSP_UP    LL_UP
#define BSP_DOWN  LL_DOWN
#define BSP_LEVEL LL_LEVEL
#define BSP_EDGE  LL_EDGE
 
/**
 * @brief structure for timer configuration
 * @details
 * This structure store all data configuration for timer, in order to configurate
 * a timer the user have to send this structure to bsp_timer_init() function.
 */
typedef struct{
	ll_timer_t 		bsp_timer;
	int 			bsp_freq_divider;
	ll_count_mode_t bsp_count_mode;
	bool 			bsp_alarm_enable;
	bool 			bsp_autoreload_enable;
	ll_int_mode_t 	bsp_int_mode;
	uint64_t 		bsp_alarm_value;
	uint64_t 		bsp_load_value;
} bsp_timer_config_t;

/**
 * @brief function for timer config
 * @param timer_cfg receives a structure which stores all timer config data
 * @details
 * This functions configure the timer with all data stored in the structure timer_cfg received
 */
void bsp_timer_init(bsp_timer_config_t *timer_cfg);


/**
 * @brief function reset timer
 * @param timer timer which his alarm will reset
 * @details
 * chinese people says it isn't needed but it needed :(
 */
void bsp_reset_timer_loop(ll_timer_t timer);

#endif
